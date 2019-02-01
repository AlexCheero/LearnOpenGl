#pragma once
#include <iostream>

class Log
{
public:
	//can't use std::endl
	template <typename T>
	Log& operator <<(T const& value)
	{
		std::cout << value;
		return *this;
	}
};