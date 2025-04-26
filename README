# Multiple Dynamic Outlier-Detection from a Data Stream by Exploiting Duality of Data and Queries

## Abstract
Real-time outlier detection from a data stream has become increasingly important in the current hyperconnected world.  
This project focuses on an important yet often unaddressed challenge in continuous outlier detection: the multiplicity and dynamicity of queries.  
This challenge arises because outliers evolve over time, but most state-of-the-art algorithms cannot handle multiple dynamic queries effectively — they process a fixed set of queries separately for each data point.

**MDUAL** is a novel algorithm based on a new idea called **duality-based unified processing**.  
By exploiting the **duality of data and queries**, MDUAL groups similar data points and similar queries together, allowing efficient and incremental processing.

Two main techniques embody the idea:
- **Data-Query Grouping**
- **Prioritized Group Processing**

Comprehensive experiments demonstrated that MDUAL achieves **216x to 221x faster execution** while consuming **11x to 13x less memory** compared to state-of-the-art algorithms, thus effectively addressing the multiplicity–dynamicity challenge in streaming environments.

---

## How to Run the Code

### 📦 Prerequisites
- **Compiler**: `clang++` or `g++` with **C++17** support.
- **Platform**: Linux or macOS (Windows with WSL also supported).

---

### 🔧 Compilation Instructions

In the project root directory, compile using:

```bash
clang++ -std=c++17 \
  test/testLoad.cpp \
  MDUAL/MDUAL.cpp \
  MDUAL/Utils.cpp \
  MDUAL/Cell.cpp \
  MDUAL/globalCell.cpp \
  simulator/QueryGenerator.cpp \
  simulator/Simulator.cpp \
  simulator/MemoryThread.cpp \
  loader/DataLoader.cpp \
  loader/QueryLoader.cpp \
  simulator/Measure.cpp \
  -o testLoad
