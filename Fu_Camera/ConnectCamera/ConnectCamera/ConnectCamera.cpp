// ConnectCamera.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "../../sdk/inc/TUCamApi.h"                         // µ¼ÈëSDKÍ·ÎÄ¼þ

#include <iostream>
#include <stdio.h>

using namespace std; //now you don't have to write 'std::' anymore.

/*void GetResolitionRange()
{
	TUCAM_CAPA_ATTR attrCapa;
	TUCAM_VALUE_TEXT valText;

	char szRes[64] = { 0 };
	valText.nTextSize = 64;
	valText.pText = &szRes[0];
	attrCapa.idCapa = TUIDC_RESOLUTION;
	if (TUCAMRET_SUCCESS == TUCAM_Capa_GetAttr(opCam.hIdxTUCam, &attrCapa))
	{
		int nCnt = attrCapa.nValMax - attrCapa.nValMin + 1;
		valText.nID = TUIDC_RESOLUTION;
		for (int i = 0; i < nCnt; ++i)
		{
			valText.dbValue = i;
			TUCAM_Capa_GetValueText(opCam.hIdxTUCam, &valText);
			szRes = valText.pText;
			// 将分辨率文本添加到下拉菜单

		}

	}

};
*/

/*

void GetCurrentResolution()
{
	int nVal = 0;
	if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(opCam.hIdxTUCam, UIDC_RESOLUTION, &nVal))
	{
	}
}

*/


int main()
{
	std::cout << "Hello World!\n";

	TUCAM_INIT itApi; // 初始化SDK环境参数
	TUCAM_OPEN opCam; // 打开相机参数

	itApi.pstrConfigPath = NULL;
	itApi.uiCamCount = 0;


	if (TUCAMRET_SUCCESS != TUCAM_Api_Init(&itApi))
	{
		std::cout << "Initialized API fail ! \n";
		return 0;
	}

	std::cout << "Camera Number is:" << itApi.uiCamCount << "\n";


	opCam.hIdxTUCam = 0;
	opCam.uiIdxOpen = 0;
	TUCAM_Dev_Open(&opCam);

	/*
	if (TUCAMRET_SUCCESS != TUCAM_Dev_Open(&opCam))
	{
		std::cout << "Open Camera fail! \n";
		return 0;
	}
	*/

	// 设置当前分辨率
	//TUCAM_Capa_SetValue(opCam.hIdxTUCam, TUIDC_RESOLUTION, nIdxRes);

	//INT32 pnVal[4] = { 16, 32, 64, 128 };


	/*
	if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(opCam.hIdxTUCam, TUIDC_RESOLUTION, pnVal))
	{
		printf("TUIDC_RESOLUTION is %d, %d, %d, %d \n", pnVal[0], pnVal[1], pnVal[2], pnVal[3]);
	}
	*/

	// set temperature
	double dbTempSet = -10.0;
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

	while (dbtemp  > dbTempSet)
	{
		Sleep(5000);
		TUCAM_Prop_GetValue(opCam.hIdxTUCam, TUIDP_TEMPERATURE, &dbtemp);
		printf("TUIDP_TEMPERATURE is %f \n", dbtemp);
		//TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_TEMPERATURE, dbTempSet);
	}

	// set exposure time
	double dbVal = 100.0f;
	TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_EXPOSURETM, dbVal);

	// get exposure time
	if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(opCam.hIdxTUCam, TUIDP_EXPOSURETM, &dbVal))
	{

		// dbVal返回当前曝光时间，单位ms
		printf("TUIDP_EXPOSURETM is %f \n", dbVal);

	}

	// 获取相机通道数
	TUCAM_VALUE_INFO valInfo;
	valInfo.nValue = 1;
	valInfo.nID = TUIDI_CAMERA_CHANNELS;
	TUCAM_Dev_GetInfo(opCam.hIdxTUCam, &valInfo);

	TUCAM_FRAME m_frame;  // 帧对象
	HANDLE m_hThdGrab; // 取图线程事件句柄
	BOOL m_bLiving; // 是否取图

	m_frame.pBuffer = NULL;
	m_frame.ucFormatGet = TUFRM_FMT_USUAl; // 一般的数据（8bit/16bit、黑白、彩色）
	m_frame.uiRsdSize = 1; // 一次捕获帧数（TUCCM_TRIGGER_STANDARD可大于1）

	if(TUCAMRET_SUCCESS != TUCAM_Buf_Alloc(opCam.hIdxTUCam,  &m_frame))
	{
		printf("TUCAM_Buf_Alloc fail !\n");
	}

	if (TUCAMRET_SUCCESS != TUCAM_Cap_Start(opCam.hIdxTUCam, TUCCM_SEQUENCE))
	{
		printf("TUCAM_Cap_Start fail !\n");
	}

	printf("m_frame.usWidth is %d, m_frame.usHeight is %d \n", m_frame.usWidth, m_frame.usHeight);


	// print out the image data 

	int N = 10;

	for(int i = 0; i <N; i++)
	{
		//printf("m_frame.pBuffer[%d] is %d", i, m_frame.pBuffer[i]);
		cout << i << static_cast<unsigned>(m_frame.pBuffer[i]) << std::endl;

	}

	// save image.....
	m_frame.ucFormatGet = TUFRM_FMT_USUAl; // 一般的数据（8bit/16bit、黑白、彩色）
	if (TUCAMRET_SUCCESS == TUCAM_Buf_WaitForFrame(opCam.hIdxTUCam, &m_frame))
	{
		TUCAM_FILE_SAVE fileSave;
		fileSave.nSaveFmt = TUFMT_TIF; // 保存Tiff格式
		fileSave.pFrame = &m_frame; // 需要保存的帧指针
		//char * s = "Joe";
		char pss[] = "C:\\Users\\Agarwal\\Desktop\\image1"; 
				// using "\\" not '\'
				// fileSave.pstrSavePath = "C:\\Users\\Agarwal\\Desktop\\image1"; will not work! 
				//Because that "C:\\Users\\Agarwal\\Desktop\\image1" is a constant char array, which cannot pass to a char pointer
		fileSave.pstrSavePath = pss; // 路径包含文件名（不包含扩展名
		if (TUCAMRET_SUCCESS == TUCAM_File_SaveImage(opCam.hIdxTUCam, fileSave))
		{
			printf("TUCAM_File_SaveImage Success");
		}

	}


	TUCAM_Cap_Stop(opCam.hIdxTUCam);                  // Stop capture   
	TUCAM_Buf_Release(opCam.hIdxTUCam);               // Release alloc buffer after stop capture and quit drawing thread

	TUCAM_Dev_Close(opCam.hIdxTUCam); // 关闭相机
	TUCAM_Api_Uninit(); // 反初始化SDK API环境


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
