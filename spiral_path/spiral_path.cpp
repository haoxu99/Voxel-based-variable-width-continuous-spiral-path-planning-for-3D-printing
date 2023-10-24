#include<iostream>
#include<OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>  
#include <OpenMesh/Core/Mesh/Handles.hh>
#include <OpenMesh\Core\Utils\Property.hh> 
#include <string>
#include <vector>
#include <algorithm>
#include<stdio.h>
#include "clipper.hpp"  
#include"clipper.cpp"
#include<GL/glut.h>
#define use_int32
#define pi 3.1415926
using namespace std;

struct element {//单元结构体，里面存储了每个单元的全部信息
	int id;
	double x, y, z, s11, s22, s12, s_max, direction;
	int b;
};
struct points {//存储bool为1 的单元信息
	int id;
	double x, y, z, s_max, direction;
	int b = 0;//该单元的度数
};
struct path {//路径结构体，每条路径都是一个双向
	int id = 0;
	double x, y, z, direction;
};

vector<element>point_buffer1;//第一层缓存
vector<vector<element>>point_buffer2;//第二层缓存
vector<vector<vector<element>>>point;//每个单元的信息
vector<vector<points>>model;//按应力大小，对有效单元进行排序，大->小,一维代表层数
vector<points>model_buffer;
vector<vector<vector<path>>>paths;//存储路径，一维代表层数
vector<path>paths_buffer1;
vector<vector<path>>paths_buffer2;

int x_count, y_count, z_count;//分别表示x，y,z的单元个数
double Bmin_x, Bmin_y, Bmax_x, Bmax_y;

void ReadFile() {
	element e;
	ifstream file1, file2;
	file1.open("VoxelizationData.txt");//读取体素文件
	file2.open("abaqus.txt");//读取应力文件
	file1 >> x_count;
	file1 >> y_count;
	file1 >> z_count;
	file1 >> Bmin_x;
	file1 >> Bmax_x;
	file1 >> Bmin_y;
	file1 >> Bmax_y;
	printf("%d %d %d \n", x_count, y_count, z_count);
	for (int i = 0; i < z_count; i++) {//将每个单元的坐标信息存入数组
		for (int j = 0; j < y_count; j++) {
			for (int m = 0; m < x_count; m++) {
				for (int s = 0; s < 5; s++) {
					switch (s) {
					case 0:file1 >> e.id; break;
					case 1:file1 >> e.x; break;
					case 2:file1 >> e.y; break;
					case 3:file1 >> e.z; break;
					case 4:file1 >> e.b; break;
					}
				}
				point_buffer1.push_back(e);
			}
			point_buffer2.push_back(point_buffer1);
			point_buffer1.clear();
		}
		point.push_back(point_buffer2);
		point_buffer2.clear();
	}
	for (int i = 0; i < point.size(); i++) {//读取单元的x,y应力数据
		for (int j = 0; j < point[0].size(); j++) {
			for (int m = 0; m < point[0][0].size(); m++) {
				if (point[i][j][m].b == 1) {
					for (int s = 0; s < 4; s++) {
						switch (s) {
						case 0: {file2 >> e.id; if (e.id != point[i][j][m].id) printf("文件读取错误！\n"); break; }
						case 1:file2 >> point[i][j][m].s11; break;
						case 2:file2 >> point[i][j][m].s22; break;
						case 3:file2 >> point[i][j][m].s12; break;
						}
					}
				}
			}
		}
	}
}

double distance(path a, path b)	//两点间距离
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

void FE_Analysis() {//计算最大主应力以及最大主应力方向
	double s1, s2, d1, d2;
	for (int i = 0; i < point.size(); i++) {
		for (int j = 0; j < point[i].size(); j++) {
			for (int m = 0; m < point[i][j].size(); m++) {
				if (point[i][j][m].b == 1) {
					//最大主应力数值（abs）
					s1 = ((point[i][j][m].s11 + point[i][j][m].s22) / (double)2) - sqrt(pow(((point[i][j][m].s11 - point[i][j][m].s22) / (double)2), 2) + pow(point[i][j][m].s12, 2));
					s2 = ((point[i][j][m].s11 + point[i][j][m].s22) / (double)2) + sqrt(pow(((point[i][j][m].s11 - point[i][j][m].s22) / (double)2), 2) + pow(point[i][j][m].s12, 2));
					if (fabs(s1) >= fabs(s2)) {
						point[i][j][m].s_max = s1;
					}
					else {
						point[i][j][m].s_max = s2;
					}
					//最大主应力方向
					d1 = atan(-(2 * point[i][j][m].s12) / (point[i][j][m].s11 - point[i][j][m].s22));
					if (point[i][j][m].s11 >= point[i][j][m].s22) {
						if (d1 <= 0) d1 = d1 + 2 * pi;
						
					}
					else if (point[i][j][m].s11 <= point[i][j][m].s22) {
						d1 = d1 + pi;
					}
					d1 = d1 / (double)2;
					//最小主应力方向
					d2= atan(-(2 * point[i][j][m].s12) / (point[i][j][m].s11 - point[i][j][m].s22));
					if (point[i][j][m].s11 >= point[i][j][m].s22) {
						d2 = d2 + pi;
					}
					else if (point[i][j][m].s11 <= point[i][j][m].s11) {
						if (d2 <= 0) d2 = d2 + 2 * pi;
					}
					d2 = d2 / (double)2;
					//最大主应力（abs）方向
					if ((s1 > s2 && fabs(s1) > fabs(s2)) || (s2 > s1 && fabs(s2) > fabs(s1))) point[i][j][m].direction = d1;
					else if ((s1 > s2 && fabs(s1) < fabs(s2)) || (s2 > s1 && fabs(s2) < fabs(s1))) point[i][j][m].direction = d2;
				}
			}
		}
	}
}

