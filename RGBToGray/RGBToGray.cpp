#include <iostream>
#define BYTE unsigned char
using namespace std;

bool RGBToGray(BYTE *pData, int iWidth, int iHeight, int iDepth)
{
	BYTE red, green, blue;
	BYTE grey;

	if(iDepth != 3 && iDepth != 4)
	{
		return FALSE;
	}
	
	for(int j = 0; j < iHeight; ++j)
	{
		for(int i = 0; i < iWidth; ++i)
		{
			blue = *(pData + j*iWidth*iDepth + i*iDepth);
			green = *(pData + j*iWidth*iDepth + i*iDepth + 1);
			red = *(pData + j*iWidth*iDepth + i*iDepth + 2);

			grey = (BYTE)((float)(red*0.299 + green*0.587 + blue*0.114));

			*(pData + j*iWidth*iDepth + i*iDepth) = grey;
			*(pData + j*iWidth*iDepth + i*iDepth + 1) = grey;
			*(pData + j*iWidth*iDepth + i*iDepth + 2) = grey;
		}
	}
	return TRUE;
}
