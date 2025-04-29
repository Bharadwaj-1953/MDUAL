<h1 align="center">
Multiple Dynamic Outlier-Detection from a Data Stream<br>
by Exploiting Duality of Data and Queries
</h1>


## 📝 Abstract
<div align="justify">
  
Detecting outliers in real time from continuously evolving data streams is crucial for applications such as network security, financial monitoring, and sensor networks. Traditional streaming outlier-detection algorithms typically assume a fixed set of distance-based queries and process each one independently, leading to redundant computations and significant memory and time overhead, especially when queries change dynamically over time.

In this project, we present MDUAL, a novel algorithm based on duality-based unified processing. By exploiting the natural duality between data points and queries, MDUAL groups similar queries and clusters of nearby data points together, enabling efficient shared and incremental computation. Two key techniques, data-query grouping, which merges queries with overlapping data regions, and prioritized group processing, which dynamically updates query results as new data arrives, allow MDUAL to significantly reduce the number of distance computations and memory usage compared to traditional methods.

We implemented MDUAL in C++17, with a strong focus on real-time performance, low memory overhead, and modular, extensible design. Through comprehensive experiments on six benchmark datasets, ranging from 1-dimensional stock market data to 55-dimensional forest cover data, we demonstrate that MDUAL achieves up to 221× speedup and consumes up to 13× less memory compared to state-of-the-art methods like SOP and pMCSKY. These results highlight that MDUAL provides a scalable and efficient solution for dynamic, multi-query outlier detection in modern high-speed data stream environments.

</div>

---



## How to Run the Code

### 📦 Prerequisites
- **Compiler**: `clang++` or `g++` with **C++17** support.
- **Platform**: Linux or macOS (Windows with WSL also supported).

---

### 🔧 Compilation Instructions

1. Navigate to the project root directory:

```bash
cd src
```

2. Compile the project using the command below:

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
```

If you prefer to use ```g++```, simply replace ```clang++``` with ```g++``` in the command.


3. After compilation, run the executable using:
```bash
./testLoad
```

4. When prompted, provide the dataset input. For example, to select the Stock Market dataset:
```bash
STK
```


## 🗂️ Data Sets

| Name | # Data Points | # Dimensions | Size  | Link |
|:----:|:-------------:|:------------:|:-----:|:----:|
| STK  | 1.05M          | 1             | 7.57MB | [link](https://infolab.usc.edu/Luan/Outlier/Datasets/stock.txt) |
| TAO  | 0.58M          | 3             | 10.7MB | [link](https://infolab.usc.edu/Luan/Outlier/Datasets/tao.txt) |
| HPC  | 1M             | 7             | 28.4MB | [link](https://infolab.usc.edu/Luan/Outlier/Datasets/household2.txt) |
| GAS  | 0.93M          | 10            | 70.7MB | [link](http://archive.ics.uci.edu/ml/machine-learning-databases/00362/HT_Sensor_UCIsubmission.zip) |
| EM   | 1M             | 16            | 119MB  | [link](https://infolab.usc.edu/Luan/Outlier/Datasets/ethylene.txt) |
| FC   | 1M             | 55            | 72.2MB | [link](https://infolab.usc.edu/Luan/Outlier/Datasets/fc.data) |

## 📊 Experimental Results and Analysis

- Extensive experiments were conducted using the six benchmark datasets to evaluate the runtime performance, memory efficiency, and outlier detection accuracy of the MDUAL algorithm.  
- Detailed runtime graphs, memory usage charts, and full experimental analysis are available upon request.  
- If you would like access to the complete results, visualizations, or further discussions, please feel free to contact me.

🌐 **Contact:** 
- **Email**: manne.bharadwaj.1953@gmail.com
- **LinkedIn**: [Bharadwaj Manne](https://www.linkedin.com/in/bharadwaj-manne-711476249/)
