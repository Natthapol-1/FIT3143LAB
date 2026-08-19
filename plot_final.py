import matplotlib.pyplot as plt
import os

artifact_dir = r"C:\Users\Natthapol\.gemini\antigravity-ide\brain\bc341ad1-81ef-4fd9-bdbc-c8db69aa2bad"
output_dir = os.path.join(artifact_dir, 'graphs')
os.makedirs(output_dir, exist_ok=True)

# Data for Size Scaling
n_values = [i * 1000000 for i in range(1, 31)]

t1_n = [0.157446, 0.332075, 0.556883, 0.857003, 1.146130, 1.519763, 1.816934, 2.231608, 2.638321, 2.993474, 3.386857, 3.902809, 4.354735, 4.837241, 5.332066, 5.755380, 6.385474, 6.841467, 7.670622, 7.911784, 8.627897, 9.174930, 9.563128, 10.587262, 10.807275, 11.582939, 12.284454, 12.623788, 13.403409, 13.931236]

t2_n = [0.112235, 0.161346, 0.381225, 0.344685, 0.561221, 0.413345, 0.782133, 1.227540, 0.885634, 2.005082, 0.940126, 2.494910, 0.911871, 0.970127, 1.009115, 1.423618, 1.512227, 3.398706, 2.437132, 2.708854, 3.564806, 4.230484, 3.944984, 3.617307, 4.029734, 4.107821, 5.636765, 5.467095, 4.271793, 5.356185]

t3_n = [0.089316, 0.132201, 0.222797, 0.479508, 0.739773, 0.386447, 0.738415, 0.511662, 0.671799, 1.270608, 2.054702, 0.913908, 0.805696, 0.914342, 2.469365, 2.058680, 1.079249, 3.386809, 2.524227, 3.584245, 4.117452, 5.039675, 3.349028, 2.833169, 3.633338, 4.630036, 4.408088, 4.147740, 3.509056, 4.294511]

# Data for Thread Scaling (N=20,000,000)
threads = [1, 2, 4, 8, 12, 16, 20, 24, 32]
# Using average of the two runs provided
run1 = [8.199497, 6.480621, 3.072233, 2.068951, 2.775189, 3.124570, 2.826929, 1.501718, 3.735865]
run2 = [9.151593, 5.960595, 3.887449, 1.940716, 4.280697, 2.453362, 1.921564, 1.911048, 1.859935]
t2_threads = [(r1 + r2) / 2 for r1, r2 in zip(run1, run2)]

t1_threads = [7.911784] * len(threads)
t3_threads = [3.584245] * len(threads)

# Speedup calculations
speedup_t2_n = [t1 / t2 for t1, t2 in zip(t1_n, t2_n)]
speedup_t3_n = [t1 / t3 for t1, t3 in zip(t1_n, t3_n)]
speedup_t2_threads = [t1_threads[0] / t2 for t2 in t2_threads]

def save_plot(filename):
    plt.savefig(os.path.join(output_dir, filename), bbox_inches='tight')
    plt.close()

# 1) Comparing run time of serial vs PThreads with increasing N
plt.figure()
plt.plot(n_values, t1_n, marker='.', label='Task 1 (Serial)')
plt.plot(n_values, t2_n, marker='.', label='Task 2 (POSIX Threads)')
plt.title("Run Time vs Size of n (Serial vs POSIX Threads)")
plt.xlabel("Size of n")
plt.ylabel("Execution Time (Seconds)")
plt.legend()
plt.grid(True)
save_plot('graph1.png')

# 2) Speedup of PThreads with increasing N
plt.figure()
plt.plot(n_values, speedup_t2_n, marker='.', color='orange')
plt.title("Speedup vs Size of n (POSIX Threads)")
plt.xlabel("Size of n")
plt.ylabel("Speedup (Serial / POSIX Threads)")
plt.grid(True)
save_plot('graph2.png')

# 3) Comparing run time of serial vs PThreads with increasing threads
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
plt.plot(n_values, t1_n, marker='.', label='Task 1 (Serial)')
plt.plot(n_values, t3_n, marker='.', label='Task 3 (OpenMP)')
plt.title("Run Time vs Size of n (Serial vs OpenMP)")
plt.xlabel("Size of n")
plt.ylabel("Execution Time (Seconds)")
plt.legend()
plt.grid(True)
save_plot('graph5.png')

# 6) Speedup of OpenMP with increasing N
plt.figure()
plt.plot(n_values, speedup_t3_n, marker='.', color='green')
plt.title("Speedup vs Size of n (OpenMP)")
plt.xlabel("Size of n")
plt.ylabel("Speedup (Serial / OpenMP)")
plt.grid(True)
save_plot('graph6.png')

# 7) Comparing run time of PThreads vs OpenMP with increasing N
plt.figure()
plt.plot(n_values, t2_n, marker='.', label='Task 2 (POSIX Threads)')
plt.plot(n_values, t3_n, marker='.', label='Task 3 (OpenMP)')
plt.title("Run Time vs Size of n (POSIX Threads vs OpenMP)")
plt.xlabel("Size of n")
plt.ylabel("Execution Time (Seconds)")
plt.legend()
plt.grid(True)
save_plot('graph7.png')

# 8) Comparing run time of PThreads vs OpenMP with increasing threads
plt.figure()
plt.plot(threads, t2_threads, marker='s', label='Task 2 (POSIX Threads)')
plt.plot(threads, t3_threads, marker='^', label='Task 3 (OpenMP - Fixed)')
plt.title("Run Time vs Number of Threads\n(N=20,000,000) (POSIX Threads vs OpenMP)")
plt.xlabel("Number of Threads (Applies to Task 2)")
plt.ylabel("Execution Time (Seconds)")
plt.xticks(threads)
plt.legend()
plt.grid(True)
save_plot('graph8.png')

print("Final 30-point graphs generated successfully.")
