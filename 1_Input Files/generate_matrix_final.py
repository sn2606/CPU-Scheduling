import numpy as np
import random


class Generate:
    def __init__(self, file_name):
        self.file_name = file_name

    def generate_one_priority(self):
        pid = np.arange(1, 21, 1)
        priority = np.random.randint(1, 21, 20)
        bt = np.random.randint(1, 100, 20)
        at = np.random.randint(0, 20, 20)
        inp = [pid, priority, bt, at]
        return inp

    def generate_one_not_priority(self):
        pid = np.arange(1, 21, 1)
        priority = np.random.randint(-1, 0, 20)
        bt = np.random.randint(1, 100, 20)
        at = np.random.randint(0, 20, 20)
        inp = [pid, priority, bt, at]
        return inp

    def priority_matrix(self, n):
        inp = []
        i = 0
        while i < n:
            x = Generate(self.file_name).generate_one_priority()
            for j in range(4):
                inp.append(x[j])
            i += 1

        inp = np.matrix(np.array(inp).transpose()) 

        with open(self.file_name, 'wb') as f:
            for line in inp:
                np.savetxt(f, line, fmt='%d')

    def non_priority_matrix(self, n):
        inp = []
        i = 0
        while i < n:
            x = Generate(self.file_name).generate_one_not_priority()
            for j in range(4):
                inp.append(x[j])
            i += 1

        inp = np.matrix(np.array(inp).transpose()) 

        with open(self.file_name, 'wb') as f:
            for line in inp:
                np.savetxt(f, line, fmt='%d')



Generate('final_priority.txt').priority_matrix(25)
Generate('final_not_priority.txt').non_priority_matrix(25)