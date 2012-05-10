//! \file Console.hpp

/*!
	Albert Elwin
	Last modified : 08.02.12

	References : http://www.gamedev.net/topic/509479-routing-stdout-to-windowconsole/
 */

#ifndef DEBUG_CONSOLE_HPP
#define DEBUG_CONSOLE_HPP

namespace Debug
{
	//! Displays the console and redirects standard i/o
	//! Windows specific function
	void displayConsole();
	//! Closes the console
	//! Windows specific function
	void closeConsole();

} // namespace Debug

#endif