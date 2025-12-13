# XML Editor & Social Network Analyzer  
A C++ Desktop Application for XML Parsing, Visualization, and Network Analysis

---

## 📌 Overview  
This project is developed as part of **CSE331: Data Structures & Algorithms**.  
The application processes XML files representing a **social network**, where each user has:

- A unique **ID**
- **Name**
- List of **Posts**
- List of **Followers**

The system provides two operation modes:

1. **Command-Line Interface (CLI)**
2. **Graphical User Interface (GUI)**

It supports XML validation, formatting, compression, converting to JSON, network graph building, and advanced analysis. 

---

## 🎯 Project Objectives  
- Learn how to **parse XML & JSON**
- Apply optimal **data structures**
- Build a functional **GUI**
- Visualize social network **graphs**
- Implement real-world **file-processing tools**  

---

## ✨ Features  

### **Level 1 — Core Features**
✔ Read & Process XML Files  
✔ XML Consistency Checking  
✔ Auto-Fixing XML Structural Errors  
✔ XML Prettifying (Indentation)  
✔ XML Minifying  
✔ XML → JSON Conversion  
✔ Custom Compression & Decompression  
✔ Full CLI Mode  
✔ Full GUI Mode  

---

### **Level 2 — Advanced Features**
✔ Build Graph Representation of Users & Followers  
✔ Visual Graph Export (`.jpg`)  
✔ Network Analysis:
- Most Influential User  
- Most Active User  
- Mutual Connections  
- Suggested Users (followers-of-followers)

✔ Post Search by **word** or **topic**  

---

## 🖥️ Command Line Usage

### General Syntax
```bash
xml_editor <operation> -i <input_file> [options]
```
### Operations

#### ✔ Verify XML
xml_editor verify -i input.xml
xml_editor verify -i input.xml -f -o fixed.xml


#### ✔ Format XML
```bash
xml_editor format -i input.xml -o formatted.xml
```

#### ✔ Convert XML → JSON
```bash
xml_editor json -i input.xml -o output.json
```

#### ✔ Minify XML
```bash
xml_editor mini -i input.xml -o minified.xml
```

#### ✔ Compress / Decompress
```bash
xml_editor compress -i input.xml -o data.comp
xml_editor decompress -i data.comp -o output.xml
```

#### ✔ Draw Graph
```bash
xml_editor draw -i input.xml -o graph.jpg
```

#### ✔ Network Analysis
```bash
xml_editor most_active -i input.xml
xml_editor most_influencer -i input.xml
xml_editor mutual -i input.xml -ids 1,2,3
xml_editor suggest -i input.xml -id 1
```

#### ✔ Post Search
```bash
xml_editor search -w keyword -i input.xml
xml_editor search -t topic -i input.xml
```


---

## 🖼️ GUI Features  
The GUI allows users to:

- Browse & load XML files  
- View file contents in a read-only text area  
- Run all operations via buttons  
- Display results live  
- Save processed files  
(From page 19) :contentReference[oaicite:5]{index=5}

### GUI Placeholder Preview  
_Add your actual screenshots later:_


---

## 🧱 Internal Architecture

### Data Structures Used  
- `vector` for dynamic storage  
- `stack` for tag matching  
- `queue / priority_queue` where needed  
- Graph stored using **adjacency list**  
- Custom structures for posts & users  

### Custom Implementations  
All required complex DS (graphs, custom compression) are implemented from scratch.

---

## 📁 Project Structure

```
project_root/

```

---

## 👥 Team Members  

|          Name          | ID      | Role   |
|------------------------|---------|--------|
| Saeed Bayoumy Nabawy   | 2200950 | Team A |
| Marcelino Maged Khalil | 2200909 | Team E |
| Mostafa Ahmed          | 2201092 | Team A |
| Mohamed Hamdy Youssif  | 2200416 | Team D |
| Abdulrahman Ibrahem    | 2200960 | Team B |
| Mohamed Ali Mohamed    | 2200942 | Team B |
| Zeyad Abdallah Shaban  | 2200932 | Team E |
| Member 8               |         |        |

---

## 👥 Team Roles
|          team          |              Role              |
|------------------------|--------------------------------|
| Team A                 |Reading XML + consistency check |
| Team B                 |XML formatting + minifying      |
| Team C                 |XML → JSON converter            |
| Team D                 |Compression + Decompression     |
| Team E                 |GUI + integration               |
