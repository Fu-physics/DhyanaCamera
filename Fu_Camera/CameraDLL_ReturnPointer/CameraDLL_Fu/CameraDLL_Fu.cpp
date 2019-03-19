// MathLibrary.cpp : Defines the exported functions for the DLL.
#include "stdafx.h"
#include <utility>
#include <limits.h>
#include "CameraDLL_Fu.h"


#include <iostream>
#include <stdio.h>

#include "..\\..\\sdk\\inc\\TUCamApi.h"




// DLL internal state variables:
static unsigned long long previous_;  // Previous value, if any
static unsigned long long current_;   // Current sequence value
static unsigned index_;

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



void CloseCamera()
{
	//TUCAM_Cap_Stop(opCam.hIdxTUCam);                  // Stop capture   
	//TUCAM_Buf_Release(opCam.hIdxTUCam);               // Release alloc buffer after stop capture and quit drawing thread

	if (TUCAMRET_SUCCESS != TUCAM_Dev_Close(opCam.hIdxTUCam)) // 关闭相机
	{
		std::cout << "TUCAM_Dev_Close fail ! \n";
	}
	std::cout << "Camera Closed ! \n";


	if (TUCAMRET_SUCCESS != TUCAM_Api_Uninit()) // 反初始化SDK API环境
	{
		std::cout << "TUCAM_Api_Uninit fail ! \n";
	}

	std::cout << "Uninitialized API ! \n \n \n";
}


void SetTemp(double dbTempSet = 20.0)
{

	// set temperature
	/*
	if (TUCAMRET_SUCCESS == TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_TEMPERATURE, dbTempSet))
	{
		printf("TUIDP_TEMPERATURE is set down ! \n");
	}
	*/


	//get temperature
	double dbtemp = 20.0;
	TUCAM_Prop_GetValue(opCam.hIdxTUCam, TUIDP_TEMPERATURE, &dbtemp);
	printf("TUIDP_TEMPERATURE is %f \n", dbtemp);

	while (dbtemp > dbTempSet)
	{
		Sleep(5000);
		TUCAM_Prop_GetValue(opCam.hIdxTUCam, TUIDP_TEMPERATURE, &dbtemp);
		printf("TUIDP_TEMPERATURE is %f \n", dbtemp);
		//TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_TEMPERATURE, dbTempSet);
	}


}

void SetExposureTime(double dbExpTime = 100.0)
{
	// set exposure time
	TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_EXPOSURETM, dbExpTime);

	// get exposure time
	double dbVal = 0.0;
	if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(opCam.hIdxTUCam, TUIDP_EXPOSURETM, &dbVal))
	{
		// dbVal返回当前曝光时间，单位ms
		printf("The current Exposure Time is: %f (ms) \n", dbVal);
	}
}


// typedef signed int          INT32, *PINT32;
void SetROIMod(INT32 HOffset, INT32 VOffset, INT32 Width, INT32 Height)
{
	roiAttr.bEnable = 1;
	roiAttr.nHOffset = HOffset;
	roiAttr.nVOffset = VOffset;
	roiAttr.nWidth = Width;
	roiAttr.nHeight = Height;

	if (TUCAMRET_SUCCESS != TUCAM_Cap_SetROI(opCam.hIdxTUCam, roiAttr))
	{
		printf("TUCAM_Cap_SetROI fail !\n");
	}


}



unsigned short int* CaptureSingleFrame(int FrameIndex = 1)
{
	m_frame.pBuffer = NULL;
	m_frame.ucFormatGet = TUFRM_FMT_USUAl; // 一般的数据（8bit/16bit、黑白、彩色）
	m_frame.uiRsdSize = 1; // 一次捕获帧数（TUCCM_TRIGGER_STANDARD可大于1）

	if (TUCAMRET_SUCCESS != TUCAM_Buf_Alloc(opCam.hIdxTUCam, &m_frame))
	{
		printf("TUCAM_Buf_Alloc fail !\n");
	}

	if (TUCAMRET_SUCCESS != TUCAM_Cap_Start(opCam.hIdxTUCam, TUCCM_SEQUENCE))
	{
		printf("TUCAM_Cap_Start fail !\n");
	}

	if (TUCAMRET_SUCCESS != TUCAM_Buf_WaitForFrame(opCam.hIdxTUCam, &m_frame))
	{
		printf("TUCAM_Buf_WaitForFrame is fail \n");
		/*
		fileSave.nSaveFmt = TUFMT_TIF; // 保存Tiff格式
		fileSave.pFrame = &m_frame; // 需要保存的帧指针
		//char * s = "Joe";
		//char pss[] = "..\\..\\..\\ImageContainer\\image1";

		
		// this par is for saving the image
		char filename[100];

		sprintf_s(filename, "..\\..\\ImageContainer\\image%03d", FrameIndex);

		printf("the image name is: %s \n", filename);

		// using "\\" not '\'
		// fileSave.pstrSavePath = "C:\\Users\\Agarwal\\Desktop\\image1"; will not work!
		//Because that "C:\\Users\\Agarwal\\Desktop\\image1" is a constant char array, which cannot pass to a char pointer

		fileSave.pstrSavePath = filename; // 路径包含文件名（不包含扩展名
		//char pss[] = "..\\..\\..\\image";
		//fileSave.pstrSavePath = pss;

		//if (TUCAMRET_SUCCESS != TUCAM_File_SaveImage(opCam.hIdxTUCam, fileSave))
		//{
		//	printf("TUCAM_File_SaveImage fail ! \n");
		//}
		*/

	}



	// read the image data directly from the memory block
	TUCAM_IMG_HEADER frmhead;

	memcpy(&frmhead, m_frame.pBuffer, sizeof(TUCAM_IMG_HEADER));


	// "%hu" is the unsing short
	//printf("the image (width, hight) is: (%hu, %hu) \n", frmhead.usWidth, frmhead.usHeight);
	//std::cout << "sizeof(frmhead.pImgData) is : " << (sizeof(frmhead.pImgData[0])) << "\n";

	// cast to a unsigned short int, which is 16 bites
	unsigned short int *intvalu;
	intvalu = (unsigned short int*)(frmhead.pImgData);
	return intvalu;

	// method two
	/*
	std::cout << "frmhead.pImgData is : ";
	for(int i = 0; i < 10; i+=2)
	{

		int x = (int)(frmhead.pImgData[i]);
		int y = (int)(frmhead.pImgData[i+1]);
		//std::cout << x << " " << y;
		std::cout << "(" << 256*x + y << ") \t";
		//std::cout << x << " " << y << "(" << x + y * 256 << ") \t";

		//twoBytes = frmhead.pImgData[i-1] << 8 | frmhead.pImgData[i];
		//cout << twoBytes << "\t";
	}
	std::cout << "\n";
	*/

	/*
	std::cout << "frmhead.pImgData is : ";
	for (int i = 0; i < 10; i++)
	{
		std::cout << (int)(intvalu[i]) << "\t";
	}

	std::cout << "\n";
	*/
}


void CaptureStop() {
	// Stop capture
	if (TUCAMRET_SUCCESS != TUCAM_Cap_Stop(opCam.hIdxTUCam))
	{
		printf("TUCAM_Cap_Stop fail !\n");
	}

}

void BufRelease()
{
	// Release alloc buffer after stop capture and quit drawing thread
	if (TUCAMRET_SUCCESS != TUCAM_Buf_Release(opCam.hIdxTUCam))
	{
		printf("TUCAM_Buf_Release fail !\n");
	}

}


           // Current seq. position

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

int sum(int a, int b)
{

	return a + b;
}