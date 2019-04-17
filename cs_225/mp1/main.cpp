
#include "rgbapixel.h"
#include "png.h"

using namespace std; 

int main() {
	PNG in_image("in.png"); 
	
	int width = in_image.width(); 
	int height = in_image.height(); 
	PNG out_image(width, height); 
	int x, y; 
	for(y=0; y<height; y++){
		for(x=0; x<width; x++){
			out_image(width-x-1, height-y-1)->red = in_image(x, y)->red; 
			out_image(width-x-1, height-y-1)->green = in_image(x, y)->green; 
			out_image(width-x-1, height-y-1)->blue = in_image(x, y)->blue; 
			out_image(width-x-1, height-y-1)->alpha = in_image(x, y)->alpha; 
		}
	}
	out_image.writeToFile("out.png"); 
	return 0; 
}

	
