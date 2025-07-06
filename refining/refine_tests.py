import os
import requests
import json
import yaml

def load_yaml_instructions(yaml_path):
    with open(yaml_path, 'r') as f:
        yaml_data = yaml.safe_load(f)
    return "\n".join(["- " + rule for rule in yaml_data.get("rules", [])])

def refine_tests(tests_dir, yaml_instruction_path, output_dir="../refined_tests", model="qwen2.5-coder:3b", api_url="http://localhost:11434/api/chat"):
    if not os.path.exists(tests_dir):
        print(f"❌ Error: Test directory not found: {tests_dir}")
        return

    test_files = [f for f in os.listdir(tests_dir) if f.endswith(".cpp")]
    if not test_files:
        print("⚠️ No .cpp test files found in the tests folder.")
        return

    os.makedirs(output_dir, exist_ok=True)
    yaml_instructions = load_yaml_instructions(yaml_instruction_path)

    for test_file in test_files:
        test_path = os.path.join(tests_dir, test_file)
        print(f"\n🛠️ Refining test: {test_file}")

        with open(test_path, 'r') as f:
            test_code = f.read()

        prompt = f"""You are a C++ unit test expert. Refine the following test file according to these rules:

{yaml_instructions}

```cpp
{test_code}
```"""

        payload = {
            "model": model,
            "messages": [{"role": "user", "content": prompt}]
        }

        try:
            response = requests.post(api_url, json=payload, stream=True)
        except requests.ConnectionError:
            print("❌ Ollama server is not running.")
            return

        if response.status_code != 200:
            print(f"❌ Error {response.status_code}: {response.text}")
            continue

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
            print(f"⚠️ No refined content for: {test_file}")
            continue

        # Save refined test to new folder with same file name
        output_path = os.path.join(output_dir, test_file)
        with open(output_path, "w") as f:
            f.write(generated)

        print(f"✅ Refined test saved: {output_path}")

if __name__ == "__main__":
    refine_tests("../tests", "refine_instruction.yaml", output_dir="../refined_tests")
