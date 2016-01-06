from collections import defaultdict
import sys

import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
from scipy import stats

with open(sys.argv[1]) as fp:
    data = fp.readlines()

temp_lists = defaultdict(list)

for line in data:
    if line.startswith('#'):
        continue

    line = line.strip()

    node_id, temp = line.split(',')

    temp_lists[node_id].append(float(temp))


caption = """\
    Time between samples ~ 5 seconds.
    Sample 0 was recored on approx. Nov 1st, 2015.
"""
fig = plt.figure(figsize=(20, 10), dpi=300)
ax1 = fig.add_subplot(111)
plt.title('Temperature v. Sample ID (ie. Time)')
plt.ylabel('Temperature (degrees Celsius)')
plt.xlabel('Sample ID')
ax1.set_xlim((0, 1050000))
for node_id, temps in temp_lists.items():
    ax1.plot(range(len(temps)), temps, label=node_id)
fig.text(0.125, 0.1, caption)
plt.legend()
fig.savefig('results.png', dpi=300)

scores = []
for node_id, temps in temp_lists.items():
    scores.append((sum(temps) / len(temps), node_id))

scores = sorted(scores)
for i, (mean, node_id) in enumerate(scores):
    print('Mean ({}):'.format(node_id), mean)
    node_1_id = node_id
    a = temp_lists[node_1_id]
    print('Standard Deviation ({}):'.format(node_id), stats.tstd(a))
    if i:
        node_2_id = scores[i - 1][1]
        b = temp_lists[node_2_id]
        print(node_1_id, 'vs.', node_2_id, end=': ')
        print(stats.ttest_ind(a, b, equal_var=False))
        diffs = list(map(lambda x: x[0] - x[1], zip(a, b)))
        print(sum(diffs) / len(diffs))

