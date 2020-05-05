import subprocess
import os

my_env = os.environ.copy()

command = "numactl --cpunodebind=0 --preferred=0 ./smooth ../data.csv"

for n in range(5):
  print(f"{n}")
  subprocess.run(command+f">>time_serial.csv", env=my_env, shell=True, check=True)
