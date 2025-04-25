#pragma once
#include <cstdint>
#include <vector>


namespace mem
{
	uintptr_t FindAdress(uintptr_t ptr, std::vector<unsigned int> offsets)
	{
		uintptr_t address = ptr;
		for (unsigned int i = 0; i < offsets.size(); ++i)
		{
			adress = *(uintptr_t*)adress;
			adress += offsets[i];
		}
		return adress;
	}
}