# Anaglyph

Anaglyph 3D is the stereoscopic 3D effect achieved by means of encoding each eye's image using filters of different (usually chromatically opposite) colors, typically red and cyan

Two camera with distance capture stereoscopic images( Left and Right Image)

Difference between Left and Right Image is called Depth 

There are two algorithms involved. The first uses the original image and the depth map to produce a left and a right image. The second combines these images into a red-cyan anaglyph.

There are a couple ways to accomplish the first part. One is to take the original image and texture map it onto a fine mesh that lies flat in the XY plane. Then you tweak the Z values of each vertex in the mesh according to the corresponding value in the depth map. You've basically created a textured bas relief. You then use a 3D rendering algorithm to render the image from two vantage points that are offset horizontally by a small amount (essentially from the vantage point of a person's left and right eyes as they would view the bas relief).

There is probably a way to directly shift the pixels left and right which is a good fast approximation to what I described above.

Once you have the left and right images, you pass one through a cyan filter and one through a red filter. If you have RGB sources, that's as simple as taking the red channel from one image and combing it with the green and blue channels from the other image.

Anaglyphs work best with muted colors. If you have strong primaries, it won't look as good. You can use an algorithm to reduce the color saturation of the original image before you begin.





RGB-D images have become an essential element in fields like computer vision, robotics, and augmented reality. The "RGB" part refers to the traditional red, green, and blue color channels in standard images, while the "D" stands for depth, adding a new dimension of spatial data. This depth information enables applications to interpret the world in three dimensions, making RGB-D images a powerful tool for advanced applications.

In this article, we’ll explore what RGB-D images are, how they are captured, and their practical applications.

What are RGB-D Images?
RGB-D images combine conventional 2D images with depth information. In standard RGB images, each pixel contains color information based on red, green, and blue intensity. However, RGB-D images go further by also assigning a depth value to each pixel, allowing for a representation of how far each pixel is from the sensor.

This combination of color and depth enables the construction of 3D models, making it highly valuable for applications that require spatial awareness or real-world interaction.

Components of an RGB-D Image
RGB Channels: The traditional color image composed of red, green, and blue pixels.
Depth Channel: A single channel representing the distance from the camera or sensor to the object in the scene. This is often represented as grayscale, where darker values indicate objects closer to the camera, and lighter values indicate farther objects.
How RGB-D Images are Captured
RGB-D images are captured using specialized devices that can simultaneously record color and depth. These devices often utilize two different methods for capturing depth information:

1. Structured Light
Structured light sensors project an infrared pattern onto the scene and measure its deformation to estimate depth. One well-known example is Microsoft's Kinect sensor. The pattern is distorted depending on how far an object is from the sensor, allowing the device to calculate depth.

2. Time-of-Flight (ToF)
Time-of-Flight cameras calculate the time it takes for light (typically infrared) to bounce back from an object and return to the sensor. The distance is determined based on the speed of light and the time delay.

3. Stereo Vision
Stereo vision involves using two cameras placed slightly apart from each other. By comparing the two images, the system can compute depth based on the differences between the two perspectives.

Devices Capturing RGB-D Images
Several devices can capture RGB-D images, including:

Microsoft Kinect: One of the most popular consumer-grade sensors.
Intel RealSense: A family of depth cameras commonly used for robotic and AR/VR applications.
Apple LiDAR Scanner: Used in newer iPhones and iPads to capture depth information for AR experiences.
RGB-D Image Processing: Simulating Depth Maps and Visualizing RGB Images in Python
In this section, we load an RGB image and simulate a depth map using Python, OpenCV, and Matplotlib.

Image Loading: The image is loaded directly from a URL using urllib and converted into OpenCV’s format.
Simulating Depth: A synthetic depth map is generated, simulating depth values based on the distance of each pixel from the top-left corner.
Visualization: The RGB image and the corresponding depth map are displayed side by side using matplotlib for comparison.
Purpose: Provides foundational code for experimenting with RGB-D image processing and visualization techniques in Python.
















## The Common Types of 3D Glasses

There are three types of 3D glasses:

Anaglyph 3D glasses
Polarized 3D glasses
Shutter 3D glasses
Working of all types of 3D glasses is given below;

Anaglyph 3D glasses:

These are the most common and widely used 3D glasses. These glasses are created in a manner where one lens is red, and the other is cyan. These lenses color-filter the layered image we are looking at to create the images we see. Our brain perceives an image as 3D because one lens completely removes all the red in a snap.

However, the other completely removes all of the cyan. Usually, the image we are viewing is the same image projected from two different angles or two completely different shots superimposed.

anaglyph 3d-glasses
Polarized 3D glasses:

They block light from reaching our eyes, but their golden-brown color prevents red and blue colors from doing so. The image displayed on the screen also plays a part. The projected image is created by superimposing two images through an orthogonal polarizing filter on the same screen in addition to the polarization on the glasses. The glasses' identical type of filter enables each eye to see a separate image on the screen.

polarized 3d-glasses
Shutter 3D glasses: ( no more this solution is used, it is very expensive)

Shutter glasses are thought to be the most sophisticated 3D glasses currently on the market. Shutter glasses use active 3D instead of the passive 3D used by the other two forms of 3D glasses. They do not use filtered images or colors to produce a three-dimensional illusion.

Shutter glasses operate through LCD screen technology that alternately darkens the left and right lenses. Unless we are paying great attention, we won't notice the lens darkening because it happens so quickly. Shutter glasses are more expensive than conventional 3D glasses and are frequently battery- or even USB-powered. The price of these glasses significantly affects image quality.

shutter 3d-glasses


## Reference

Depth estimation
https://medium.com/artificialis/getting-started-with-depth-estimation-using-midas-and-python-d0119bfe1159

RGBD 
https://www.geeksforgeeks.org/rgb-d-images-a-comprehensive-overview/


source code 
https://bitbucket.org/stereophotoview/stereophotoview/src/master/


ffmpeg option 
https://trac.ffmpeg.org/wiki/Stereoscopic

3D Glass Selection
https://exmplayer.sourceforge.net/3d.html

