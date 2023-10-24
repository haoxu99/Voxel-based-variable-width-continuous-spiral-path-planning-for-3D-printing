#include <iostream>  
#include <OpenMesh/Core/IO/MeshIO.hh>  
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>  
#include <OpenMesh/Core/Mesh/Handles.hh>
#include <OpenMesh\Core\Utils\Property.hh> 
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include<stdio.h>
#include "clipper.hpp"  
#include"clipper.cpp"
#include<math.h>
#define use_int32

struct point {

	double x, y;

}p, p1;
struct voxelization {//体素点结构体
	double x, y;
	bool b;
};
struct node {
	int id;
	float x, y, z;
};
struct element {
	int n1, n2, n3, n4, n5, n6, n7, n8;
	int id;
};

using namespace std;

typedef unsigned char boolean;

vector<point>coord;  //缓存一层的轮廓线
vector<point>polypoint; //一层的轮廓线坐标
vector<voxelization>v_coord;//缓存一层的体素质心坐标
vector<vector<voxelization>>v_coord1;//缓存二维体素质心坐标
vector<vector<voxelization>>v_point;//体素点,一维层数，二维此层体素质心点坐标
vector<point>tripoint;  //一层的变化线 线段坐标 成对
vector<point>new_tripoint;  //新的一层的变化线 线段坐标 成对
vector<point>interpoint; //交点坐标
vector<vector<point>>model; //slice  整个模型的，分层的轮廓线坐标，一维层数，二维此层的轮廓线点坐标
vector<vector<point>>model2; //slice  整个模型的，分层的轮廓线坐标，一维层数，二维此层的轮廓线点坐标

vector<vector<vector<voxelization>>>a_point;//缓存三维的质心，用于Abaqus
vector<node>node1;
vector<vector<node>>node2;
vector<vector<vector<node>>>node3;
vector<element>a_element;
vector<element>b_element;



const string file_1 = "test1.STL";//读取文件

typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;//定义完整MyMesh类，用三角网格，自定的属性
MyMesh mesh;

//用于包围盒
double Bmax_x, Bmax_y, Bmax_z, Bmin_x, Bmin_y, Bmin_z, px, py, pz;
int countt = 0;
int y_count = 0;//计算y轴元素的个数

// 读取文件的函数
void readfile(string file) {//文件读取到了mesh当中
	// 请求顶点法线 vertex normals
	mesh.request_vertex_normals();
	//如果不存在顶点法线，则报错 
	if (!mesh.has_vertex_normals())
	{
		cout << "错误：标准定点属性 “法线”不存在" << endl;
		return;
	}
	// 如果有顶点发现则读取文件 
	OpenMesh::IO::Options opt;
	if (!OpenMesh::IO::read_mesh(mesh, file, opt))
	{
		cout << "无法读取文件:" << file << endl;
		return;
	}
	else cout << "成功读取文件:" << file << endl;
	cout << endl; // 为了ui显示好看一些
				  //如果不存在顶点法线，则计算出
	if (!opt.check(OpenMesh::IO::Options::VertexNormal))
	{
		// 通过面法线计算顶点法线
		mesh.request_face_normals();
		// mesh计算出顶点法线
		mesh.update_normals();
		// 释放面法线
		mesh.release_face_normals();
	}
}

//通过遍历点获取模型的包围盒
void BoundingBox() {
	MyMesh::Point pt;
	int st = 0;
	for (auto it = mesh.vertices_begin(); it != mesh.vertices_end(); ++it) {
		MyMesh::VertexHandle vh_i = *it;
		pt = mesh.point(vh_i);
		px = pt.data()[0];
		py = pt.data()[1];
		pz = pt.data()[2];
		if (st == 0) {
			Bmax_x = Bmin_x = px;
			Bmax_y = Bmin_y = py;
			Bmax_z = Bmin_z = pz;
			st++;
		}
		else {
			if (px > Bmax_x)Bmax_x = px; else if (px < Bmin_x)Bmin_x = px;
			if (py > Bmax_y)Bmax_y = py; else if (py < Bmin_y)Bmin_y = py;
			if (pz > Bmax_z)Bmax_z = pz; else if (pz < Bmin_z)Bmin_z = pz;
		}
	}
}

