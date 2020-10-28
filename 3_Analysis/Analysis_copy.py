
# To add a new cell, type '# %%'
# To add a new markdown cell, type '# %% [markdown]'
# %%
from IPython import get_ipython

# %%
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
get_ipython().run_line_magic('matplotlib', 'inline')
plt.style.use('seaborn-whitegrid')


# %%
x = list(np.arange(1, 26))
ind = ['FCFS', 'PS_NP', 'PS_P', 'SJF_NP', 'SJF_P', 'RR', 'MLQ', 'MLFQ']

avg_wt = pd.read_csv("op_avg_wt.csv", header = None, names = x, index_col = False)
avg_tat = pd.read_csv("op_avg_tat.csv", header = None, names = x, index_col = False)
throughput = pd.read_csv("op_throughput.csv", header = None, names = x, index_col = False)

avg_wt


# %%
avg_tat


# %%
throughput


# %%
fig_wt = plt.figure(figsize = (20, 10))
axes_wt = fig_wt.add_axes([0, 0, 1, 1])

for i in range(8):
    axes_wt.plot(x, avg_wt.loc[i], label = ind[i])
                           
axes_wt.set_title('AVERAGE WAIT TIME')
axes_wt.set_xlabel('INPUT NUMBER')
axes_wt.set_ylabel('AVG WT')
axes_wt.set_xticks(x)

plt.legend()

fig_wt.savefig("C:\\Users\\S K Nayak\\Documents\\CSE2005 Project\\OS Project\\Average Waiting Time.png")


# %%
fig_tat = plt.figure(figsize = (20, 10))
axes_tat = fig_tat.add_axes([0, 0, 1, 1])

for i in range(8):
    axes_tat.plot(x, avg_tat.loc[i], label = ind[i])

axes_tat.set_title('AVERAGE TURNAROUND TIME')
axes_tat.set_xlabel('INPUT NUMBER')
axes_tat.set_ylabel('AVG TAT')
axes_tat.set_xticks(x)

plt.legend()

fig_wt.savefig("C:\\Users\\S K Nayak\\Documents\\CSE2005 Project\\OS Project\\Average Turnaround Time.png")


# %%
fig_tpt = plt.figure(figsize = (20, 10))
axes_tpt = fig_tpt.add_axes([0, 0, 1, 1])

for i in range(8):
    axes_tpt.plot(x, throughput.loc[i], label = ind[i])

axes_tpt.set_title('THROUGHPUT')
axes_tpt.set_xlabel('INPUT NUMBER')
axes_tpt.set_ylabel('THROUGHPUT')
axes_tpt.set_xticks(x)

plt.legend()

fig_wt.savefig("C:\\Users\\S K Nayak\\Documents\\CSE2005 Project\\OS Project\\Throughput.png")


# %%
avg_wt_info = avg_wt.describe()
avg_wt_min = avg_wt.idxmin()

for i, j in enumerate(avg_wt_min):
    avg_wt_min[i + 1] = (lambda j :  ind[j])(j)
    
avg_wt_min


# %%
avg_tat_info = avg_tat.describe()
avg_tat_min = avg_tat.idxmin()

for i, j in enumerate(avg_tat_min):
    avg_tat_min[i + 1] = (lambda j :  ind[j])(j)
    
avg_tat_min


# %%
tpt_info = throughput.describe()
tpt_max = throughput.idxmax()

for i, j in enumerate(tpt_max):
    tpt_max[i + 1] = (lambda j :  ind[j])(j)
    
tpt_max


# %%
sns.countplot(x = avg_wt_min)


# %%
sns.countplot(x = avg_tat_min)


# %%
sns.countplot(x = tpt_max)


# %%



