# AstroParse — Code to Cosmic JSON 🚀

[![License: MIT](https://img.shields.io/badge/License-MIT-purple.svg)](https://opensource.org/licenses/MIT)
[![Docker Image](https://img.shields.io/docker/pulls/silvesterhsu/astroparse.svg)](https://hub.docker.com/r/silvesterhsu/astroparse)

**Turn C++ code into LLM-digestible JSON – because AI deserves X-ray vision for code!**
Powered by Clang AST dissection, Bazel/Docker industrial pipelines, and JSON that pleases both machines and humans.

------

## 🔥 Features

- **C++ AST → JSON**: Surgical precision with Clang's parsing scalpel
- **Bazel Builds**: Industrial-grade compilation with dependency isolation
- **Docker in One Shot**: No dependency hell – just warp-speed deployment
- **Pre-built Binaries**: For rebels who hate compiling from source

------

## 🛠 Installation

### Prerequisites

- Docker (recommended)
- Linux (Windows warriors: WSL2 required)

### Pre-built Release (For Binary Cowboys)

``` bash
./astroparse-linux-amd64 your_code.cpp 
```

### Build from source

Init docker container for build
``` bash
./docker.sh init
```

Enter the build env
``` bash
./docker.sh
```

Build the code
``` bash
./scripts/build.sh
```

### Generate AST json for example

Gen `compile_commands.json`
``` bash
./scripts/update_compile_commands.sh
```

Get AST json file
```bash
./bazel-bin/astroparse/astroparse /AstroParse/parse/util.cc
```


------

## 🚀 Demo

### Input: `demo.cpp`

``` cpp
#include <iostream>  
class SpaceShip {  
public:  
    void launch() { std::cout << "3...2...1...Ignition!\n"; }  
};  
```

### Output: `ast.json` (snippet)

``` json
{  
  "AST": {  
    "type": "TranslationUnitDecl",  
    "children": [  
      {  
        "type": "ClassDecl",  
        "name": "SpaceShip",  
        "methods": [  
          {  
            "type": "CXXMethodDecl",  
            "name": "launch",  
            "body": "CompoundStmt"  
          }  
        ]  
      }  
    ]  
  }  
}  
```

------

## 📡 Architecture

```
[Your C++ Code] → Clang Frontend → AST → Custom Traverser → JSON Serializer → [Feed to LLMs/Tools]  
```

------

## 🤖 Contributing

1. **Fork This Repo**: Clone your personal universe branch
2. **Code Style**: Follow `.clang-format` – follow Google style
3. **Tests**: All PRs must pass `./scripts/test.sh`

------

## ⚖️ License

**MIT License** – Details at [LICENSE](https://github.com/SilvesterHsu/AstroParse/blob/main/LICENSE)

------

**Crafted with ❤️ and Clang by Seel.Xu**

*“Any sufficiently advanced code parser is indistinguishable from magic.”* – Arthur C. Clarke (remixed)