//截面函数
bool IntersectPlane(MyMesh::Point pt, MyMesh::Point pnorm) //pt截面上一点，pnorm截面法向
{
	const float ERR = 0.001;
	//参数包括 pt，pnorm，*pilist，pnum[]  具体函数原理 见 截面算法.docx
	int starte, ne, ne1, nf;
	MyMesh::Point vt1, vt2;
	//MyMesh::Face f1;
	MyMesh::HalfedgeHandle nhe;
	MyMesh::FaceHandle nhf;
	float d1, d2, sd1, sd2;
	bool* flag, suc;
	float dist, mind = 1.0e+8;

	sd1 = sd2 = -10000;
	int esize = mesh.n_halfedges();
	flag = new bool[esize];

	suc = false;


	for (MyMesh::HalfedgeIter it = mesh.halfedges_begin(); it != mesh.halfedges_end(); ++it) //遍历整个模型所有的边，有交点的把id记录在flag中
	{

		MyMesh::HalfedgeHandle hh = *it;
		int id = hh.idx();
		flag[id] = false;

		auto fromVertex = mesh.from_vertex_handle(hh);
		auto toVertex = mesh.to_vertex_handle(hh);
		vt1 = mesh.point(fromVertex);
		vt2 = mesh.point(toVertex);
		//printf("$ %.3f %.3f $\n", vt1.data()[0],vt2.data()[0]);
		d1 = pnorm.data()[0] * (vt1.data()[0] - pt.data()[0]) + pnorm.data()[1] * (vt1.data()[1] - pt.data()[1])
			+ pnorm.data()[2] * (vt1.data()[2] - pt.data()[2]);//d1到面的距离
		d2 = pnorm.data()[0] * (vt2.data()[0] - pt.data()[0]) + pnorm.data()[1] * (vt2.data()[1] - pt.data()[1])
			+ pnorm.data()[2] * (vt2.data()[2] - pt.data()[2]);

		if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0) || d1 > 0 && d2 == 0 || d1 == 0 && d2 > 0))//线段与面相交
		{
			flag[id] = true;

			vt1.data()[0] = vt1.data()[0] - pt.data()[0];
			vt1.data()[1] = vt1.data()[1] - pt.data()[1];
			vt1.data()[2] = vt1.data()[2] - pt.data()[2];       // point date minus point date 
			dist = vt1.data()[0] * vt1.data()[0] + vt1.data()[1] * vt1.data()[1] + vt1.data()[2] * vt1.data()[2];

			if (dist < mind)
			{
				nhe = hh;	//最短边
				mind = dist;//最短距离
				ne = id;    //最短所在边的编号               //  printf("ne:  %d  \n", ne);
				suc = true;
			}
		}
	}

	if (!suc)
	{
		delete[]flag;
		return false; //没有交点，这里return false，跳出整个函数
	}

	starte = ne;//标记循环起始的边

	suc = false;

	nhf = mesh.face_handle(nhe);//最短边所在面

	while (!suc)
	{
		//printf("%%%%");	

		auto fromVertex = mesh.from_vertex_handle(nhe);
		auto toVertex = mesh.to_vertex_handle(nhe);

		vt1 = mesh.point(fromVertex);
		vt2 = mesh.point(toVertex);

		d1 = pnorm.data()[0] * (vt1.data()[0] - pt.data()[0]) + pnorm.data()[1] * (vt1.data()[1] - pt.data()[1])
			+ pnorm.data()[2] * (vt1.data()[2] - pt.data()[2]);
		d2 = pnorm.data()[0] * (vt2.data()[0] - pt.data()[0]) + pnorm.data()[1] * (vt2.data()[1] - pt.data()[1])
			+ pnorm.data()[2] * (vt2.data()[2] - pt.data()[2]);
		//printf("$$$%lf %lf \n", d1, d2);
		if ((sd1 == d1) && (sd2 == d2))
		{
			flag[ne] = false;
		}
		sd1 = d1; sd2 = d2;
		//pilist[num].data()[0] = (float)fabs(d1) / ((float)fabs(d1) + (float)fabs(d2))*(vt2.data()[0] - vt1.data()[0]) + vt1.data()[0];
		//pilist[num].data()[1] = (float)fabs(d1) / ((float)fabs(d1) + (float)fabs(d2))*(vt2.data()[1] - vt1.data()[1]) + vt1.data()[1];
		//pilist[num].data()[2] = (float)fabs(d1) / ((float)fabs(d1) + (float)fabs(d2))*(vt2.data()[2] - vt1.data()[2]) + vt1.data()[2];

		p = { (float)fabs(d1) / ((float)fabs(d1) + (float)fabs(d2)) * (vt2.data()[0] - vt1.data()[0]) + vt1.data()[0] ,(float)fabs(d1) / ((float)fabs(d1) + (float)fabs(d2)) * (vt2.data()[1] - vt1.data()[1]) + vt1.data()[1] };
		coord.push_back(p);


		do {
			for (auto it = mesh.fh_begin(nhf); it != mesh.fh_end(nhf); ++it) //nhf最短边所在面,迭代这个面的边，只有3个
			{
				MyMesh::HalfedgeHandle halfnow = *it;

				const int ne1 = halfnow.idx();

				if (flag[ne1] == false || ne == ne1) continue;

				MyMesh::VertexHandle fromV = mesh.from_vertex_handle(halfnow);
				MyMesh::VertexHandle toV = mesh.to_vertex_handle(halfnow);

				vt1 = mesh.point(fromV);
				vt2 = mesh.point(toV);

				d1 = pnorm.data()[0] * (vt1.data()[0] - pt.data()[0]) + pnorm.data()[1] * (vt1.data()[1] - pt.data()[1])
					+ pnorm.data()[2] * (vt1.data()[2] - pt.data()[2]);
				d2 = pnorm.data()[0] * (vt2.data()[0] - pt.data()[0]) + pnorm.data()[1] * (vt2.data()[1] - pt.data()[1])
					+ pnorm.data()[2] * (vt2.data()[2] - pt.data()[2]);

				p = { (float)fabs(d1) / ((float)fabs(d1) + (float)fabs(d2)) * (vt2.data()[0] - vt1.data()[0]) + vt1.data()[0] ,(float)fabs(d1) / ((float)fabs(d1) + (float)fabs(d2)) * (vt2.data()[1] - vt1.data()[1]) + vt1.data()[1] };
				coord.push_back(p);

				MyMesh::HalfedgeHandle halfnext = mesh.opposite_halfedge_handle(halfnow);//获取反向半边

				nhf = mesh.face_handle(halfnext);//返回这个边所在的面

				int ne2 = halfnext.idx();

				flag[ne1] = flag[ne2] = false;//ne1,ne2是对向的两个半边，存过其中一个的交点就都变为false

				if (nhf.idx() == -1)
				{
					starte = ne;
					flag[ne] = false;
					break;
				}
				ne = ne2;//以对边的那个面再找下一个与面相交的线

				//pilist[num].data()[0] = (float)fabs(d1) / ((float)fabs(d1) + (float)fabs(d2))*(vt2.data()[0] - vt1.data()[0]) + vt1.data()[0];
				//pilist[num].data()[1] = (float)fabs(d1) / ((float)fabs(d1) + (float)fabs(d2))*(vt2.data()[1] - vt1.data()[1]) + vt1.data()[1];
				//pilist[num].data()[2] = (float)fabs(d1) / ((float)fabs(d1) + (float)fabs(d2))*(vt2.data()[2] - vt1.data()[2]) + vt1.data()[2];
				//printf("##%lf %lf %lf\n", pilist[num].data()[0], pilist[num].data()[1], pilist[num].data()[2]);

				break;
			}
		} while (ne != starte);

		suc = true;

		for (auto it = mesh.halfedges_begin(); it != mesh.halfedges_end(); ++it) //检索有没有第二个环
		{

			MyMesh::HalfedgeHandle hh = *it;

			int id = hh.idx();

			if (flag[id] == true)
			{
				ne = id;
				starte = ne;
				nhe = hh;
				nhf = mesh.face_handle(nhe);
				if (nhf.idx() == -1)
				{
					flag[ne] = false;
					continue;
				}
				//pilist[num].data()[0] = -10000;
				//pilist[num].data()[1] = -10000;
				//pilist[num].data()[2] = -10000;
				p = { -10000,-10000 };//两个环中间的间隔数据
				coord.push_back(p);

				suc = false;
				break;
			}
		}


	};

	delete[]flag;
	return true;
}

