#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "armorPlate.h"
#include<algorithm>
using namespace cv;
using namespace std;

int main()
{
	//�xȡҕ�l
	VideoCapture cap("D:\\Users\\lenovo\\Desktop\\�������� .avi");
	capture_IsOpened(cap);
	while (1)
	{
		ArmorPlate armorPlate;
		armorPlate.setframe(cap);
		//ͼ��Ԥ����
		armorPlate.image_process();
		//ɸѡ����
		armorPlate.light.swap(armorPlate.light_choose());
		if (armorPlate.light.size() < 2)goto show;
		else
		{
			for (int i = 0; i < armorPlate.light.size(); i++)
			{
				armorPlate.draw(armorPlate.light[i]);
			}
		}
		//ƥ�䲢ɸѡװ�װ�
		armorPlate.armor.swap(armorPlate.armor_choose(armorPlate.light));
		if (armorPlate.armor.size() < 1)goto show;
		else
		{
			sort(armorPlate.armor.begin(), armorPlate.armor.end(), cmp);//����װ�װ����������������
			armorPlate.draw(armorPlate.armor[0]);
			circle(armorPlate.getframe(), armorPlate.armor[0].center, 4, Scalar(0, 255, 255), 2);
		}
	show:
		armorPlate.show();
		waitKey(1);
	}
	return 0;
}


