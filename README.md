//Muntean Vlad-Andrei 315CA

	For this project, the photo editor uses grayscale (pgm) and
colored (ppm) pixel map-type images. Every image is seen as a matrix of
characters and is stored within a structure called "photo", along with the
magic number ("type"), number of columns ("col"), number of lines ("lin"),
the maximum value of the pixels ("max") and a structure that holds the 4
coordinates of the selection (x1, x2, y1, y2). In the case of colored images,
we use matrix of type "struct rgb", containing three red, green and blue values
for each pixel. To use as less memory as possible, every pixel will be stored
in a variable of type  "unsigned char".


	The choice of commands is controlled using a switch statement, to
assure a better reading. The function "choose_command()" returns a letter
for every type of command read, to assure the functionality of the switch
statement.


	Loading the needed image is done with the "load_file()" command. Upon
opening the file, we will erase the old data from the previous image and print
an error message in case the file doesn't exist. For both ascii and binary
files, the magic number, size and maximum value are read as ascii, with the
"clean_file()" function inbetween each read to ignore comments (this function
reads a line and checks for the first character if it's a '#'. Once a
non-commented line is found, the file's pointer returns to the first position
for a normal re-read). Binary files are read using "fread()", every pixel being
of type char, saved with no spaces or newlines. For ascii, the pixels are read
with "fprintf()" as integers. After loading the image into our program's
memory, the file is closed.


	For the SAVE command, the file will be open with the write
specifier("w"), and we will read the new file name with "fgets()", in order to
save all written elements of the line and we will erase the newline character
to ease work. In case we want to save the file in the ascii format, the last
6 characters of the line will be " ascii" (I have included a space at the start
to avoid problems with files named as 'somethingascii' that, maybe, we would
like to save as binary), otherwise, the file will be saved in binary form.
We will check the original magic number and modify it according to the new
format we need ('P2' grayscale format saved as ascii becomes 'P5', same for
color and vice-versa). For voth formats we will write magic number, columns,
lines and maximum value as ascii, but for ascii, the matrix will be written
as integers with spaces inbetween, whereas binary
will have unspaced characters.


	The SELECT command will modify the "selected" structure with the read
coordinates. In case the next word read is ALL, the coordinates will be
reset, otherwise, we will read the coordinates, make sure all of them are
integers and 4 in number. After that, we will compare the coordinates to not
exit the image's bounds and swap the coordinates if they are in read in
decreasing order (to better ease operations). In case the selection fails, the
old coordinates remain.


	Cropping the image will create a new matrix, with the same magic
number and maximum value, but its width and height will be determined by the
selected coordinates. In this matrix, all the pixels from the selection are
moved, the old matrix is freed and replaced in memory by the cropped matrix. 


	The APPLY command has four four possible effects that I have indexed as
such: 1. EDGE, 2. SHARPEN, 3. BLUR and 4. GAUSSIAN_BLUR. The effects are
applied through a method of convolving every grid of 3*3 from the selection
with a specific kernel, therefore the operation is the same for every effect,
the only difference is the specific kernel, which we initialize in a 3*3 matrix
using the "init_kernel()" function. After that, we will use a new matrix
("new_color") that will keep the pixels from the selection after applying the
chosen effect. It is important to keep in mind that the selection must be
checked for margins with no neighbouring pixels, in that case the pixels of
the specific side will be unaltered (and the pixels from margins that do not
touch the bounds will be convovled with pixels outside the selection). Every
new pixel will be rounded, in case there is a division made (effect 3 and 4),
and then clamped(in the case of edge detection, values may surpass 255 or be
negativ). After the matrix is completed, the elements are copied back into the
original matrix. (Note: the APPLY command only works for colored images, in
case the existing image is grayscale, the specific message is shown and the
function is stopped).


	HISTOGRAM is available only for grayscale images. It reads two inputs
(length in stars for each bin and number of bins). In case the number of inputs
is different from 2, the command is deemed invalid and the function stops.
The value of each bin is stored in an array ("histo_map"). We will use a "step"
value to see the numerical value between each bin. To determine the bin in
which our pixel value is, it's enough to divide the value to the step, later
incrementing the value in the array. For a visual representation of our
histogram, we will calculate tha maximum value from the array of bins and
divide every bin with it, the result later being multiplied by the number of
stars chosen.


	EQUALIZE is another command that works only on grayscale images. For
this, we can reuse the "calc_histo()" function, used previously for the
histogram, to calculate an array for every existing value (same as saying we
use 256 bins). The array is used to calculate the sum ("histo_sum") needed for
the equalization formula. Every pixel from our array is replaced by the sum
multiplied by 255 (value that coincides with the maximum value of our matrix)
and then divided by the area of the image (lines * columns). The result is
rounded and clamped.


	The  ROTATE command works on both types of images. All selections
smaller than the whole image must be square. In case of a non-square image
it can be perfectly rotated, as long as it's fully selected. All supported
angles are multiples of 90 degrees greater from the [-360, 360] interval.
Values such as (-90, 270), (-180, 180), (0, 360), etc. coincide, therefore
we can implement only four cases. The default rotation of this function is
to the lef (270 degrees), a 180 degree rotation is achieved by rotating the
image twice to 270 degrees and a 90 degree rotation is a triple rotation to
270 degrees. A 0 or 360 degree rotation will keep the image unaltered. To
succesfully rotate a selection, we will allocate a new matrix ("rot_gray" or
"rot_color", depending on the image type) that will copy the columns of the
original selection as rows, the last column becoming the first line in the
new matrix and so on. In case we have a square selection, it's enough to copy
the elements of the new matrix into the original selection, but otherwise, we
need to invert the values of the rows and columns of our photo, free the old
matrix and copy the new one in our structure's memory. The helper matrix is
then freed.


	EXIT will free all resources and end the program. If no image is
loaded, a specific message will be printed before exit.


	An invalid command will trigger a message and the program will catch
any residual information written on that line to avoid interpreting them as
individual commands.


	Every command is dependent of the "check" variable. If its value is 1,
it shows that an image is loaded and permits a normal run of our commands.
This variable is given the positive value only after a succesful load and is
given 0 when a load crashes.