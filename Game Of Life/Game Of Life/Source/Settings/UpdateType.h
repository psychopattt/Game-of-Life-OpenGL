#pragma once

enum UpdateType : char
{
	None = 0,
	Display = 1 << 0,
	Simulation = 1 << 1
};

inline UpdateType operator |(UpdateType a, UpdateType b)
{
	return static_cast<UpdateType>(static_cast<char>(a) | static_cast<char>(b));
}

inline UpdateType operator &(UpdateType a, UpdateType b)
{
	return static_cast<UpdateType>(static_cast<char>(a) & static_cast<char>(b));
}

inline UpdateType& operator |=(UpdateType& a, UpdateType b)
{
	return a = a | b;
}