void findIntersect(float z)
{
	for (double i = Bmin_z+z/2; i < Bmax_z; i += z)
	{
		MyMesh::Normal vf(0, 0, 1);//截面法向
		MyMesh::Point pt; //截面上一点
		pt.data()[0] = 0; pt.data()[1] = 0; pt.data()[2] = i;
		IntersectPlane(pt, vf);//生成一层轮廓线，存入coord

		model.push_back(coord);
		coord.clear();
	}
	countt = model.size();
}

void InitVoxelization(float x,float y,float z) {//创建包围盒体素点
	voxelization v;
	double v_x, v_y, v_z;
	v_x = Bmin_x + x/2;
	v_y = Bmin_y + y/2;
	v_z = Bmin_z + z/2;
	for (v_y; v_y < Bmax_y; v_y += y) {
		v_x = Bmin_x + x / 2;
		for (v_x; v_x < Bmax_x; v_x += x) {
			v.x = v_x;
			v.y = v_y;
			v.b = false;
			v_coord.push_back(v);
		}
		y_count++;
	}
	for (v_z; v_z < Bmax_z; v_z += z) {
		v_point.push_back(v_coord);
	}
	v_coord.clear();
	printf("包围盒大小：x=%.1f,y=%.1f,z=%.1f\n", v_point[0].size() / y_count * x, y_count * y, v_point.size() * z);
}

