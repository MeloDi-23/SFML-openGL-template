#pragma once
#ifndef NDEBUG
#include <iostream>

#define PRINT_VALUE(v) (std::cout << "Value " << #v << " = " << v << " at func " << __func__<< " line "<< __LINE__ <<" file "<< __FILE__ << std::endl)
#define PRINT_MSG(s) (std::cout << "Message " s " at func " << __func__<< " line "<< __LINE__ <<" file "<< __FILE__ << std::endl)

#else
#define PRINT_VALUE(v) {}
#define PRINT_MSG(s) {}

#endif

// 古法debug，出于某种位置原因，无法使用断点