void VectorInsert(int i,int m) {//生成单路径函数
	bool flag = true;
	path s;
	while (flag) {
		flag = false;
		if (model[i][m].direction >= 0 && model[i][m].direction < pi / (double)8) {
			
			for (int j = 0; j < model[i].size(); j++) {
				if (model[i][j].id == model[i][m].id + 1 && model[i][m].id % x_count != 0 && model[i][j].b == 0) {//右边的单元
					model[i][m].b++;
					model[i][j].b++;
					m = j;
					s.id = model[i][j].id;
					s.x = model[i][j].x;
					s.y = model[i][j].y;
					s.z = model[i][j].z;
					s.direction = model[i][j].direction;
					paths_buffer1.push_back(s);
					flag = true;
					break;
				}
			}
		}
		else if (model[i][m].direction >= pi / (double)8 && model[i][m].direction < pi * ((double)3 / (double)8)) {
			for (int j = 0; j < model[i].size(); j++) {
				if (model[i][j].id == model[i][m].id + x_count + 1 && model[i][m].id % x_count != 0 && model[i][j].b == 0) {//右上边的单元
					model[i][m].b++;
					model[i][j].b++;
					m = j;
					s.id = model[i][j].id;
					s.x = model[i][j].x;
					s.y = model[i][j].y;
					s.z = model[i][j].z;
					s.direction = model[i][j].direction;
					paths_buffer1.push_back(s);
					flag = true;
					break;
				}
			}
		}
		else if (model[i][m].direction >= pi * ((double)3 / (double)8) && model[i][m].direction < pi * ((double)5 / (double)8)) {
			for (int j = 0; j < model[i].size(); j++) {
				if (model[i][j].id == model[i][m].id + x_count && model[i][j].b == 0) {//上边的单元
					model[i][m].b++;
					model[i][j].b++;
					m = j;
					s.id = model[i][j].id;
					s.x = model[i][j].x;
					s.y = model[i][j].y;
					s.z = model[i][j].z;
					s.direction = model[i][j].direction;
					paths_buffer1.push_back(s);
					flag = true;
					break;
				}
			}
		}
		else if (model[i][m].direction >= pi * ((double)5 / (double)8) && model[i][m].direction < pi * ((double)7 / (double)8)) {
			for (int j = 0; j < model[i].size(); j++) {
				if (model[i][j].id == model[i][m].id + x_count - 1 && model[i][m].id % x_count != 1 && model[i][j].b == 0) {//左上边的单元
					model[i][m].b++;
					model[i][j].b++;
					m = j;
					s.id = model[i][j].id;
					s.x = model[i][j].x;
					s.y = model[i][j].y;
					s.z = model[i][j].z;
					s.direction = model[i][j].direction;
					paths_buffer1.push_back(s);
					flag = true;
					break;
				}
			}
		}
		else if (model[i][m].direction >= pi * ((double)7 / (double)8) && model[i][m].direction < pi) {
			for (int j = 0; j < model[i].size(); j++) {
				if (model[i][j].id == model[i][m].id - 1 && model[i][m].id % x_count != 1 && model[i][j].b == 0) {//左边的单元
					model[i][m].b++;
					model[i][j].b++;
					m = j;
					s.id = model[i][j].id;
					s.x = model[i][j].x;
					s.y = model[i][j].y;
					s.z = model[i][j].z;
					s.direction = model[i][j].direction;
					paths_buffer1.push_back(s);
					flag = true;
					break;
				}
			}
		}
	}
}

