/*	colormod.cpp
 *
 *	Implements extern globals from colormod.h
 *
 *	Vasco Portilheiro, 2015
 */

#include <iostream>
#include "colormod.h"

namespace Color {
	Code colorFromInt(int n) {
		switch(n) {
			case 0:
				return Code::DEFAULT;
				break;
			case 1:
				return Code::RED;
				break;
			case 2:
				return Code::GREEN;
				break;
			case 3:
				return Code::YELLOW;
				break;
			case 4:
				return Code::BLUE;
				break;
			case 5:
				return Code::MAGENTA;
				break;
			case 6:
				return Code::CYAN;
				break;
			default:
				throw "Number outside of range [0,6] given to Color::colorFromInt()";
		}
	}
}
