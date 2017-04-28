// SSBAI.h - Contains declaration of Function class  
#pragma once  

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
		static SSBAI_API void initialize(void**frame_buffer, void **mem_buffer);
		static SSBAI_API void frame_update();
	};


}

