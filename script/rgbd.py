# Step 1: Install necessary packages (you can skip this in Colab as these are pre-installed)
# !pip install opencv-python-headless https://www.geeksforgeeks.org/rgb-d-images-a-comprehensive-overview/
# !pip install matplotlib

import numpy as np
import matplotlib.pyplot as plt
import cv2
import urllib.request

# Step 2: Load an RGB image from a URL using urllib
image_url = 'https://upload.wikimedia.org/wikipedia/commons/4/47/PNG_transparency_demonstration_1.png'
resp = urllib.request.urlopen(image_url)
image = np.asarray(bytearray(resp.read()), dtype="uint8")
image = cv2.imdecode(image, cv2.IMREAD_COLOR)

# Convert the image from BGR (OpenCV format) to RGB format
rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

# Step 3: Simulate a depth map
# For demonstration, let's create a synthetic depth map where
# objects further away from the top-left corner have higher depth values.
height, width, _ = rgb_image.shape
depth_map = np.zeros((height, width))

for i in range(height):
    for j in range(width):
        depth_map[i, j] = np.sqrt(i**2 + j**2)

# Normalize depth values to the range [0, 255]
depth_map = (255 * (depth_map / np.max(depth_map))).astype(np.uint8)

# Step 4: Display the RGB image and depth map side by side

fig, ax = plt.subplots(1, 2, figsize=(12, 6))

# Show the RGB image
ax[0].imshow(rgb_image)
ax[0].set_title('RGB Image')
ax[0].axis('off')

# Show the depth map
cax = ax[1].imshow(depth_map, cmap='gray')
ax[1].set_title('Depth Map')
ax[1].axis('off')

# Add a colorbar to visualize depth intensity
fig.colorbar(cax, ax=ax[1], shrink=0.6)

plt.show()