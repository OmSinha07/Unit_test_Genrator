import os
import subprocess
import requests
import textwrap

# ✅ CONFIGURATION
MODEL_DIR = "../orgChartApi/models"
TEST_DIR = "../orgChartApi/test"
OUTPUT_DIR = "../build"
GTEST_INCLUDE = "../googletest/googletest/include"
GTEST_LIB = "../googletest/build/lib"
OLLAMA_URL = "http://localhost:11434/api/generate"
OLLAMA_MODEL = "qwen2.5-coder:3b"

os.makedirs(OUTPUT_DIR, exist_ok=True)

def compile_test(model_cc, test_cpp, executable):
    # Use full static library paths for third_party dependencies if present
    jsoncpp_lib = "../orgChartApi/third_party/jsoncpp/lib/libjsoncpp.a"
    trantor_lib = "../orgChartApi/third_party/trantor/lib/libtrantor.a"
    drogon_lib = "../orgChartApi/third_party/drogon/lib/libdrogon.a"
    compile_cmd = [
        "g++", "-std=c++17", model_cc, test_cpp,
    "-I", os.path.dirname(model_cc),
    "-I", GTEST_INCLUDE,
    "-I", "../",                         # For project includes
    "-I", "/usr/local/include",           # Drogon & Trantor includes
    "-I", "../orgChartApi",
    "-I", "../orgChartApi/third_party/jsoncpp/include",  # <-- Add JsonCpp include if present
    "-I", "../orgChartApi/third_party/trantor/include",  # <-- Add Trantor include if present
    "-I", "../orgChartApi/third_party/drogon/include",   # <-- Add Drogon include if present

    "-L", "/usr/local/lib",
    "-L", GTEST_LIB,
    "-L", "../orgChartApi/third_party/jsoncpp/lib",      # <-- Add JsonCpp lib if present
    "-L", "../orgChartApi/third_party/trantor/lib",      # <-- Add Trantor lib if present
    "-L", "../orgChartApi/third_party/drogon/lib",       # <-- Add Drogon lib if present

    "-lssl", "-lcrypto",
    "-lgtest", "-lpthread",
    jsoncpp_lib,
    trantor_lib,
    drogon_lib,

    "-o", os.path.join(OUTPUT_DIR, "test_executable")
    ]
    result = subprocess.run(compile_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    # Save build log to a file for each model
    build_log_path = os.path.join(OUTPUT_DIR, f"{os.path.basename(model_cc).replace('.cc','')}_build_output.txt")
    with open(build_log_path, "w") as log_file:
        log_file.write(result.stdout + "\n" + result.stderr)
    return result.returncode == 0, result.stdout + "\n" + result.stderr

def run_test_and_coverage(model_cc, executable):
    run = subprocess.run([executable], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    test_output = run.stdout + "\n" + run.stderr

    gcov_cmd = ["gcov", model_cc]
    gcov = subprocess.run(gcov_cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    coverage_output = gcov.stdout + "\n" + gcov.stderr

    coverage_percent = "0"
    for line in gcov.stdout.splitlines():
        if "Lines executed:" in line:
            coverage_percent = line.split(":")[1].split("%")[0].strip()
            break

    return run.returncode == 0, test_output, coverage_percent, coverage_output

def call_ollama(prompt_text):
    payload = {
        "model": OLLAMA_MODEL,
        "prompt": prompt_text,
        "stream": False
    }
    response = requests.post(OLLAMA_URL, json=payload)
    response.raise_for_status()
    content = response.json()["response"]
    print("\n🤖 Ollama Response:\n", content)
    return content

def generate_yaml_prompt(filename, source_code, log, coverage=None, test_output=None):
    if coverage is None:
        return f"""
prompt: |
  The following C++ Google Test code failed to build. Please fix it.

  ---
  filename: {filename}
  source_code: |
{textwrap.indent(source_code, '    ')}
  build_log: |
{textwrap.indent(log, '    ')}
  ---
  Return only the corrected code, no explanations.
""".strip()
    else:
        return f"""
prompt: |
  Improve this Google Test code:
  1. Remove duplicates.
  2. Improve naming.
  3. Suggest better test coverage.

  ---
  filename: {filename}
  source_code: |
{textwrap.indent(source_code, '    ')}
  test_output: |
{textwrap.indent(test_output, '    ')}
  coverage: "{coverage}%"
  ---
  Return only the improved test code, no explanation.
""".strip()

def main():
    print(f"🔍 Scanning models in {MODEL_DIR} and tests in {TEST_DIR}...")

    models = [f for f in os.listdir(MODEL_DIR) if f.endswith('.cc')]
    tests = [f for f in os.listdir(TEST_DIR) if f.endswith('_test.cpp') or f.endswith('_test.cc')]

    for model in models:
        model_name = model.replace('.cc', '')
        model_cc = os.path.join(MODEL_DIR, model)
        # Accept both .cpp and .cc for test files
        expected_test_names = [f"{model_name}_test.cpp", f"{model_name}_test.cc"]
        found_test = None
        for test_name in expected_test_names:
            if test_name in tests:
                found_test = test_name
                break
        if not found_test:
            print(f"⚠️ Skipping {model} (no matching test found)")
            continue

        test_cpp = os.path.join(TEST_DIR, found_test)
        executable = os.path.join(OUTPUT_DIR, f"run_{model_name}_tests")

        print(f"\n🚀 Compiling and testing: {model_cc} + {test_cpp}")

        with open(test_cpp, 'r') as f:
            test_code = f.read()

        build_success, build_log = compile_test(model_cc, test_cpp, executable)

        if not build_success:
            print("❌ Build failed.")
            print(build_log)  
            yaml_prompt = generate_yaml_prompt(found_test, test_code, build_log)
            call_ollama(yaml_prompt)
            continue

        print("✅ Build succeeded. Running tests...")
        tests_passed, test_output, coverage, gcov_output = run_test_and_coverage(model_cc, executable)

        if not tests_passed:
            print("❌ Tests failed.")
            yaml_prompt = generate_yaml_prompt(found_test, test_code, test_output)
            call_ollama(yaml_prompt)
            continue

        print(f"✅ Tests passed. Coverage: {coverage}%")

        if float(coverage) < 80:
            print("⚠️ Coverage below threshold. Sending for improvement...")
            yaml_prompt = generate_yaml_prompt(found_test, test_code, gcov_output, coverage, test_output)
            call_ollama(yaml_prompt)
        else:
            print("🎉 All good. No action needed for this file.")

if __name__ == "__main__":
    main()
