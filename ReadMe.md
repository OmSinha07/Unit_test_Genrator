 Assignment 5: Unit Test Generator using Qwen 3B (Keploy AI Sprint)
This project is submitted as Assignment 5 for Keploy's AI Automation Sprint.
It demonstrates the automation of unit test generation, refinement, build, and coverage reporting for a C++ application using Qwen 3B (via Ollama) and traditional tools like Google Test and LCOV.

Target Codebase 👉 keploy/orgChartApi

📁 Project Structure
'''
unit_test_generator/
├── buildlog/                     # Build logs and coverage reports (LCOV)
├── googletest/                    # Google Test framework
├── initial_test_generator/        # Scripts + AI outputs for initial test generation
├── orgChartApi-codebase/          # C++ source code (models, controllers)
├── refining/                      # Scripts + refined unit tests
├── test/                          # AI-generated unit tests (initial/final)
├── test_runner/                   # Scripts to build, run, and generate coverage
├── yaml_prompts/                  # YAML prompts guiding the AI (Qwen 3B)
├── CMakeLists                     # CMake configuration
└── ReadMe                         # Assignment report (this file)


'''
🛠 Technologies Used
C++

Python

Google Test (GTest)

LCOV + gcov for test coverage

Ollama + Qwen 3B (Local LLM)

🔄 End-to-End Workflow
➤ Step 1: Generate Initial Unit Tests (initial_test_generator/)
Uses Qwen 3B to generate initial test cases from the C++ models.

Prompt controlled by: yaml_prompts/generate_tests.yaml

Output stored in: test/

✅ Command:

python initial_test_generator/generate_tests.py --input orgChartApi-codebase/models --output test/
➤ Step 2: Refine Unit Tests (refining/)
The initial test cases are improved by Qwen 3B to:

Remove duplicates

Add missing libraries

Follow best GTest practices

Prompt controlled by: yaml_prompts/refine_tests.yaml

Output stored in: refining/

✅ Command:
python refining/refine_tests.py --input test/ --output refining/
➤ Step 3: Build, Test & Generate Coverage (test_runner/)
Runs CMake and builds the code + tests.

If build fails:

Errors and logs are captured in buildlog/

AI fixes them using yaml_prompts/fix_build.yaml

On success:

Tests executed

LCOV coverage reports generated: buildlog/coverage_report/index.html

✅ Command:

python test_runner/build_and_coverage.py --source orgChartApi-codebase --tests refining/
📊 Test Coverage Achieved
Metric	Value
Line Coverage	55.1% overall
Function Coverage	43.04% overall
Model Coverage	Up to 68%

✅ Coverage Report Path:

buildlog/coverage_report/index.html
✅ Sample Coverage Report Screenshot:
(Add your LCOV screenshot here)

✅ Build Error Handling
Common errors like:

Missing headers

Incorrect function names

Typo or signature mismatch

Automatically fixed using Qwen 3B and strict YAML prompts.

🚀 How to Run This Project (Quick Start)
Install:

Google Test

LCOV + gcov

Ollama with Qwen 3B (ollama run qwen:3b)

Run commands:

python initial_test_generator/generate_tests.py
python refining/refine_tests.py
python test_runner/build_and_coverage.py
Open coverage report in browser.


💡 Highlights
End-to-end AI-powered unit test automation for C++ using Qwen 3B.

Achieved over 55% code coverage in first iteration.

Automated build-error fixing loop.

Reproducible, simple, and adaptable to any C++ project.