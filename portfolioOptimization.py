import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
x = np.linspace(-2, 2, 8)
y = np.linspace(-2, 2, 8)
z = np.linspace(-2, 2, 8)
X, Y, Z = np.meshgrid(x, y, z)
U = Y*Z*np.exp(X*Y)
V = X*Z*np.exp(X*Y)
W = np.exp(X*Y)
fig = plt.figure(figsize=(10,8))
ax = fig.add_subplot(111, projection='3d')
ax.quiver(X, Y, Z, U, V, W, length=0.3, normalize=True, color='blue')
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')
ax.set_title('Gradient Vector Field of f(x,y,z)')
plt.show()