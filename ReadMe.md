# Unit Test Generator using Qwen 3B

> **Assignment 5: Keploy API FELLOWSHIP**  
> This project demonstrates the automation of unit test generation, refinement, build, and coverage reporting for a C++ application using Qwen 3B (via Ollama) and traditional tools like Google Test and LCOV.

**Target Codebase:** [keploy/orgChartApp](https://github.com/keploy/orgChartApi)

---

## 📁 Project Structure

```
unit_test_generator/
├── buildlog/                     # Build logs and coverage reports (LCOV)
├── googletest/                   # Google Test framework
├── initial_test_generator/       # Scripts + AI outputs for initial test generation
├── orgChartApi-codebase/         # C++ source code (models, controllers)
├── refining/                     # Scripts + refined unit tests
├── test/                         # AI-generated unit tests (initial/final)
├── test_runner/                  # Scripts to build, run, and generate coverage
├── yaml_prompts/                 # YAML prompts guiding the AI (Qwen 3B)
├── CMakeLists.txt               # CMake configuration
└── README.md                    # This file
```

---

## 🛠 Technologies Used

- **C++** - Target application language
- **Python** - Automation scripts
- **Google Test (GTest)** - Unit testing framework
- **LCOV + gcov** - Test coverage analysis
- **Ollama + Qwen 3B** - Local Large Language Model

---

## 🔄 End-to-End Workflow

### Step 1: Generate Initial Unit Tests
**Location:** `initial_test_generator/`

- Uses Qwen 3B to generate initial test cases from the C++ models
- Prompt controlled by: `yaml_prompts/generate_tests.yaml`
- Output stored in: `test/`

```bash
python initial_test_generator/generate_tests.py --input orgChartApi-codebase/models --output test/
```

### Step 2: Refine Unit Tests
**Location:** `refining/`

The initial test cases are improved by Qwen 3B to:
- Remove duplicates
- Add missing libraries
- Follow best GTest practices

- Prompt controlled by: `yaml_prompts/refine_tests.yaml`
- Output stored in: `refining/`

```bash
python refining/refine_tests.py --input test/ --output refining/
```

### Step 3: Build, Test & Generate Coverage
**Location:** `test_runner/`

- Runs CMake and builds the code + tests
- **If build fails:**
  - Errors and logs are captured in `buildlog/`
  - AI fixes them using `yaml_prompts/fix_build.yaml`
- **On success:**
  - Tests executed
  - LCOV coverage reports generated: `buildlog/coverage_report/index.html`

```bash
python test_runner/build_and_coverage.py --source orgChartApi-codebase --tests refining/
```

---

## 📊 Test Coverage Results

| Metric | Value |
|--------|-------|
| **Line Coverage** | 55.1% overall |
| **Function Coverage** | 43.04% overall |
| **Model Coverage** | Up to 68% |

**Coverage Report Path:** ``
### Sample Coverage Report
![Coverage Report Screenshot](coverage/Screenshot%202025-07-05%20163759.png)
*LCOV Coverage Report showing detailed line and function coverage metrics*
---

## 🚀 Quick Start Guide

### Prerequisites

1. **Install Dependencies:**
   - Google Test
   - LCOV + gcov
   - Ollama with Qwen 3B

2. **Setup Ollama:**
   ```bash
   ollama run qwen:3b
   ```

### Running the Project

1. **Generate Initial Tests:**
   ```bash
   python initial_test_generator/generate_tests.py
   ```

2. **Refine Tests:**
   ```bash
   python refining/refine_tests.py
   ```

3. **Build and Generate Coverage:**
   ```bash
   python test_runner/run_models.py
   ```

4. **View Coverage Report:**
   Open `buildlog/coverage_report/index.html` in your browser

---

## 🔧 Build Error Handling

The system automatically handles common build errors including:
- Missing headers
- Incorrect function names
- Typos or signature mismatches

**Error Resolution Process:**
- Build errors are automatically detected and logged
- AI analyzes errors using strict YAML prompts
- Corrections are applied automatically
- Build process retries until success

---

## 💡 Key Features

- **End-to-end AI-powered unit test automation** for C++ using Qwen 3B
- **Achieved over 55% code coverage** in first iteration
- **Automated build-error fixing loop** with AI assistance
- **Reproducible, simple, and adaptable** to any C++ project
- **Local LLM integration** for privacy and cost-effectiveness

---

## 📈 Future Enhancements

- Improve coverage from 55% to 75%+
- Support for additional programming languages
- Integration with CI/CD pipelines
- Web-based dashboard for non-technical users
- Custom model training for domain-specific testing

---

## 🤝 Contributing

This project is part of the Keploy API Fellowship. Contributions and improvements are welcome!

---

## 📄 License

This project is submitted as Assignment 5 for Keploy's API Fellowship.

---

## 🔗 Related Links

- [Keploy](https://keploy.io/)
- [Target Codebase: orgChartApp](https://github.com/keploy/orgChartApp)
- [Ollama](https://ollama.ai/)
- [Google Test](https://github.com/google/googletest)