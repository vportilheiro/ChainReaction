/*	colormod.h
 *
 *	This file provides a way to change the attributes (color, bold, italic, etc...)
 *	of text printed to a Unix-like console, using ANSI escape codes.
 *
 *	Credit for this code goes to Joel Sjögren, who posted it as an answer on StackOverflow.
 *	Found at: http://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
 *
 *	Vasco Portilheiro, 2015
 */

#ifndef _COLORMOD_H_
#define _COLORMOD_H_

#include <ostream>

namespace Color {

	enum Code {
		DEFAULT		= 0,
		BOLD		= 1,
		BLINK		= 5,
		RED		    = 31,
		GREEN		= 32,
		YELLOW		= 33,
		BLUE		= 34,
		MAGENTA		= 35,
		CYAN		= 36,
	};

	/* Modifier class, through which text may be streamed. Note that
	 * a modifier may be "hidden" and thus text will not be changed. */
	class Modifier {
	public:

		Modifier(Code pCode, bool show) : code(pCode), show(show) {}
		friend std::ostream& operator<<(std::ostream& os, const Modifier& mod) {
			if (mod.show) {
				return os << "\033[" << mod.code << "m";
			} else {
				return os;
			}
		}

		/* Toggles to show/hide modifier */
		void enable() {
			show = true;
		}

		void disable() {
			show = false;
		}

		/* Returns whether the modifier is enabled */
		bool enabled() const {
			return show;
		}


		/* Allows to set the code */
		void setCode(Code pCode) {
			code = pCode;
		}

	private:
		Code code;
		bool show;
	};

	/* Returns a color code from an integer from 0 to 6,
	 * where 0 is the default coloring, and the rest follow
	 * the order of colors in Code */
	extern Code colorFromInt(int n);
}	

#endif
