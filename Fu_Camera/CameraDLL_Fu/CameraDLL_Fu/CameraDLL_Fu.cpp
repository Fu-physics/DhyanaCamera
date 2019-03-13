// MathLibrary.cpp : Defines the exported functions for the DLL.
#include "stdafx.h"
#include <utility>
#include <limits.h>
#include "CameraDLL_Fu.h"


#include <iostream>
#include <stdio.h>

#include "..\\..\\sdk\\inc\\TUCamApi.h"

TUCAM_INIT itApi; // 初始化SDK环境参数
TUCAM_OPEN opCam; // 打开相机参数

TUCAM_FRAME m_frame;  // 帧对象
HANDLE m_hThdGrab; // 取图线程事件句柄
BOOL m_bLiving; // 是否取图

TUCAM_FILE_SAVE fileSave; // save file

TUCAM_ROI_ATTR roiAttr; // set frame size


void OpenCamera()
{
	std::cout << "Opening Camera ...\n";

	itApi.pstrConfigPath = NULL;
	itApi.uiCamCount = 0;

	if (TUCAMRET_SUCCESS != TUCAM_Api_Init(&itApi))
	{
		std::cout << "Initialized API fail ! \n";
	}

	std::cout << "Initialized API ! \n";

	opCam.hIdxTUCam = 0;
	opCam.uiIdxOpen = 0;
	TUCAM_Dev_Open(&opCam);


	std::cout << "Camera was opened \n";
	std::cout << "Camera Number is:" << itApi.uiCamCount << "\n";
	std::cout << "\n \n \n";
}






// DLL internal state variables:
static unsigned long long previous_;  // Previous value, if any
static unsigned long long current_;   // Current sequence value
static unsigned index_;               // Current seq. position

// Initialize a Fibonacci relation sequence
// such that F(0) = a, F(1) = b.
// This function must be called before any other function.
void fibonacci_init(
	const unsigned long long a,
	const unsigned long long b)
{
	index_ = 0;
	current_ = a;
	previous_ = b; // see special case when initialized
}

// Produce the next value in the sequence.
// Returns true on success, false on overflow.
bool fibonacci_next()
{
	// check to see if we'd overflow result or position
	if ((ULLONG_MAX - previous_ < current_) ||
		(UINT_MAX == index_))
	{
		return false;
	}

	// Special case when index == 0, just return b value
	if (index_ > 0)
	{
		// otherwise, calculate next sequence value
		previous_ += current_;
	}
	std::swap(current_, previous_);
	++index_;
	return true;
}

// Get the current value in the sequence.
unsigned long long fibonacci_current()
{
	return current_;
}

// Get the current index position in the sequence.
unsigned fibonacci_index()
{
	return index_;
}