// ConnectCamera.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "../../sdk/inc/TUCamApi.h"                         // µ¼ÈëSDKÍ·ÎÄ¼þ

#include <iostream>
#include <stdio.h>

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

	INT32 nVal = 0;
	INT32 pnVal[4] = {16, 32, 64, 128};


	if (TUCAMRET_SUCCESS == TUCAM_Capa_GetValue(opCam.hIdxTUCam, TUIDC_RESOLUTION, pnVal))
	{
		printf("TUIDC_RESOLUTION is %d, %d, %d, %d \n", pnVal[0], pnVal[1], pnVal[2], pnVal[3], pnVal[4]);

	}

	TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_TEMPERATURE, -10.0);

	double dbVal = 10.0f;

	TUCAM_Prop_SetValue(opCam.hIdxTUCam, TUIDP_EXPOSURETM, dbVal);

	if (TUCAMRET_SUCCESS == TUCAM_Prop_GetValue(opCam.hIdxTUCam, TUIDP_EXPOSURETM, &dbVal))
	{
		
		// dbVal返回当前曝光时间，单位ms
		printf("TUIDP_EXPOSURETM is %f \n", dbVal);

	}



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
