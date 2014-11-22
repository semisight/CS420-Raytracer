Assignment #3: Ray tracing

FULL NAME: Alexander Bogart


MANDATORY FEATURES
------------------

<Under "Status" please indicate whether it has been implemented and is
functioning correctly.  If not, please explain the current status.>

Feature:                                 Status: finish? (yes/no)
-------------------------------------    -------------------------
1) Ray tracing triangles                  yes

2) Ray tracing sphere                     yes

3) Triangle Phong Shading                 yes

4) Sphere Phong Shading                   yes

5) Shadows rays                           yes

6) Still images                           yes
   
7) Extra Credit (up to 20 points)

Antialiasing -- 3 sampling modes: X1 (standard, one sample per pixel), SGSSAA
(square grid supersampling, 4 samples per pixel), and RGSSAA (rotated grid
supersampling, 4 samples per pixel). The sampling type can be set in
assign3.cpp at the top of the file (the constant SAMPLING). Square grid is 4
pixels offset as a square around each logical pixel. Rotated grid takes those
sample points and rotates by 30 degrees. To create the final image, these 4
points are averaged for the final output point.

Reflective raytracing -- Can be enabled by setting the NUM_REFLECTIONS constant
in assign3.cpp to a non-zero value. This number indicates the maximum number of
reflection rays that the program will cast until it stops.

