import os
import requests
import json
import yaml

def load_yaml_instructions(yaml_path):
    with open(yaml_path, 'r') as f:
        yaml_data = yaml.safe_load(f)
    return "\n".join(["- " + rule for rule in yaml_data.get("rules", [])])

def generate_test_from_cpp(cpp_path, yaml_instruction_path, output_dir="../tests", model="qwen2.5-coder:3b", api_url="http://localhost:11434/api/chat"):
    if not os.path.isfile(cpp_path):
        print(f"❌ File not found: {cpp_path}")
        return

    with open(cpp_path, 'r') as f:
        cpp_code = f.read()

    yaml_instructions = load_yaml_instructions(yaml_instruction_path)

    prompt = f"""You are an AI assistant generating Google Test unit tests for the following C++ code.

Instructions:
{yaml_instructions}

C++ Code:
```cpp
{cpp_code}
```"""

    payload = {
        "model": model,
        "messages": [{"role": "user", "content": prompt}]
    }

    try:
        response = requests.post(api_url, json=payload, stream=True)
    except requests.ConnectionError:
        print("❌ Cannot connect to Ollama. Is it running?")
        return

    if response.status_code != 200:
        print(f"❌ Error {response.status_code}: {response.text}")
        return

    generated = ""
    for line in response.iter_lines(decode_unicode=True):
        if line:
            try:
                data = json.loads(line)
                if "message" in data and "content" in data["message"]:
                    generated += data["message"]["content"]
            except json.JSONDecodeError:
                continue

    if not generated.strip():
        print(f"⚠️ No test code generated for {cpp_path}")
        return

    # Print on terminal
    print("\n🧪 Generated Test Code:\n")
    print(generated)

    # Save to root-level /tests/
    os.makedirs(output_dir, exist_ok=True)
    file_name = os.path.splitext(os.path.basename(cpp_path))[0] + "_test.cpp"
    output_path = os.path.join(output_dir, file_name)

    with open(output_path, "w") as f:
        f.write(generated)

    print(f"✅ Saved: {output_path}")

def collect_cpp_files(folder):
    cpp_files = []
    for root, _, files in os.walk(folder):
        for file in files:
            print(f"🔍 Scanning file: {file}")
            if file.endswith((".cc", ".cpp", ".cxx", ".c", ".h")):
                full_path = os.path.join(root, file)
                cpp_files.append(full_path)
                print(f"📄 Added for unit testing: {full_path}")
    return cpp_files

if __name__ == "__main__":
    print("🚀 Starting unit test generation...\n")

    base_path = "../codebase/orgChartApi"
    instruction_file = "strict_instruction.yaml"
    output_test_dir = "../tests"
    folders_to_scan = ["models"]  # ✅ Only generate for models

    for folder in folders_to_scan:
        full_path = os.path.join(base_path, folder)
        print(f"\n📂 Scanning: {full_path}")
        if os.path.exists(full_path):
            cpp_files = collect_cpp_files(full_path)
            if not cpp_files:
                print(f"⚠️ No .cc/.cpp/.h files found in: {full_path}")
            for cpp_file in cpp_files:
                generate_test_from_cpp(cpp_file, instruction_file, output_dir=output_test_dir)
        else:
            print(f"❌ Folder does not exist: {full_path}")

    print("\n✅ Done.")
