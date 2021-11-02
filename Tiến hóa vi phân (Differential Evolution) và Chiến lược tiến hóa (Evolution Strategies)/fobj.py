import numpy as np
import math

# Sphere
def Sphere_f(x):
    f = 0
    for i in x:
        f += i**2
    return f

# Rastrigin
def Rastrigin_f(x):
  f = 10 * len(x)
  for i in x:
    f += i**2 - 10 * np.cos(2 * np.pi * i)
  return f

# Rosenbrock
def Rosenbrock_f(x):
    f = 0
    for i in range(len(x) - 1):
        f += (100*((x[i + 1] - x[i]**2)**2) + (1 -x[i])**2)
    return f

# Griewank
def Griewank_f(x):
    f1 = 0
    for i in x:
        f1 += (i*i)/4000
    f2 = 1
    for i in range(len(x)):
        f2 *= np.cos(x[i]/math.sqrt(i+1))
    return f1 - f2 + 1

# Ackley
def Ackley_f(x):
    a = 20
    b = 0.2
    c = 2 * np.pi
    f1 = 0
    f2 = 0
    for i in range(len(x)):
        f1 += x[i] ** 2
        f2 += np.cos(c * x[i])
    p1 = -a * np.exp(-b * np.sqrt(f1 / len(x)))
    p2 = -np.exp(f2 / len(x))

    return p1 + p2 + a + np.exp(1)