bool InOrOutPolygon(voxelization a) {//判断点是否在多边形内，在true，否false，使用的是引射线法
	double x0 = a.x;
	double y0 = a.y;
	int crossings = 0;
	int n = polypoint.size();
	for (int i = 0; i < n; i++)
	{
		// 点在两个x之间 且以点垂直y轴向上做射线
		double slope = (polypoint[(i + 1 + n) % n].y - polypoint[i].y) / (polypoint[(i + 1 + n) % n].x - polypoint[i].x);
		boolean cond1 = (polypoint[i].x <= x0) && (x0 < polypoint[(i + 1 + n) % n].x);
		boolean cond2 = (polypoint[(i + 1 + n) % n].x <= x0) && (x0 < polypoint[i].x);
		boolean above = (y0 < slope* (x0 - polypoint[i].x) + polypoint[i].y);
		if ((cond1 || cond2) && above) crossings++;
	}
	if (crossings % 2 != 0) {
		return true;
	}
	else {
		return false;
	}
}

void intersection() {//获得在多边形内的体素点
	for (int i = 0; i < v_point.size(); i++) {
		polypoint.clear();//把上层的缓存线删除
		polypoint = model[i];//把第i层的线段坐标进行缓存
		for (int j = 0; j < v_point[i].size(); j++) {
			v_point[i][j].b = InOrOutPolygon(v_point[i][j]);//判断此体素点是否在多边形内
		}
	}
}

