/*main.cpp
  Author:	Rodas T. Gebreslassie

  Purpose:	Alter the color of an original image by using the average color of connected 
			pixels in the same group. The group is created by using color as a critaria. 
			Seed pixels are first identified in raster order, then the neighboring pixels 
			that have close enough color to the seed pixel will get grouped. 
*/

#include <iostream>
#include "ImageLib.h"
#include "PixelContainer.h"

using namespace std;

void paintAverage(image& in, PixelContainer& container);
void recursiveAddAndChangeColor(image& image, PixelContainer& container, int row, int col, pixel& seed);




//main():
//Precodnition:		test4.GIF exists and is a valid GIF file.
//Postconditions:	output.GIF will be written and it is a color modfied version of the 
//					origional image.
int main() {

	//reads test4.GIF from file
	image cat = ReadGIF("test4.GIF");
	

	//New image object is created that has equal dimensions to the origional image. 
	image copyCat = CreateImage(cat.rows, cat.cols);


	//PixelContainer object container is created, it is used to store one group of 
	//pixels at a time.
	PixelContainer container;

	//finalContainer object is used to store all the pixels of an image.
	PixelContainer finalContainer;


	//SigmentNumber - is a counter that gets updated each time a seed pixel is found. 
	int segmentNumber = 0;


	for (int row = 0; row < cat.rows; row++) {
		for (int col = 0; col < cat.cols; col++) {

			//As the nested loop executes, if a pixel is not black, we will set it as a
			//seed pixel. Initially no pixel is black in the image.
			if (cat.pixels[row][col].red != 0 && cat.pixels[row][col].blue != 0 &&
				cat.pixels[row][col].green != 0) {

				segmentNumber++;
				pixel seedPixel = cat.pixels[row][col];

				//Once the seed pixel is found, we will look for other connected pixels 
				//recursively that that have close enough color with the seed pixel.
				recursiveAddAndChangeColor(cat, container, row, col, seedPixel);

				cout << "The seed Pixel is at row: " << row << "  and col: " << col << endl;


				//When the recursive function finishes its execution, we will get all the 
				//connected pixels that have close enough color with the seed pixel. Those 
				//pixels are added to the container, now we will  merge the nodes in the 
				//container to the finalContainer.
				finalContainer.merge(container);
				
				//container.averagePixel() compute the average color value for each colors and 
				//replace each node with the average color. 
				container.averagePixel();

				//will paint an image object with the colors that are contained in the 
				//PixelContainer. 
				paintAverage(copyCat, container);
				


			}



		}
	}
	cout << "The number of Segements are: " << segmentNumber << endl;
	
	WriteGIF("Output.GIF", copyCat);
	container.~PixelContainer();
	finalContainer.~PixelContainer();
}

//recursiveAddAndChangeColor(image&, PixelContainer& , int, int, pixel&):
//					Is a recursive function that adds pixel data (row, col, red, blue, green)
//					to the container after checking if a given pixel color is close enough to 
//					the seed pixel.
//
//Precondition:		When the seed pixel is updated, an empty pixel container should be sent 
//					as a parameter so averging the color of pixels in the container won't create
//					mistake.
//Postconditions:	nodes will be will get added to the pixel container.
void recursiveAddAndChangeColor(image& image, PixelContainer& container, int row, int col, pixel& seed) {


	//Check if col and row are within the image boundary.
	if (row >= image.rows || col >= image.cols || row < 0 || col < 0) {

		return;
	}


	//Check if the pixel is black, Return if it is black
	if (image.pixels[row][col].red == 0 && image.pixels[row][col].blue == 0 &&
		image.pixels[row][col].green == 0) {

		return;
	}

	//compare the pixel color value to that of the seed pixel. if the callculation gave a number 
	//greater than or equal to 100, return.
	pixel p = image.pixels[row][col];
	if (abs(seed.red - p.red) + abs(seed.blue - p.blue) + abs(seed.green - p.green) >= 100) {
		return;
	}

	//add pixel to the container.
	container.addPixel(row, col, image.pixels[row][col].red, image.pixels[row][col].blue, image.pixels[row][col].green);

	image.pixels[row][col].red = 0;
	image.pixels[row][col].blue = 0;
	image.pixels[row][col].green = 0;


	recursiveAddAndChangeColor(image, container, row + 1, col, seed);
	recursiveAddAndChangeColor(image, container, row - 1, col, seed);
	recursiveAddAndChangeColor(image, container, row, col + 1, seed);
	recursiveAddAndChangeColor(image, container, row, col - 1, seed);
	return;


}

//paintAverage(image&, PixelContainer&):	Takes an image and a PixelContainer as a parameter 
//											and paint an image with the color data contained in
//											PixelContainer's nodes.
// Precondition:							None
// Postconditions:							The pixels in the image will get changed.
void paintAverage(image& in, PixelContainer& container) {
	

	while (container.isEmpty() == false) {
		int row;
		int col;
		int red;
		int blue;
		int green;

		container.getFirst(row, col, red, blue, green);
		in.pixels[row][col].red = red;
		in.pixels[row][col].blue = blue;
		in.pixels[row][col].green = green;
	}
}