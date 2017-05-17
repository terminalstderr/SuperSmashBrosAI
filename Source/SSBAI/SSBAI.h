// SSBAI.h - Contains declaration of Function class  
#pragma once  
#include "Utility.h"

#ifdef SSBAI_EXPORTS  
#define SSBAI_API __declspec(dllexport)   
#else  
#define SSBAI_API __declspec(dllimport)   
#endif  

namespace ssbai
{
	// This class is exported from SBBAI.dll  
	class Hooks
	{
	public:
		static SSBAI_API void frame_update(uint8_t *memory, uint32_t *controller1, uint32_t *controller2);
	};


}

