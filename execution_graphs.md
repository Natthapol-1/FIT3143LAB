# Execution Performance Graphs

Here are the requested graphs comparing the performance of the Serial, POSIX Threads (Task 2), and OpenMP (Task 3) implementations based on the data you provided.

## Task 2: POSIX Threads

### 1) Run time vs Size of n (Serial vs POSIX Threads)
![Run time vs Size of n](C:/Users/Natthapol/.gemini/antigravity-ide/brain/bc341ad1-81ef-4fd9-bdbc-c8db69aa2bad/graphs/graph1.png)

### 2) Speedup vs Size of n (POSIX Threads)
![Speedup vs Size of n](C:/Users/Natthapol/.gemini/antigravity-ide/brain/bc341ad1-81ef-4fd9-bdbc-c8db69aa2bad/graphs/graph2.png)

### 3) Run time vs Number of Threads (Serial vs POSIX Threads)
*Note: Evaluated at N = 20,000,000.*
![Run time vs Number of Threads](C:/Users/Natthapol/.gemini/antigravity-ide/brain/bc341ad1-81ef-4fd9-bdbc-c8db69aa2bad/graphs/graph3.png)

### 4) Speedup vs Number of Threads (POSIX Threads)
*Note: Evaluated at N = 20,000,000.*
![Speedup vs Number of Threads](C:/Users/Natthapol/.gemini/antigravity-ide/brain/bc341ad1-81ef-4fd9-bdbc-c8db69aa2bad/graphs/graph4.png)

---

## Task 3: OpenMP

### 5) Run time vs Size of n (Serial vs OpenMP)
![Run time vs Size of n](C:/Users/Natthapol/.gemini/antigravity-ide/brain/bc341ad1-81ef-4fd9-bdbc-c8db69aa2bad/graphs/graph5.png)

### 6) Speedup vs Size of n (OpenMP)
![Speedup vs Size of n](C:/Users/Natthapol/.gemini/antigravity-ide/brain/bc341ad1-81ef-4fd9-bdbc-c8db69aa2bad/graphs/graph6.png)

---

## Task 2 vs Task 3 Comparison

### 7) Run time vs Size of n (POSIX Threads vs OpenMP)
![Run time vs Size of n](C:/Users/Natthapol/.gemini/antigravity-ide/brain/bc341ad1-81ef-4fd9-bdbc-c8db69aa2bad/graphs/graph7.png)

### 8) Run time vs Number of Threads (POSIX Threads vs OpenMP)
*Note: Evaluated at N = 20,000,000. OpenMP threads are fixed, resulting in a constant execution time, while POSIX threads vary.*
![Run time vs Number of Threads (POSIX vs OpenMP)](C:/Users/Natthapol/.gemini/antigravity-ide/brain/bc341ad1-81ef-4fd9-bdbc-c8db69aa2bad/graphs/graph8.png)
