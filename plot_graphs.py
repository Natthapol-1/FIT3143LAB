import matplotlib.pyplot as plt
import os

artifact_dir = r"C:\Users\Natthapol\.gemini\antigravity-ide\brain\bc341ad1-81ef-4fd9-bdbc-c8db69aa2bad"
output_dir = os.path.join(artifact_dir, 'graphs')
os.makedirs(output_dir, exist_ok=True)

# Data
n_values = [10_000_000, 20_000_000, 30_000_000, 40_000_000]
t1_n = [2.975282, 7.907641, 13.903484, 20.987612]
t2_n = [1.477683, 4.030564, 5.695076, 8.852203]
t3_n = [1.042588, 1.662556, 3.418828, 4.638608]

threads = [1, 2, 3, 4]
t1_threads = [7.907641, 7.907641, 7.907641, 7.907641] # Serial time for N=20m
t2_threads = [9.890393, 5.882095, 5.134729, 4.642763]

# Speedup calculations
speedup_t2_n = [t1 / t2 for t1, t2 in zip(t1_n, t2_n)]
speedup_t3_n = [t1 / t3 for t1, t3 in zip(t1_n, t3_n)]
speedup_t2_threads = [t1_threads[0] / t2 for t2 in t2_threads]

def save_plot(filename):
    plt.savefig(os.path.join(output_dir, filename), bbox_inches='tight')
    plt.close()

# 1) Comparing run time of serial vs PThreads with increasing N
plt.figure()
plt.plot(n_values, t1_n, marker='o', label='Task 1 (Serial)')
plt.plot(n_values, t2_n, marker='s', label='Task 2 (POSIX Threads)')
plt.title("Run Time vs Size of n (Serial vs POSIX Threads)")
plt.xlabel("Size of n")
plt.ylabel("Execution Time (Seconds)")
plt.legend()
plt.grid(True)
save_plot('graph1.png')

# 2) Speedup of PThreads with increasing N
plt.figure()
plt.plot(n_values, speedup_t2_n, marker='o', color='orange')
plt.title("Speedup vs Size of n (POSIX Threads)")
plt.xlabel("Size of n")
plt.ylabel("Speedup (Serial / POSIX Threads)")
plt.grid(True)
save_plot('graph2.png')

# 3) Comparing run time of serial vs PThreads with increasing threads (N=20,000,000)
plt.figure()
plt.plot(threads, t1_threads, marker='o', label='Task 1 (Serial)')
plt.plot(threads, t2_threads, marker='s', label='Task 2 (POSIX Threads)')
plt.title("Run Time vs Number of Threads\n(N=20,000,000) (Serial vs POSIX Threads)")
plt.xlabel("Number of Threads")
plt.ylabel("Execution Time (Seconds)")
plt.xticks(threads)
plt.legend()
plt.grid(True)
save_plot('graph3.png')

# 4) Speedup of PThreads with increasing threads
plt.figure()
plt.plot(threads, speedup_t2_threads, marker='o', color='orange')
plt.title("Speedup vs Number of Threads\n(N=20,000,000) (POSIX Threads)")
plt.xlabel("Number of Threads")
plt.ylabel("Speedup (Serial / POSIX Threads)")
plt.xticks(threads)
plt.grid(True)
save_plot('graph4.png')

# 5) Comparing run time of serial vs OpenMP with increasing N
plt.figure()
plt.plot(n_values, t1_n, marker='o', label='Task 1 (Serial)')
plt.plot(n_values, t3_n, marker='^', label='Task 3 (OpenMP)')
plt.title("Run Time vs Size of n (Serial vs OpenMP)")
plt.xlabel("Size of n")
plt.ylabel("Execution Time (Seconds)")
plt.legend()
plt.grid(True)
save_plot('graph5.png')

# 6) Speedup of OpenMP with increasing N
plt.figure()
plt.plot(n_values, speedup_t3_n, marker='^', color='green')
plt.title("Speedup vs Size of n (OpenMP)")
plt.xlabel("Size of n")
plt.ylabel("Speedup (Serial / OpenMP)")
plt.grid(True)
save_plot('graph6.png')

# 7) Comparing run time of PThreads vs OpenMP with increasing N
plt.figure()
plt.plot(n_values, t2_n, marker='s', label='Task 2 (POSIX Threads)')
plt.plot(n_values, t3_n, marker='^', label='Task 3 (OpenMP)')
plt.title("Run Time vs Size of n (POSIX Threads vs OpenMP)")
plt.xlabel("Size of n")
plt.ylabel("Execution Time (Seconds)")
plt.legend()
plt.grid(True)
save_plot('graph7.png')

print("Graphs generated successfully.")
