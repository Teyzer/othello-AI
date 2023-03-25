import itertools
import numpy as np

X = np.random.randn(3, 1)
W1 = np.random.randn(3, 3)
b1 = np.random.randn(3, 1)
W2 = np.random.randn(3, 3)
b2 = np.random.randn(3, 1)
W3 = np.random.randn(1, 3)
b3 = np.random.randn(1, 1)

Z1 = W1.dot(X) + b1


' '.join([' '.join([str(i) for i in row]) for row in Z1])