void OptimizePoints() {//优化空点函数
	path s;
	for (int i = 0; i < model.size(); i++) {
		for (int j = 0; j < model[i].size(); j++) {
			if (model[i][j].b == 0) {
				int Right_up = 0;//判断该空点的斜连接是否可连接，可以连接为0，不可连接为1
				int Left_up = 0;
				int Right_down = 0;
				int Left_down = 0;
				for (int m = 0; m < paths[i].size(); m++) {
					for (int n = 1; n < paths[i][m].size(); n++) {
						if ((model[i][j].id + 1 == paths[i][m][n - 1].id && model[i][j].id + x_count == paths[i][m][n].id && model[i][j].id % x_count != 0) ||
							(model[i][j].id + 1 == paths[i][m][n].id && model[i][j].id + x_count == paths[i][m][n - 1].id && model[i][j].id % x_count != 0)) {
							Right_up = 1;//如果与右上的端点连接，则会与已有的路径交叉
						}
						if ((model[i][j].id - 1 == paths[i][m][n - 1].id && model[i][j].id + x_count == paths[i][m][n].id && model[i][j].id % x_count != 1) ||
							(model[i][j].id - 1 == paths[i][m][n].id && model[i][j].id + x_count == paths[i][m][n - 1].id && model[i][j].id % x_count != 1)) {
							Left_up = 1;//如果与左上的端点连接，则会与已有的路径交叉
						}
						if ((model[i][j].id + 1 == paths[i][m][n - 1].id && model[i][j].id - x_count == paths[i][m][n].id && model[i][j].id % x_count != 0) ||
							(model[i][j].id + 1 == paths[i][m][n].id && model[i][j].id - x_count == paths[i][m][n - 1].id && model[i][j].id % x_count != 0)) {
							Right_down = 1;//如果与右下的端点连接，则会与已有的路径交叉
						}
						if ((model[i][j].id - 1 == paths[i][m][n - 1].id && model[i][j].id - x_count == paths[i][m][n].id && model[i][j].id % x_count != 1) ||
							(model[i][j].id - 1 == paths[i][m][n].id && model[i][j].id - x_count == paths[i][m][n - 1].id && model[i][j].id % x_count != 1)) {
							Left_down = 1;//如果与左下的端点连接，则会与已有的路径交叉
						}
					}
				}
				for (int m = 0; m < paths[i].size(); m++) {
					//右-终点
					if (paths[i][m][paths[i][m].size() - 1].id == model[i][j].id + 1 && model[i][j].id % x_count != 0) {
						model[i][j].b++;
						s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
						paths[i][m].push_back(s);
						j = 0;
						break;//寻找到连接的路径，退出该空点的循环
					}//上-终点
					else if (paths[i][m][paths[i][m].size() - 1].id == model[i][j].id + x_count) {
						model[i][j].b++;
						s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
						paths[i][m].push_back(s);
						j = 0;
						break;
					}//左-终点
					else if (paths[i][m][paths[i][m].size() - 1].id == model[i][j].id - 1 && model[i][j].id % x_count != 1) {
						model[i][j].b++;
						s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
						paths[i][m].push_back(s);
						j = 0;
						break;
					}//下-终点
					else if (paths[i][m][paths[i][m].size() - 1].id == model[i][j].id - x_count) {
						model[i][j].b++;
						s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
						paths[i][m].push_back(s);
						j = 0;
						break;
					}//右上-终点
					else if (paths[i][m][paths[i][m].size() - 1].id == model[i][j].id + 1 + x_count && model[i][j].id % x_count != 0 && Right_up == 0) {
						model[i][j].b++;
						s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
						paths[i][m].push_back(s);
						j = 0;
						break;
					}//左上-终点
					else if (paths[i][m][paths[i][m].size() - 1].id == model[i][j].id - 1 + x_count && model[i][j].id % x_count != 1 && Left_up == 0) {
						model[i][j].b++;
						s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
						paths[i][m].push_back(s);
						j = 0;
						break;
					}//左下-终点
					else if (paths[i][m][paths[i][m].size() - 1].id == model[i][j].id - 1 - x_count && model[i][j].id % x_count != 1 && Left_down == 0) {
						model[i][j].b++;
						s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
						paths[i][m].push_back(s);
						j = 0;
						break;
					}//右下-终点
					else if (paths[i][m][paths[i][m].size() - 1].id == model[i][j].id + 1 - x_count && model[i][j].id % x_count != 0 && Right_down == 0) {
						model[i][j].b++;
						s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
						paths[i][m].push_back(s);
						j = 0;
						break;
					}//右-起点
					else if (paths[i][m][0].id == model[i][j].id + 1 && model[i][j].id % x_count != 0) {
						model[i][j].b++;
						s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
						paths[i][m].push_back(s);
						for (int f = paths[i][m].size() - 1; f > 0; f--) {
							s = paths[i][m][f];
							paths[i][m][f] = paths[i][m][f - 1];
							paths[i][m][f - 1] = s;
						}
						j = 0;
						break;
					}//上-起点
					else if (paths[i][m][0].id == model[i][j].id + x_count) {
						model[i][j].b++;
						s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
						paths[i][m].push_back(s);
						for (int f = paths[i][m].size() - 1; f > 0; f--) {
							s = paths[i][m][f];
							paths[i][m][f] = paths[i][m][f - 1];
							paths[i][m][f - 1] = s;
						}
						j = 0;
						break;
					}//左-起点
					else if (paths[i][m][0].id == model[i][j].id - 1 && model[i][j].id % x_count != 1) {
						model[i][j].b++;
						s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
						paths[i][m].push_back(s);
						for (int f = paths[i][m].size() - 1; f > 0; f--) {
							s = paths[i][m][f];
							paths[i][m][f] = paths[i][m][f - 1];
							paths[i][m][f - 1] = s;
						}
						j = 0;
						break;
					}//下-起点
					else if (paths[i][m][0].id == model[i][j].id - x_count) {
						model[i][j].b++;
						s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
						paths[i][m].push_back(s);
						for (int f = paths[i][m].size() - 1; f > 0; f--) {
							s = paths[i][m][f];
							paths[i][m][f] = paths[i][m][f - 1];
							paths[i][m][f - 1] = s;
						}
						j = 0;
						break;
					}//右上-起点
					else if (paths[i][m][0].id == model[i][j].id + 1 + x_count && model[i][j].id % x_count != 0 && Right_up == 0) {
						model[i][j].b++;
						s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
						paths[i][m].push_back(s);
						for (int f = paths[i][m].size() - 1; f > 0; f--) {
							s = paths[i][m][f];
							paths[i][m][f] = paths[i][m][f - 1];
							paths[i][m][f - 1] = s;
						}
						j = 0;
						break;
					}//左上-起点
					else if (paths[i][m][0].id == model[i][j].id - 1 + x_count && model[i][j].id % x_count != 1 && Left_up == 0) {
						model[i][j].b++;
						s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
						paths[i][m].push_back(s);
						for (int f = paths[i][m].size() - 1; f > 0; f--) {
							s = paths[i][m][f];
							paths[i][m][f] = paths[i][m][f - 1];
							paths[i][m][f - 1] = s;
						}
						j = 0;
						break;
					}//左下-起点
					else if (paths[i][m][0].id == model[i][j].id - 1 - x_count && model[i][j].id % x_count != 1 && Left_down == 0) {
						model[i][j].b++;
						s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
						paths[i][m].push_back(s);
						for (int f = paths[i][m].size() - 1; f > 0; f--) {
							s = paths[i][m][f];
							paths[i][m][f] = paths[i][m][f - 1];
							paths[i][m][f - 1] = s;
						}
						j = 0;
						break;
					}//右下-起点
					else if (paths[i][m][0].id == model[i][j].id + 1 - x_count && model[i][j].id % x_count != 0 && Right_down == 0) {
					model[i][j].b++;
					s.id = model[i][j].id; s.x = model[i][j].x; s.y = model[i][j].y; s.z = model[i][j].z; s.direction = model[i][j].direction;
					paths[i][m].push_back(s);
					for (int f = paths[i][m].size() - 1; f > 0; f--) {
						s = paths[i][m][f];
						paths[i][m][f] = paths[i][m][f - 1];
						paths[i][m][f - 1] = s;
					}
					j = 0;
					break;
					}
				}
				
			}
		}
	}
} 

