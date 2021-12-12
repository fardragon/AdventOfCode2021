#include "Cave.hpp"

bool Cave::IsBig() const
{
	return std::all_of(name.begin(), name.end(),
	                   [](unsigned char c)
	                   {
		                   return static_cast<bool>(std::isupper(c));
	                   }
	);
}
