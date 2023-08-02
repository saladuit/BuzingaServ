/* ************************************************************************** */
/*                                                                            */
/*                                                    .--.  _                 */
/*   Color.hpp                                       |o_o || |                */
/*                                                   |:_/ || |_ _   ___  __   */
/*   By: safoh <safoh@student.codam.nl>             //   \ \ __| | | \ \/ /   */
/*                                                 (|     | )|_| |_| |>  <    */
/*   Created: 2023/05/25 11:27:00 by safoh        /'\_   _/`\__|\__,_/_/\_\   */
/*   Updated: 2023/05/25 11:27:00 by safoh        \___)=(___/                 */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_HPP
#define COLOR_HPP

#include <string>

class Color
{
  public:
	static const std::string red;
	static const std::string green;
	static const std::string yellow;
	static const std::string blue;
	static const std::string magenta;
	static const std::string cyan;
	static const std::string white;
	static const std::string black;

	static const std::string bold_red;
	static const std::string bold_green;
	static const std::string bold_yellow;
	static const std::string bold_blue;
	static const std::string bold_magenta;
	static const std::string bold_cyan;
	static const std::string bold_white;
	static const std::string bold_black;

	static const std::string reset;
};

/* ********************************Background******************************** */

#define BGWHT "\x1B[1;47m" // White
#define BGBLK "\x1B[1;40m" // Black
#define BGRED "\x1B[1;41m" // Red
#define BGMAG "\x1B[1;45m" // Magenta
#define BGCYN "\x1B[1;46m" // Cyan
#define BGBLU "\x1B[1;44m" // Blue
#define BGGRN "\x1B[1;42m" // Green
#define BGYEL "\x1B[1;43m" // Yellow

#endif

/* ************************************************************************** */
