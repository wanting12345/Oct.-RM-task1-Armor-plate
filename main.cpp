#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "armorPlate.h"
#include<algorithm>
using namespace cv;
using namespace std;

int main()
{
	//讀取視頻
	VideoCapture cap("D:\\Users\\lenovo\\Desktop\\能量机关 .avi");
	capture_IsOpened(cap);
	while (1)
	{
		ArmorPlate armorPlate;
		armorPlate.setframe(cap);
		//图像预处理
		armorPlate.image_process();
		//筛选灯条
		armorPlate.light.swap(armorPlate.light_choose());
		if (armorPlate.light.size() < 2)goto show;
		else
		{
			for (int i = 0; i < armorPlate.light.size(); i++)
			{
				armorPlate.draw(armorPlate.light[i]);
			}
		}
		//匹配并筛选装甲板
		armorPlate.armor.swap(armorPlate.armor_choose(armorPlate.light));
		if (armorPlate.armor.size() < 1)goto show;
		else
		{
			sort(armorPlate.armor.begin(), armorPlate.armor.end(), cmp);//根据装甲板的面积降序进行排序
			armorPlate.draw(armorPlate.armor[0]);
			circle(armorPlate.getframe(), armorPlate.armor[0].center, 4, Scalar(0, 255, 255), 2);
		}
	show:
		armorPlate.show();
		waitKey(1);
	}
	return 0;
}


