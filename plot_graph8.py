import matplotlib.pyplot as plt
import os

artifact_dir = r"C:\Users\Natthapol\.gemini\antigravity-ide\brain\bc341ad1-81ef-4fd9-bdbc-c8db69aa2bad"
output_dir = os.path.join(artifact_dir, 'graphs')
os.makedirs(output_dir, exist_ok=True)

threads = [1, 2, 3, 4]
t2_threads = [9.890393, 5.882095, 5.134729, 4.642763]
t3_threads = [1.662556] * 4

plt.figure()
plt.plot(threads, t2_threads, marker='s', label='Task 2 (POSIX Threads)')
plt.plot(threads, t3_threads, marker='^', label='Task 3 (OpenMP - Fixed)')
plt.title("Run Time vs Number of Threads\n(N=20,000,000) (POSIX Threads vs OpenMP)")
plt.xlabel("Number of Threads (Applies to Task 2)")
plt.ylabel("Execution Time (Seconds)")
plt.xticks(threads)
plt.legend()
plt.grid(True)
plt.savefig(os.path.join(output_dir, 'graph8.png'), bbox_inches='tight')
plt.close()

print("Graph 8 generated successfully.")