void PathConnect() {
	paths_buffer1.clear();
	int Right_up, Left_up, Right_down, Left_down;
	///////////////////////根据方向先进行合并（终点-起点）
	for (int i = 0; i < paths.size(); i++) {
		for (int j = 0; j < paths[i].size(); j++) {
			Right_up = 0;//判断该空点的斜连接是否可连接，可以连接为0，不可连接为1
			Left_up = 0;
			Right_down = 0;
			Left_down = 0;
			for (int m = 0; m < paths[i].size(); m++) {
				for (int n = 1; n < paths[i][m].size(); n++) {
					if ((paths[i][j][paths[i][j].size() - 1].id + 1 == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id + x_count == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 0) ||
						(paths[i][j][paths[i][j].size() - 1].id + 1 == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id + x_count == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 0)) {
						Right_up = 1;//如果与右上的端点连接，则会与已有的路径交叉
					}
					if ((paths[i][j][paths[i][j].size() - 1].id - 1 == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id + x_count == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 1) ||
						(paths[i][j][paths[i][j].size() - 1].id - 1 == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id + x_count == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 1)) {
						Left_up = 1;//如果与左上的端点连接，则会与已有的路径交叉
					}
					if ((paths[i][j][paths[i][j].size() - 1].id + 1 == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id - x_count == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 0) ||
						(paths[i][j][paths[i][j].size() - 1].id + 1 == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id - x_count == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 0)) {
						Right_down = 1;//如果与右下的端点连接，则会与已有的路径交叉
					}
					if ((paths[i][j][paths[i][j].size() - 1].id - 1 == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id - x_count == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 1) ||
						(paths[i][j][paths[i][j].size() - 1].id - 1 == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id - x_count == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 1)) {
						Left_down = 1;//如果与左下的端点连接，则会与已有的路径交叉
					}
				}
			}
			//右
			if (paths[i][j][paths[i][j].size() - 1].direction >= 0 && paths[i][j][paths[i][j].size() - 1].direction < pi / (double)8 && paths[i][j][paths[i][j].size() - 1].id % x_count != 0) {
				for (int m = 0; m < paths[i].size(); m++) {
					if (m == j) m++;
					if (m == paths[i].size()) break;
					if (paths[i][m][0].id == paths[i][j][paths[i][j].size() - 1].id + 1) {
						//将m行插入到第j行后方并删除第m行
						paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
						paths[i].erase(paths[i].begin() + m);
						m = 0; j = 0;
						break;
					}
				}
			}
			//右上
			else if (paths[i][j][paths[i][j].size() - 1].direction >= pi / (double)8 && paths[i][j][paths[i][j].size() - 1].direction < pi * ((double)3 / (double)8) && paths[i][j][paths[i][j].size() - 1].id % x_count != 0 && Right_up == 0) {
				for (int m = 0; m < paths[i].size(); m++) {
					if (m == j) m++;
					if (m == paths[i].size()) break;
					if (paths[i][m][0].id == paths[i][j][paths[i][j].size() - 1].id + 1 + x_count) {
						//将m行插入到第j行后方并删除第m行
						paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
						paths[i].erase(paths[i].begin() + m);
						m = 0; j = 0;
						break;
					}
				}
			}
			//上
			else if (paths[i][j][paths[i][j].size() - 1].direction >= pi * ((double)3 / (double)8) && paths[i][j][paths[i][j].size() - 1].direction < pi * ((double)5 / (double)8)) {
				for (int m = 0; m < paths[i].size(); m++) {
					if (m == j) m++;
					if (m == paths[i].size()) break;
					if (paths[i][m][0].id == paths[i][j][paths[i][j].size() - 1].id + x_count) {
						//将m行插入到第j行后方并删除第m行
						paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
						paths[i].erase(paths[i].begin() + m);
						m = 0; j = 0;
						break;
					}
				}
			}
			//左上
			else if (paths[i][j][paths[i][j].size() - 1].direction >= pi * ((double)5 / (double)8) && paths[i][j][paths[i][j].size() - 1].direction < pi * ((double)7 / (double)8) && paths[i][j][paths[i][j].size() - 1].id % x_count != 1 && Left_up == 0) {
				for (int m = 0; m < paths[i].size(); m++) {
					if (m == j) m++;
					if (m == paths[i].size()) break;
					if (paths[i][m][0].id == paths[i][j][paths[i][j].size() - 1].id + x_count - 1) {
						//将m行插入到第j行后方并删除第m行
						paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
						paths[i].erase(paths[i].begin() + m);
						m = 0; j = 0;
						break;
					}
				}
			}
			//左
			else if (paths[i][j][paths[i][j].size() - 1].direction >= pi * ((double)7 / (double)8) && paths[i][j][paths[i][j].size() - 1].direction < pi && paths[i][j][paths[i][j].size() - 1].id % x_count != 1) {
				for (int m = 0; m < paths[i].size(); m++) {
					if (m == j) m++;
					if (m == paths[i].size()) break;
					if (paths[i][m][0].id == paths[i][j][paths[i][j].size() - 1].id - 1) {
						//将m行插入到第j行后方并删除第m行
						paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
						paths[i].erase(paths[i].begin() + m);
						m = 0; j = 0;
						break;
					}
				}
			}
		}
	}
	///////////进行相邻路径的合并
	//起点-起点
	for (int i = 0; i < paths.size(); i++) {
		for (int j = 0; j < paths[i].size(); j++) {
			for (int m = 0; m < paths[i].size(); m++) {
				if (m == j) m++;
				if (m == paths[i].size()) break;
				//右
				if (paths[i][m][0].id == paths[i][j][0].id + 1 &&
					paths[i][j][0].id % x_count != 0) {
					reverse(paths[i][j].begin(), paths[i][j].end());//翻转
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
				//上、下
				else if (paths[i][m][0].id == paths[i][j][0].id + x_count ||
					paths[i][m][0].id == paths[i][j][0].id - x_count) {
					reverse(paths[i][j].begin(), paths[i][j].end());//翻转
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
				//左
				else if (paths[i][m][0].id == paths[i][j][0].id - 1 &&
					paths[i][j][0].id % x_count != 1) {
					reverse(paths[i][j].begin(), paths[i][j].end());//翻转
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
			}
		}
	}
	//终点-终点
	for (int i = 0; i < paths.size(); i++) {
		for (int j = 0; j < paths[i].size(); j++) {
			for (int m = 0; m < paths[i].size(); m++) {
				if (m == j) m++;
				if (m == paths[i].size()) break;
				//右
				if (paths[i][m][paths[i][m].size() - 1].id == paths[i][j][paths[i][j].size() - 1].id + 1 &&
					paths[i][j][paths[i][j].size() - 1].id % x_count != 0) {
					reverse(paths[i][m].begin(), paths[i][m].end());//翻转
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					j = 0; m = 0;
					break;
				}
				//上、下
				else if (paths[i][m][paths[i][m].size() - 1].id == paths[i][j][paths[i][j].size() - 1].id + x_count ||
					paths[i][m][paths[i][m].size() - 1].id == paths[i][j][paths[i][j].size() - 1].id - x_count) {
					reverse(paths[i][m].begin(), paths[i][m].end());//翻转
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					j = 0; m = 0;
					break;
				}
				//左
				else if (paths[i][m][paths[i][m].size() - 1].id == paths[i][j][paths[i][j].size() - 1].id - 1 &&
					paths[i][j][paths[i][j].size() - 1].id % x_count != 1) {
					reverse(paths[i][m].begin(), paths[i][m].end());//翻转
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					j = 0; m = 0;
					break;
				}
			}
		}
	}
	//终点-起点
	for (int i = 0; i < paths.size(); i++) {
		for (int j = 0; j < paths[i].size(); j++) {
			for (int m = 0; m < paths[i].size(); m++) {
				if (m == j) m++;
				if (m == paths[i].size()) break;
				//右
				if (paths[i][m][0].id == paths[i][j][paths[i][j].size() - 1].id + 1 &&
					paths[i][j][paths[i][j].size() - 1].id % x_count != 0) {
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
				//上、下
				else if (paths[i][m][0].id == paths[i][j][paths[i][j].size() - 1].id + x_count ||
					paths[i][m][0].id == paths[i][j][paths[i][j].size() - 1].id - x_count) {
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
				//左
				else if (paths[i][m][0].id == paths[i][j][paths[i][j].size() - 1].id - 1 &&
					paths[i][j][paths[i][j].size() - 1].id % x_count != 1) {
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
			}
		}
	}
	///////斜连接
	for (int i = 0; i < paths.size(); i++) {
		for (int j = 0; j < paths[i].size(); j++) {
			//起点-起点
			Right_up = 0;//判断该空点的斜连接是否可连接，可以连接为0，不可连接为1
			Left_up = 0;
			Right_down = 0;
			Left_down = 0;
			for (int m = 0; m < paths[i].size(); m++) {
				for (int n = 1; n < paths[i][m].size(); n++) {
					if ((paths[i][j][0].id + 1 == paths[i][m][n - 1].id && paths[i][j][0].id + x_count == paths[i][m][n].id && paths[i][j][0].id % x_count != 0) ||
						(paths[i][j][0].id + 1 == paths[i][m][n].id && paths[i][j][0].id + x_count == paths[i][m][n - 1].id && paths[i][j][0].id % x_count != 0)) {
						Right_up = 1;//如果与右上的端点连接，则会与已有的路径交叉
					}
					if ((paths[i][j][0].id - 1 == paths[i][m][n - 1].id && paths[i][j][0].id + x_count == paths[i][m][n].id && paths[i][j][0].id % x_count != 1) ||
						(paths[i][j][0].id - 1 == paths[i][m][n].id && paths[i][j][0].id + x_count == paths[i][m][n - 1].id && paths[i][j][0].id % x_count != 1)) {
						Left_up = 1;//如果与左上的端点连接，则会与已有的路径交叉
					}
					if ((paths[i][j][0].id + 1 == paths[i][m][n - 1].id && paths[i][j][0].id - x_count == paths[i][m][n].id && paths[i][j][0].id % x_count != 0) ||
						(paths[i][j][0].id + 1 == paths[i][m][n].id && paths[i][j][0].id - x_count == paths[i][m][n - 1].id && paths[i][j][0].id % x_count != 0)) {
						Right_down = 1;//如果与右下的端点连接，则会与已有的路径交叉
					}
					if ((paths[i][j][0].id - 1 == paths[i][m][n - 1].id && paths[i][j][0].id - x_count == paths[i][m][n].id && paths[i][j][0].id % x_count != 1) ||
						(paths[i][j][0].id - 1 == paths[i][m][n].id && paths[i][j][0].id - x_count == paths[i][m][n - 1].id && paths[i][j][0].id % x_count != 1)) {
						Left_down = 1;//如果与左下的端点连接，则会与已有的路径交叉
					}
				}
			}
			for (int m = 0; m < paths[i].size(); m++) {
				if (m == j) m++;
				if (m == paths[i].size()) break;
				//右上
				if (paths[i][m][0].id == paths[i][j][0].id + 1 + x_count && Right_up == 0 && paths[i][j][0].id%x_count!=0) {
					reverse(paths[i][j].begin(), paths[i][j].end());//翻转
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
				//左上
				else if (paths[i][m][0].id == paths[i][j][0].id - 1 + x_count && Left_up == 0 && paths[i][j][0].id % x_count != 1) {
					reverse(paths[i][j].begin(), paths[i][j].end());//翻转
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
				//左下
				else if (paths[i][m][0].id == paths[i][j][0].id - 1 - x_count && Left_down == 0&& paths[i][j][0].id % x_count != 1) {
					reverse(paths[i][j].begin(), paths[i][j].end());//翻转
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
				//右下
				else if (paths[i][m][0].id == paths[i][j][0].id + 1 - x_count && Right_down == 0 && paths[i][j][0].id % x_count != 0) {
					reverse(paths[i][j].begin(), paths[i][j].end());//翻转
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
			}
			//终点-终点
			Right_up = 0;//判断该空点的斜连接是否可连接，可以连接为0，不可连接为1
			Left_up = 0;
			Right_down = 0;
			Left_down = 0;
			for (int m = 0; m < paths[i].size(); m++) {
				for (int n = 1; n < paths[i][m].size(); n++) {
					if ((paths[i][j][paths[i][j].size() - 1].id + 1 == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id + x_count == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 0) ||
						(paths[i][j][paths[i][j].size() - 1].id + 1 == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id + x_count == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 0)) {
						Right_up = 1;//如果与右上的端点连接，则会与已有的路径交叉
					}
					if ((paths[i][j][paths[i][j].size() - 1].id - 1 == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id + x_count == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 1) ||
						(paths[i][j][paths[i][j].size() - 1].id - 1 == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id + x_count == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 1)) {
						Left_up = 1;//如果与左上的端点连接，则会与已有的路径交叉
					}
					if ((paths[i][j][paths[i][j].size() - 1].id + 1 == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id - x_count == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 0) ||
						(paths[i][j][paths[i][j].size() - 1].id + 1 == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id - x_count == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 0)) {
						Right_down = 1;//如果与右下的端点连接，则会与已有的路径交叉
					}
					if ((paths[i][j][paths[i][j].size() - 1].id - 1 == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id - x_count == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 1) ||
						(paths[i][j][paths[i][j].size() - 1].id - 1 == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id - x_count == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 1)) {
						Left_down = 1;//如果与左下的端点连接，则会与已有的路径交叉
					}
				}
			}
			for (int m = 0; m < paths[i].size(); m++) {
				if (m == j) m++;
				if (m == paths[i].size()) break;
				//右上
				if (paths[i][m][paths[i][m].size() - 1].id == paths[i][j][paths[i][j].size() - 1].id + 1 + x_count && Right_up == 0 && paths[i][j][paths[i][j].size() - 1].id%x_count!=0) {
					reverse(paths[i][m].begin(), paths[i][m].end());//翻转
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
				//左上
				else if (paths[i][m][paths[i][m].size() - 1].id == paths[i][j][paths[i][j].size() - 1].id - 1 + x_count && Left_up == 0 && paths[i][j][paths[i][j].size() - 1].id % x_count != 1) {
					reverse(paths[i][m].begin(), paths[i][m].end());//翻转
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
				//左下
				else if (paths[i][m][paths[i][m].size() - 1].id == paths[i][j][paths[i][j].size() - 1].id - 1 - x_count && Left_down == 0 && paths[i][j][paths[i][j].size() - 1].id % x_count != 1) {
					reverse(paths[i][m].begin(), paths[i][m].end());//翻转
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
				//右下
				else if (paths[i][m][paths[i][m].size() - 1].id == paths[i][j][paths[i][j].size() - 1].id + 1 - x_count && Right_down == 0 && paths[i][j][paths[i][j].size() - 1].id % x_count != 0) {
					reverse(paths[i][m].begin(), paths[i][m].end());//翻转
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
			}
			//终点-起点
			Right_up = 0;//判断该空点的斜连接是否可连接，可以连接为0，不可连接为1
			Left_up = 0;
			Right_down = 0;
			Left_down = 0;
			for (int m = 0; m < paths[i].size(); m++) {
				for (int n = 1; n < paths[i][m].size(); n++) {
					if ((paths[i][j][paths[i][j].size() - 1].id + 1 == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id + x_count == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 0) ||
						(paths[i][j][paths[i][j].size() - 1].id + 1 == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id + x_count == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 0)) {
						Right_up = 1;//如果与右上的端点连接，则会与已有的路径交叉
					}
					if ((paths[i][j][paths[i][j].size() - 1].id - 1 == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id + x_count == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 1) ||
						(paths[i][j][paths[i][j].size() - 1].id - 1 == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id + x_count == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 1)) {
						Left_up = 1;//如果与左上的端点连接，则会与已有的路径交叉
					}
					if ((paths[i][j][paths[i][j].size() - 1].id + 1 == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id - x_count == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 0) ||
						(paths[i][j][paths[i][j].size() - 1].id + 1 == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id - x_count == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 0)) {
						Right_down = 1;//如果与右下的端点连接，则会与已有的路径交叉
					}
					if ((paths[i][j][paths[i][j].size() - 1].id - 1 == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id - x_count == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 1) ||
						(paths[i][j][paths[i][j].size() - 1].id - 1 == paths[i][m][n].id && paths[i][j][paths[i][j].size() - 1].id - x_count == paths[i][m][n - 1].id && paths[i][j][paths[i][j].size() - 1].id % x_count != 1)) {
						Left_down = 1;//如果与左下的端点连接，则会与已有的路径交叉
					}
				}
			}
			for (int m = 0; m < paths[i].size(); m++) {
				if (m == j) m++;
				if (m == paths[i].size()) break;
				//右上
				if (paths[i][m][0].id == paths[i][j][paths[i][j].size() - 1].id + 1 + x_count && Right_up == 0 && paths[i][j][paths[i][j].size() - 1].id % x_count != 0) {
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
				//左上
				else if (paths[i][m][0].id == paths[i][j][paths[i][j].size() - 1].id - 1 + x_count && Left_up == 0 && paths[i][j][paths[i][j].size() - 1].id % x_count != 1) {
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
				//左下
				else if (paths[i][m][0].id == paths[i][j][paths[i][j].size() - 1].id - 1 - x_count && Left_down == 0 && paths[i][j][paths[i][j].size() - 1].id % x_count != 1) {
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
				//右下
				else if (paths[i][m][0].id == paths[i][j][paths[i][j].size() - 1].id + 1 - x_count && Right_down == 0 && paths[i][j][paths[i][j].size() - 1].id % x_count != 0) {
					paths[i][j].insert(paths[i][j].end(), paths[i][m].begin(), paths[i][m].end());
					paths[i].erase(paths[i].begin() + m);
					m = 0; j = 0;
					break;
				}
			}
		}
	}
}

void Dijkstra() {
	double arcs, s_begin, s_end;
	paths_buffer1.clear();
	for (int i = 0; i < paths.size(); i++) {
		for (int j = 0; j < paths[i].size() - 1; j++) {
			 arcs = distance(paths[i][j][paths[i][j].size() - 1], paths[i][j + 1][0]);
			for (int m = j + 1; m < paths[i].size(); m++) {
				s_begin = distance(paths[i][j][paths[i][j].size() - 1], paths[i][m][0]);
				s_end = distance(paths[i][j][paths[i][j].size() - 1], paths[i][m][paths[i][m].size() - 1]);
				if (s_begin < arcs && s_begin < s_end) {
					arcs = s_begin;
					paths_buffer1 = paths[i][m];
					paths[i][m] = paths[i][j + 1];
					paths[i][j + 1] = paths_buffer1;
					paths_buffer1.clear();
				}
				else if (s_end < arcs && s_end < s_begin) {
					arcs = s_end;
					reverse(paths[i][m].begin(), paths[i][m].end());
					paths_buffer1 = paths[i][m];
					paths[i][m] = paths[i][j + 1];
					paths[i][j + 1] = paths_buffer1;
					paths_buffer1.clear();
				}
			}
		}
	}
}

void StressPath() {
	points s;//工具
	path n;
	bool flag;
	//将所有bool为1的单元存入model中
	for (int i = 0; i < point.size(); i++) {
		for (int j = 0; j < point[0].size(); j++) {
			for (int m = 0; m < point[0][0].size(); m++) {
				if (point[i][j][m].b == 1) {
					s.id = point[i][j][m].id;
					s.x = point[i][j][m].x;
					s.y = point[i][j][m].y;
					s.z = point[i][j][m].z;
					s.s_max = point[i][j][m].s_max;
					s.direction = point[i][j][m].direction;
					model_buffer.push_back(s);
				}

			}
		}
		model.push_back(model_buffer);
		model_buffer.clear();
	}
	//按照应力（abs）的大小对其进行排序
	//采用冒泡排序
	for (int i = 0; i < model.size(); i++) {
		for (int j = 0; j < model[i].size() - 1; j++) {
			flag = false;
			for (int m = model[i].size() - 1; m > j; m--) {
				if (abs(model[i][m - 1].s_max) < abs(model[i][m].s_max)) {
					s = model[i][m - 1];
					model[i][m - 1] = model[i][m];
					model[i][m] = s;
					flag = true;
				}
			}
			if (flag == false) break;
		}
	}
	//生成应力路径
	for (int i = 0; i < model.size(); i++) {
		for (int j = 0; j < model[i].size(); j++) {
			if (model[i][j].b == 0) {
				//model[i][j].b++;//增加该单元的度数
				s = model[i][j];
				n.id = s.id;
				n.x = s.x;
				n.y = s.y;
				n.z = s.z;
				n.direction = s.direction;
				paths_buffer1.push_back(n);
				VectorInsert(i, j);
				paths_buffer2.push_back(paths_buffer1);
				paths_buffer1.clear();
			}
			
		}
		paths.push_back(paths_buffer2);
		paths_buffer2.clear();
	}
	//删除只有一个单元的路径
	for (int i = 0; i < paths.size(); i++) {
		for (int j = 0; j < paths[i].size(); j++) {
			if (paths[i][j].size() == 1) {
				paths[i].erase(paths[i].begin() + j);
				j--;//每删除一个元素，其后面的地址会向前进一个单位，所以j需要-1
			}
		}
	}
	//优化路径空点
	printf("优化空点......\n");
	OptimizePoints();
	//连续路径优化
	printf("连续路径优化......\n");
	PathConnect();
	//最短空行程优化
	Dijkstra();
	//删除同一条直线
}

void GcodePrint() {
	FILE* fp;
	errno_t err;     //判断此文件流是否存在 存在返回1
	err = fopen_s(&fp, "cura2.gcode", "a"); //若return 1 , 则将指向这个文件的文件流给
	double t1 = 0.03326;//层厚0.2，丝宽0.4
	double t2 = 0.04654;//层厚0.2，丝宽0.56
	double E = 0;
	double r;//回抽
	fprintf(fp, "M190 S50.000000\n");
	fprintf(fp, "M109 S210.000000\n");
	fprintf(fp, "G21\n");
	fprintf(fp, "G90\n");
	fprintf(fp, "M82\n");
	fprintf(fp, "M107\n");
	fprintf(fp, "G28 X0 Y0\n");
	fprintf(fp, "G28 Z0\n");
	fprintf(fp, "G1 Z15.0 F9000\n");
	fprintf(fp, "G92 E0\n");
	fprintf(fp, "G1 F200 E3\n");
	fprintf(fp, "G92 E0\n");
	fprintf(fp, "G1 F9000\n");
	fprintf(fp, "M117 Printing...\n");
	fprintf(fp, "M106 S255");
	fprintf(fp, "G1 F4800 E-6.00000\n");

	//fprintf(fp, ";LAYER:0\n");

	//fprintf(fp, "G0 F2000 Z%.1f\n", 0.1);
	//for (int i = 0; Bmin_x + 2 * i * 0.42 + 0.42 < Bmax_x; i++)  //第1、2层打实，第一层高度是0.27
	//{
	//	fprintf(fp, "G0 F2000 X%f Y%f \n", Bmin_x + 2 * i * 0.42, Bmin_y);
	//	fprintf(fp, "G1 F1000 X%f Y%f E%f\n", Bmin_x + 2 * i * 0.42, Bmax_y, E += (Bmax_y - Bmin_y) * t1);
	//	fprintf(fp, "G0 F2000 X%f Y%f \n", Bmin_x + 2 * i * 0.42 + 0.42, Bmax_y);
	//	fprintf(fp, "G1 F1000 X%f Y%f E%f\n", Bmin_x + 2 * i * 0.42 + 0.42, Bmin_y, E += (Bmax_y - Bmin_y) * t1);
	//}
	//fprintf(fp, ";LAYER:1\n");

	//fprintf(fp, "G0 F2000 Z%.1f\n", 0.3);
	//for (int i = 0; Bmin_y + 2 * i * 0.42 + 0.42 < Bmax_y; i++)  //这是第二层，二层以后都是0.2
	//{
	//	fprintf(fp, "G0 F2000 X%f Y%f \n", Bmin_x, Bmin_y + 2 * i * 0.42);
	//	fprintf(fp, "G1 F1000 X%f Y%f E%f\n", Bmax_x, Bmin_y + 2 * i * 0.42, E += (Bmax_y - Bmin_y) * t1);
	//	fprintf(fp, "G0 F2000 X%f Y%f \n", Bmax_x, Bmin_y + 2 * i * 0.42 + 0.42);
	//	fprintf(fp, "G1 F1000 X%f Y%f E%f\n", Bmin_x, Bmin_y + 2 * i * 0.42 + 0.42, E += (Bmax_y - Bmin_y) * t1);
	//}
	
	
	
	//模型第一层
	fprintf(fp, "G0 F9000 X%f Y%f Z%f\n", paths[0][0][0].x, paths[0][0][0].y, paths[0][0][0].z + 0.1);//0.52模型第一层与底座间隔0.22
	fprintf(fp, "G1 F4800 E0.00000\n");
	for (int j = 0; j < paths[0].size(); j++) {
		r = E - 6;
		fprintf(fp, "G1 F4800 E%f\n",r);//回抽一定距离，避免拉丝
		fprintf(fp, "G0 X%f Y%f\n", paths[0][j][0].x, paths[0][j][0].y);
		fprintf(fp, "G1 F4800 E%f\n", E);
		for (int m = 1; m < paths[0][j].size(); m++) {
			if (paths[0][j][m - 1].x != paths[0][j][m].x && paths[0][j][m - 1].y != paths[0][j][m].y) {
				fprintf(fp, "G1 F1200 X%f Y%f E%f\n", paths[0][j][m].x, paths[0][j][m].y, E += distance(paths[0][j][m - 1], paths[0][j][m]) * t1);
			}
			else {
				fprintf(fp, "G1 F1200 X%f Y%f E%f\n", paths[0][j][m].x, paths[0][j][m].y, E += distance(paths[0][j][m - 1], paths[0][j][m]) * t2);
			}
		}
	}

	for (int i = 1; i < paths.size(); i++) {//从第二层开始
		fprintf(fp, "G0 F9000 X%f Y%f Z%f\n", paths[i][0][0].x, paths[i][0][0].y, paths[i][0][0].z + 0.1);
		for (int j = 0; j < paths[i].size(); j++) {
			r = E - 6;
			fprintf(fp, "G1 F4800 E%f\n", r);//回抽一定距离，避免拉丝
			fprintf(fp, "G0 X%f Y%f\n", paths[i][j][0].x, paths[i][j][0].y);
			fprintf(fp, "G1 F4800 E%f\n", E);
			for (int m = 1; m < paths[i][j].size(); m++) {
				if (paths[i][j][m - 1].x != paths[i][j][m].x && paths[i][j][m - 1].y != paths[i][j][m].y) {
					
					fprintf(fp, "G1 F3000 X%f Y%f E%f\n", paths[i][j][m].x, paths[i][j][m].y, E += distance(paths[i][j][m - 1], paths[i][j][m]) * t1);
				}
				else {
					
					fprintf(fp, "G1 F3000 X%f Y%f E%f\n", paths[i][j][m].x, paths[i][j][m].y, E += distance(paths[i][j][m - 1], paths[i][j][m]) * t2);
				}
			}
		}
	}
	fprintf(fp, "M104 S0\n");
	fprintf(fp, "M140 S0\n");
	fprintf(fp, "G91\n");
	fprintf(fp, "G1 E-1 F300\n");
	fprintf(fp, "G1 Z+0.5 E-5 X-20 Y-20 F9000\n");
	fprintf(fp, "G28 X0 Y0\n");
	fprintf(fp, "M84\n");
	fprintf(fp, "G90\n");
	fprintf(fp, "M81\n");
}

void main(int argc, char** argv) {
	printf("正在读取文件........\n");
	ReadFile();
	printf("读取成功！\n");
	printf("正常计算最大主应力.......\n");
	FE_Analysis();
	printf("生成路径中........\n");
	StressPath();
	printf("路径生成成功！\n");
	GcodePrint();
	int s1, s2, s3, s4, s5;
	s1 = 0; s2 = 0; s3 = 0; s4 = 0; s5 = 0;
	for (int i = 0; i < point.size(); i++) {
		for (int j = 0; j < point[0].size(); j++) {
			for (int m = 0; m < point[0][0].size(); m++) {
				if (point[i][j][m].direction > 0 && point[i][j][m].direction < pi / 8) s1++;
				else if (point[i][j][m].direction >= (double)pi / (double)8 && point[i][j][m].direction < (double)pi * (double)((double)3 / (double)8)) s2++;
				else if (point[i][j][m].direction >= (double)pi * (double)((double)3 / (double)8) && point[i][j][m].direction < (double)pi * (double)((double)5 / (double)8)) s3++;
				else if (point[i][j][m].direction >= (double)pi * (double)((double)5 / (double)8) && point[i][j][m].direction < (double)pi * (double)((double)7 / (double)8)) s4++;
				else if (point[i][j][m].direction >= (double)pi * (double)((double)7 / (double)8) && point[i][j][m].direction < (double)pi) s5++;
				//printf("%d %f %f %f %d %f %f %f %f %f\n", point[i][j][m].id, point[i][j][m].x, point[i][j][m].y, point[i][j][m].z, point[i][j][m].b, point[i][j][m].s11, point[i][j][m].s22, point[i][j][m].s12, point[i][j][m].s_max, point[i][j][m].direction);
			}
		}
	}
	printf("%d %d %d %d %d\n", s1, s2, s3, s4,s5);
	int s=0;
	for (int i = 0; i < model.size(); i++) {
		for (int j = 0; j < model[i].size(); j++) {
			if (model[i][j].b == 0) {
				s++;
			}
		}
	}
	printf("第1层总共有%d条路径,空点数量为%d\n", paths[0].size(),s);
	/*for (int i = 0; i < model[0].size(); i++) {
		printf("%d %f\n",model[0][i].id, model[0][i].s_max);
	}*/
	//for (int i = 0; i < paths.size(); i++) {
		/*for (int j = 0; j < paths[0].size(); j++) {
			for (int m = 0; m < paths[0][j].size(); m++) {
				printf("%d\n",paths[0][j][m].id);
			}
			printf("*************\n");
		}
	printf("%d\n", paths[0].size());*/
	//}
	//优化空点检验
	/*int n=0;
	for (int j = 0; j < model[0].size(); j++) {
		if (model[0][j].b == 0) {
			n++;
		}
	}
	printf("%d\n", n);*/

}
