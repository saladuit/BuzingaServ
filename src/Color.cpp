/* ************************************************************************** */
/*                                                                            */
/*                                                    .--.  _                 */
/*   Color.cpp                                       |o_o || |                */
/*                                                   |:_/ || |_ _   ___  __   */
/*   By: safoh <safoh@student.codam.nl>             //   \ \ __| | | \ \/ /   */
/*                                                 (|     | )|_| |_| |>  <    */
/*   Created: 2023/07/19 15:08:14 by safoh        /'\_   _/`\__|\__,_/_/\_\   */
/*   Updated: 2023/07/19 15:08:14 by safoh        \___)=(___/                 */
/*                                                                            */
/* ************************************************************************** */

#include <Color.hpp>

/* **********************************Normal********************************** */

const std::string Color::red = "\x1B[0;31m";
const std::string Color::green = "\x1B[0;32m";
const std::string Color::yellow = "\x1B[0;33m";
const std::string Color::blue = "\x1B[0;34m";
const std::string Color::magenta = "\x1B[0;35m";
const std::string Color::cyan = "\x1B[0;36m";
const std::string Color::white = "\x1B[0;37m";
const std::string Color::black = "\x1B[0;30m";
const std::string Color::reset = "\x1B[0m";

/* ***********************************Bold*********************************** */

const std::string Color::bold_red = "\x1B[1;37m";
const std::string Color::bold_green = "\x1B[1;30m";
const std::string Color::bold_yellow = "\x1B[1;31m";
const std::string Color::bold_blue = "\x1B[1;35m";
const std::string Color::bold_magenta = "\x1B[1;36m";
const std::string Color::bold_cyan = "\x1B[1;34m";
const std::string Color::bold_white = "\x1B[1;32m";
const std::string Color::bold_black = "\x1B[1;33m";
