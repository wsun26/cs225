#include "image.h"

void Image::flipleft(){
	int temp;  
	for(size_t y = 0; y < (*this).height(); y++){
		for(size_t x = 0; x < (*this).width()/2; x++){
			temp = (*this)(x, y)->red; 
			(*this)(x, y)->red = (*this)((*this).width()-x-1, y)->red; 
			(*this)((*this).width()-x-1, y)->red = temp; 

			temp = (*this)(x, y)->green; 
			(*this)(x, y)->green = (*this)((*this).width()-x-1, y)->green; 
			(*this)((*this).width()-x-1, y)->green = temp; 

			temp = (*this)(x, y)->blue; 
			(*this)(x, y)->blue = (*this)((*this).width()-x-1, y)->blue; 
			(*this)((*this).width()-x-1, y)->blue = temp; 
			
		}
	}

}

void Image::adjustbrightness(int r, int g, int b){
	int red, green, blue; 
	for(size_t y = 0; y < (*this).height(); y++){
		for(size_t x = 0; x < (*this).width(); x++){
			red = (*this)(x, y)->red + r; 
			green = (*this)(x, y)->green + g; 
			blue = (*this)(x, y)->blue + b; 

			if(red > 255) {(*this)(x, y)->red = 255;} 
			if(green > 255) {(*this)(x, y)->green = 255;} 
			if(blue > 255) {(*this)(x, y)->blue = 255;} 

			if(red < 0) {(*this)(x, y)->red = 0;} 
			else if(red <= 255) {(*this)(x, y)->red = red;} 
			if(green < 0) {(*this)(x, y)->green = 0;} 
			else if(green <= 255) {(*this)(x, y)->green = green;} 
			if(blue < 0) {(*this)(x, y)->blue = 0;} 
			else if(blue <= 255) {(*this)(x, y)->blue = blue;} 

		}
	}
}

void Image::invertcolors(){
	for(size_t y = 0; y < (*this).height(); y++){
		for(size_t x = 0; x < (*this).width(); x++){
			(*this)(x, y)->red = 255 - (*this)(x, y)->red; 
			(*this)(x, y)->green = 255 - (*this)(x, y)->green; 
			(*this)(x, y)->blue = 255 - (*this)(x, y)->blue;  
		}
	}
}