void HoudiniOutput(float z) {//生成Houdini体素坐标，可在Houdini中查看体素后模型
	FILE* ho;
	errno_t err;
	err = fopen_s(&ho, "HoudiniOutput.obj", "a");
	float t = z / 2;
	for (int i = 0; i < v_point.size(); i++) {
		for (int j = 0; j < v_point[i].size(); j++) {
			if (v_point[i][j].b == true) {
				fprintf(ho, "v %f %f %f\n", v_point[i][j].x, v_point[i][j].y, t);
			}
		}
		t += z;
	}
	//实验用
	//for (int i = 0; i < a_point.size(); i++) {
	//	for (int j = 0; j < a_point[i].size(); j++) {
	//		for (int s = 0; s < a_point[i][j].size(); s++) {
	//			fprintf(ho, "v %f %f %f\n", a_point[i][j][s].x, a_point[i][j][s].y, t);
	//		}
	//	}
	//	t += z;
	//}
	fclose(ho);
}

//初始化编号
void InitNode(float v_x, float v_y, float v_z) {
	node n;
	int m = 0;
	//生成三维的单元质心坐标
	for (int i = 0; i < v_point.size(); i++) {
		m = 0;
		for (int j = 0; j < y_count; j++) {
			for (int s = 0; s < v_point[i].size() / y_count; s++) {
				v_coord.push_back(v_point[i][m]);
				m++;
			}
			v_coord1.push_back(v_coord);
			v_coord.clear();
		}
		a_point.push_back(v_coord1);
		v_coord.clear();
	}
	//正式开始计算编号
	int id = 0;
	double z, x, y;
	z = Bmin_z;
	for (int i = 0; i <= a_point.size(); i++) {
		y = Bmin_y;
		for (int j = 0; j <= a_point[0].size(); j++) {
			x = Bmin_x;
			for (int m = 0; m <= a_point[0][0].size(); m++) {
				id++;
				n.id = id; n.x = x; n.y = y; n.z = z;
				node1.push_back(n);
				x += v_x;
			}
			node2.push_back(node1);
			node1.clear();
			y += v_y;
		}
		node3.push_back(node2);
		node2.clear();
		z += v_z;
	}


}

void InitElement() {
	element e;
	int n = 0;
	for (int i = 0; i < node3.size()-1; i++) {
		for (int j = 0; j < node3[0].size()-1; j++) {
			for (int m = 0; m < node3[0][0].size()-1; m++) {
				n++;
				e.id = n; 
				e.n1 = node3[i][j][m].id;
				e.n2 = node3[i][j][m + 1].id;
				e.n3 = node3[i][j + 1][m + 1].id;
				e.n4 = node3[i][j + 1][m].id;
				e.n5 = node3[i + 1][j][m].id;
				e.n6 = node3[i + 1][j][m + 1].id;
				e.n7 = node3[i + 1][j + 1][m + 1].id;
				e.n8 = node3[i + 1][j + 1][m].id;
				if (a_point[i][j][m].b == true) {//不输入不在模型内的体素，但单元号数值在累加
					a_element.push_back(e);
				}
			}
		}
	}
	n = 0;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < node3[0].size() - 1; j++) {
			for (int m = 0; m < node3[0][0].size() - 1; m++) {
				n++;
				e.id = n;
				e.n1 = node3[i][j][m].id;
				e.n2 = node3[i][j][m + 1].id;
				e.n3 = node3[i][j + 1][m + 1].id;
				e.n4 = node3[i][j + 1][m].id;
				e.n5 = node3[i + 1][j][m].id;
				e.n6 = node3[i + 1][j][m + 1].id;
				e.n7 = node3[i + 1][j + 1][m + 1].id;
				e.n8 = node3[i + 1][j + 1][m].id;
				if (a_point[i][j][m].b == true) {//不输入不在模型内的体素，但单元号数值在累加
					b_element.push_back(e);
				}
			}
		}
	}
}
//导出inp文件
void InpOutput() {
	FILE* io;
	errno_t err;
	err = fopen_s(&io, "AbaqusOutput.inp", "a");
	fprintf(io, "*Heading\n");
	fprintf(io, "*Preprint, echo=NO, model=NO, history=NO\n");
	fprintf(io, "*Part, name=test1\n");
	fprintf(io, "*Node\n");
	for (int i = 0; i < node3.size(); i++) {
		for (int j = 0; j < node3[i].size(); j++) {
			for (int m = 0; m < node3[i][j].size(); m++) {
				fprintf(io, "%d, %f, %f, %f\n", node3[i][j][m].id, node3[i][j][m].x, node3[i][j][m].y, node3[i][j][m].z);
			}
		}
	}
	fprintf(io, "*Element, TYPE=C3D8R\n");
	for (int i = 0; i < a_element.size(); i++) {
		fprintf(io, "%d, %d,%d,%d,%d,%d,%d,%d,%d\n", a_element[i].id, a_element[i].n1, a_element[i].n2, a_element[i].n3, a_element[i].n4, a_element[i].n5, a_element[i].n6, a_element[i].n7, a_element[i].n8);
	}
	fprintf(io, "*End Part\n");
	fclose(io);
	
	/////仿真
	FILE* si;
	err = fopen_s(&si, "simulation.inp", "a");
	fprintf(si, "*Heading\n");
	fprintf(si, "*Preprint, echo=NO, model=NO, history=NO\n");
	fprintf(si, "*Part, name=test1\n");
	fprintf(si, "*Node\n");
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < node3[i].size(); j++) {
			for (int m = 0; m < node3[i][j].size(); m++) {
				fprintf(si, "%d, %f, %f, %f\n", node3[i][j][m].id, node3[i][j][m].x, node3[i][j][m].y, node3[i][j][m].z);
			}
		}
	}
	fprintf(si, "*Element, TYPE=C3D8R\n");
	for (int i = 0; i < b_element.size(); i++) {
		fprintf(si, "%d, %d,%d,%d,%d,%d,%d,%d,%d\n", b_element[i].id, b_element[i].n1, b_element[i].n2, b_element[i].n3, b_element[i].n4, b_element[i].n5, b_element[i].n6, b_element[i].n7, b_element[i].n8);
	}
	fprintf(si, "*End Part\n");
	fclose(si);
}

