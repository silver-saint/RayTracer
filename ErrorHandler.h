#pragma once
#include "types.h"
#include <string>
#ifdef NDEBUG
const bool DEBUGLAYER = false;
#else
const bool DEBUGLAYER = true;
#endif

inline static void PrintError(const std::wstring& errorMsg, bool DebugLayerOn)
{
	MessageBox(nullptr, errorMsg.c_str(), L"Error", MB_OK);
	(DebugLayerOn ? DebugBreak() : exit(1));
}