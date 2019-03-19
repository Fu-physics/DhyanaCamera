// ConnectCamera.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "../../sdk/inc/TUCamApi.h"                         // µ¼ÈëSDKÍ·ÎÄ¼þ

#include <iostream>
#include <stdio.h>

using namespace std; //now you don't have to write 'std::' anymore.


TUCAM_INIT itApi; // 初始化SDK环境参数
TUCAM_OPEN opCam; // 打开相机参数

TUCAM_FRAME m_frame;  // 帧对象
HANDLE m_hThdGrab; // 取图线程事件句柄
BOOL m_bLiving; // 是否取图

TUCAM_FILE_SAVE fileSave; // save file

TUCAM_ROI_ATTR roiAttr; // set frame size

//char cSN[TUSN_SIZE] = { 0 };





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


void SetTemp(double dbTempSet)
{

	// set temperature
	/*
	if (TUCAMRET_SUCCESS == TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_TEMPERATURE, dbTempSet))
	{
		printf("TUIDP_TEMPERATURE is set down ! \n");
	}
	*/

	//get temperature
	double dbtemp = 10.0;
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

void SetExposureTime(double dbExpTime)
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



void SetROIMod()
{
	roiAttr.bEnable = 1;
	roiAttr.nVOffset = 1024;
	roiAttr.nHOffset = 1024;
	roiAttr.nWidth = 256;
	roiAttr.nHeight = 256;

	if (TUCAMRET_SUCCESS != TUCAM_Cap_SetROI(opCam.hIdxTUCam, roiAttr))
	{
		printf("TUCAM_Cap_SetROI fail !\n");
	}

}


unsigned short int* CaptureSingleFrame(int FrameIndex=1)
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

	

	if (TUCAMRET_SUCCESS == TUCAM_Buf_WaitForFrame(opCam.hIdxTUCam, &m_frame))
	{
		printf("TUCAM_Buf_WaitForFrame is SUCCESS..\n");
		/*

		fileSave.nSaveFmt = TUFMT_TIF; // 保存Tiff格式
		fileSave.pFrame = &m_frame; // 需要保存的帧指针
		//char * s = "Joe";
		//char pss[] = "..\\..\\..\\ImageContainer\\image1";

		char filename[100];

		sprintf_s(filename, "..\\..\\..\\ImageContainer\\image%03d", FrameIndex);

		printf("the image name is: %s \n", filename);

		// using "\\" not '\'
		// fileSave.pstrSavePath = "C:\\Users\\Agarwal\\Desktop\\image1"; will not work! 
		//Because that "C:\\Users\\Agarwal\\Desktop\\image1" is a constant char array, which cannot pass to a char pointer
		//fileSave.pstrSavePath = filename; // 路径包含文件名（不包含扩展名
		char pss[] = ".\\image1";
		fileSave.pstrSavePath = pss;
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
	printf("the image (width, hight) is: (%hu, %hu) \n", frmhead.usWidth, frmhead.usHeight);
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
	if (TUCAMRET_SUCCESS == TUCAM_Buf_Release(opCam.hIdxTUCam))
	{
		printf("TUCAM_Buf_Release SUCCESS !\n");
	}

}



int main()
{
	// Open the camera ...
	OpenCamera();

	// Set the Temperature
	double Temperature = 30.0;
	SetTemp(Temperature);

	// Set the ExposureTime
			// the time unit is "ms" 
	double ExposureTime = 10.0;
	SetExposureTime(ExposureTime);

	// set the frame size
	//SetROIMod();

	unsigned short int* imageDta;
	
	// Capture multi-frames
	for (int i=1; i < 10; i++)
	{
		SetROIMod(); // for each frame, you have to set the ROI. So it cannot put out of this for loop
					 //CaptureSingleFrame
		imageDta = CaptureSingleFrame(i);

		
		std::cout << "frmhead.pImgData is : ";
		for (int i = 0; i < 10; i++)
		{
			std::cout << (int)(imageDta[i]) << "\t";
		}

		std::cout << "\n";

		// Stop capture
		CaptureStop();
		// Release alloc buffer after stop capture and quit drawing thread
		BufRelease();
	}
	
	std::cout << "\n \n \n";
	CloseCamera();
	std::cout << "The end!\n";

}




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