void DataOutput(float v_z) {//输出体素数据
	int id = 0;
	float z = v_z / 2;//体素质点为体素高的一半
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, "VoxelizationData.txt", "a");
	fprintf(fp, "%d %d %d\n", v_point[0].size() / y_count, y_count, v_point.size());//分别分x,y,z的体素个数
	//fprintf(fp, "%f %f %f %f\n", Bmin_x, Bmax_x, Bmin_y, Bmax_y);//包围盒的长宽，用于生成Gcode代码的底盘
	for (int i = 0; i < v_point.size(); i++) {
		if (i != 0) {
			z += v_z;
		}
		for (int j = 0; j < v_point[0].size(); j++) {
			id++;
			fprintf(fp, "%d %f %f %f %d\n", id, v_point[i][j].x, v_point[i][j].y, z, v_point[i][j].b);//单元号，x,y,z的坐标，bool值
		}
	}
	fclose(fp);
}

void main(int argc, char** argv) {
	double z = 0.2;//层厚
	double v_x = 0.2;//体素大小
	double v_y = 0.2;
	double v_z = 0.2;
	//************************************************************************切片
	printf("slicing...\n");
	readfile(file_1);
	BoundingBox();
	findIntersect(v_z);  //前三个函数切片，产生model
	printf("slice complete,layer count: %d\n", countt);
	//************************************************************************体素化
	printf("Voxelizating...\n");
	InitVoxelization(v_x,v_y,v_z);//创建体素化包围盒
	intersection();//体素化模型
	DataOutput(v_z);
	printf("Voxelization complete!\n");
	//************************************************************************导出Houdini
	printf("Building Houdini...\n");
	HoudiniOutput(v_z);//生成Houdini体素坐标，可在Houdini中查看体素后模型
	printf("Houdini complete!\n");
	//************************************************************************导出Abaqus.inp
	//printf("Building Abaqus.inp...\n");
	//InitNode(v_x, v_y, v_z);
	//InitElement();
	//InpOutput();
	//printf("Abaqus.inp complete!\n");
}