

void Init_Element() {//生成模型单元
	points t;
	
	for (int m = 0; m < point.size(); m++) {
		for (int i = 0; i < point[m].size(); i++) {
			for (int j = 0; j < point[m][i].size(); j++) {
				if (point[m][i][j].b == 1) {
					t.id = point[m][i][j].id;
					t.x = point[m][i][j].x;
					t.y = point[m][i][j].y;
					t.z = point[m][i][j].z;
					t.b = 0; 
					model_buffer.push_back(t);
				}
			}
		}
		model.push_back(model_buffer);
		model_buffer.clear();
	}
}


void VectorInsert(int i, int j) {//生成单路径函数
	bool flag = true;
	path s;
	int q = 0;
	while (flag) {
		int t = -1;
		int n=-1;
		flag = false;
		if (iol == 1 && t != 1) {
			if (t != 1) {
				for (int m = 0; m < model[i].size(); m++) {//down
					if (model[i][m].id == model[i][j].id - x_count && model[i][m].b == 0) {
						n = m;
						t = 1;
						break;
					}
				}
			}
			if (t != 1) {
				for (int m = 0; m < model[i].size(); m++) {//down-right
					if (model[i][m].id == model[i][j].id + 1 - x_count && model[i][j].id % x_count != 0 && model[i][m].b == 0) {
						n = m;
						t = 1;
						break;
					}
				}
			}
			if (t != 1) {
				for (int m = 0; m < model[i].size(); m++) {//right
					if (model[i][m].id == model[i][j].id + 1 && model[i][j].id % x_count != 0 && model[i][m].b == 0) {
						n = m;
						t = 1;
						break;
					}
				}
			}
			if (t != 1) {
				for (int m = 0; m < model[i].size(); m++) {//right-up
					if (model[i][m].id == model[i][j].id + 1 + x_count && model[i][j].id % x_count != 0 && model[i][m].b == 0) {
						n = m;
						t = 1;
						break;
					}
				}
			}
			if (t != 1) {
				for (int m = 0; m < model[i].size(); m++) {//up
					if (model[i][m].id == model[i][j].id + x_count && model[i][m].b == 0) {
						n = m;
						t = 1;
						break;
					}
				}
			}
			if (t != 1) {
				iol = 2;
			}
		}

		if (iol == 2 && t != 1) {
			if (t != 1) {
				for (int m = 0; m < model[i].size(); m++) {//up
					if (model[i][m].id == model[i][j].id + x_count && model[i][m].b == 0) {
						n = m;
						t = 1;
						break;
					}
				}
			}
			if (t != 1) {
				for (int m = 0; m < model[i].size(); m++) {//up-left
					if (model[i][m].id == model[i][j].id - 1 + x_count && model[i][j].id % x_count != 1 && model[i][m].b == 0) {
						n = m;
						t = 1;
						break;
					}
				}
			}
			if (t != 1) {
				for (int m = 0; m < model[i].size(); m++) {//left
					if (model[i][m].id == model[i][j].id - 1 && model[i][j].id % x_count != 1 && model[i][m].b == 0) {
						n = m;
						t = 1;
						break;
					}
				}
			}
			if (t != 1) {
				for (int m = 0; m < model[i].size(); m++) {//left-down
					if (model[i][m].id == model[i][j].id - 1 - x_count && model[i][j].id % x_count != 1 && model[i][m].b == 0) {
						n = m;
						t = 1;
						break;
					}
				}
			}
			if (t != 1) {
				for (int m = 0; m < model[i].size(); m++) {//down
					if (model[i][m].id == model[i][j].id - x_count && model[i][m].b == 0) {
						n = m;
						t = 1;
						break;
					}
				}
			}
			if (t != 1) {
				iol = 1;
			}
		}

		if (iol == 1 && t != 1) {
			if (t != 1) {
				for (int m = 0; m < model[i].size(); m++) {//down
					if (model[i][m].id == model[i][j].id - x_count && model[i][m].b == 0) {
						n = m;
						t = 1;
						break;
					}
				}
			}
			if (t != 1) {
				for (int m = 0; m < model[i].size(); m++) {//down-right
					if (model[i][m].id == model[i][j].id + 1 - x_count && model[i][j].id % x_count != 0 && model[i][m].b == 0) {
						n = m;
						t = 1;
						break;
					}
				}
			}
			if (t != 1) {
				for (int m = 0; m < model[i].size(); m++) {//right
					if (model[i][m].id == model[i][j].id + 1 && model[i][j].id % x_count != 0 && model[i][m].b == 0) {
						n = m;
						t = 1;
						break;
					}
				}
			}
			if (t != 1) {
				for (int m = 0; m < model[i].size(); m++) {//right-up
					if (model[i][m].id == model[i][j].id + 1 + x_count && model[i][j].id % x_count != 0 && model[i][m].b == 0) {
						n = m;
						t = 1;
						break;
					}
				}
			}
			if (t != 1) {
				for (int m = 0; m < model[i].size(); m++) {//up
					if (model[i][m].id == model[i][j].id + x_count && model[i][m].b == 0) {
						n = m;
						t = 1;
						break;
					}
				}
			}
			if (t != 1) {
				iol = 2;
			}
		}


		if (t != 1) {
			iol = 1;
		}
		if (t > -1) {
			//printf("1\n");
			model[i][n].b++;
			model[i][j].b++;
			j = n;
			s.id = model[i][n].id;
			s.x = model[i][n].x;
			s.y = model[i][n].y;
			s.z = model[i][n].z;
			paths_buffer1.push_back(s);
			flag = true;
		}
	}
}

void tool_paths() {
	points s;//工具
	path n;
	bool flag;
	Init_Element();
	for (int i = 0; i < model.size(); i++) {
		for (int j = 0; j < model[i].size(); j++) {
			if (model[i][j].b == 0) {
				//model[i][j].b++;//增加该单元的度数
				s = model[i][j];
				n.id = s.id;
				n.x = s.x;
				n.y = s.y;
				n.z = s.z;
				paths_buffer1.push_back(n);
				VectorInsert(i, j);
				paths_buffer2.push_back(paths_buffer1);
				paths_buffer1.clear();
			}

		}
		printf("%.2lf%%\r", i * 100.0 / model.size());
		paths.push_back(paths_buffer2);
		paths_buffer2.clear();
	}
}

void double_path() {
	path p;
	int t = 0;//用于判断起点和终点
	paths_buffer1.clear();
	paths_buffer2.clear();
	paths_buffer3.clear();
	for (int i = 0; i < paths.size(); i++) {
		for (int j = 0; j < paths[i].size(); j++) {
			for (int n = 0; n < 2; n++) {
				if (n == 1) {
					reverse(paths[i][j].begin(), paths[i][j].end());
				}
				for (int m = 0; m < paths[i][j].size(); m++) {
					if (m == 0) {//起点
						if (paths[i][j][m + 1].id == paths[i][j][m].id - x_count) {//上->下
							p.x = paths[i][j][m].x - d1 * 0.5;
							p.y = paths[i][j][m].y + d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x - d1 * 0.5;
							p.y = paths[i][j][m].y;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if (paths[i][j][m + 1].id == paths[i][j][m].id + x_count) {//下->上
							p.x = paths[i][j][m].x + d1 * 0.5;
							p.y = paths[i][j][m].y - d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x + d1 * 0.5;
							p.y = paths[i][j][m].y;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if (paths[i][j][m + 1].id == paths[i][j][m].id + 1) {//左->右
							p.x = paths[i][j][m].x - d1 * 0.5;
							p.y = paths[i][j][m].y - d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x;
							p.y = paths[i][j][m].y - d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);

						}
						else if (paths[i][j][m + 1].id == paths[i][j][m].id - 1) {//右->左
							p.x = paths[i][j][m].x + d1 * 0.5;
							p.y = paths[i][j][m].y + d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x;
							p.y = paths[i][j][m].y + d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if (paths[i][j][m + 1].id == paths[i][j][m].id - x_count + 1) {//左上->右下
							p.x = paths[i][j][m].x - sqrt(2) * d2 * 0.5;
							p.y = paths[i][j][m].y;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x - (d2 * 0.5) * (sqrt(2) * 0.5);
							p.y = paths[i][j][m].y - (d2 * 0.5) * (sqrt(2) * 0.5);
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if (paths[i][j][m + 1].id == paths[i][j][m].id + x_count - 1) {//右下->左上
							p.x = paths[i][j][m].x + sqrt(2) * d2 * 0.5;
							p.y = paths[i][j][m].y;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x + (d2 * 0.5) * (sqrt(2) * 0.5);
							p.y = paths[i][j][m].y + (d2 * 0.5) * (sqrt(2) * 0.5);
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if (paths[i][j][m + 1].id == paths[i][j][m].id - x_count - 1) {//右上->左下
							p.x = paths[i][j][m].x;
							p.y = paths[i][j][m].y + sqrt(2) * d2 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x - (d2 * 0.5) * (sqrt(2) * 0.5);
							p.y = paths[i][j][m].y + (d2 * 0.5) * (sqrt(2) * 0.5);
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if (paths[i][j][m + 1].id == paths[i][j][m].id + x_count + 1) {//左下->右上
							p.x = paths[i][j][m].x;
							p.y = paths[i][j][m].y - sqrt(2) * d2 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x + (d2 * 0.5) * (sqrt(2) * 0.5);
							p.y = paths[i][j][m].y - (d2 * 0.5) * (sqrt(2) * 0.5);
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
					}
					else if (m == paths[i][j].size() - 1) {//终点
						if (paths[i][j][m - 1].id == paths[i][j][m].id + x_count) {//上-下
							p.x = paths[i][j][m].x - d1 * 0.5;
							p.y = paths[i][j][m].y;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x - d1 * 0.5;
							p.y = paths[i][j][m].y - d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);

						}
						else if (paths[i][j][m - 1].id == paths[i][j][m].id - x_count) {//下-上
							p.x = paths[i][j][m].x + d1 * 0.5;
							p.y = paths[i][j][m].y;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x + d1 * 0.5;
							p.y = paths[i][j][m].y + d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);

						}
						else if (paths[i][j][m - 1].id == paths[i][j][m].id - 1) {//左-右
							p.x = paths[i][j][m].x;
							p.y = paths[i][j][m].y - d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x + d1 * 0.5;
							p.y = paths[i][j][m].y - d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);

						}
						else if (paths[i][j][m - 1].id == paths[i][j][m].id + 1) {//右-左
							p.x = paths[i][j][m].x;
							p.y = paths[i][j][m].y + d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x - d1 * 0.5;
							p.y = paths[i][j][m].y + d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);

						}
						else if (paths[i][j][m - 1].id == paths[i][j][m].id + x_count - 1) {//左上-右下
							p.x = paths[i][j][m].x - (d2 * 0.5) * (sqrt(2) * 0.5);
							p.y = paths[i][j][m].y - (d2 * 0.5) * (sqrt(2) * 0.5);
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x;
							p.y = paths[i][j][m].y - sqrt(2) * d2 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);

						}
						else if (paths[i][j][m - 1].id == paths[i][j][m].id - x_count + 1) {//右下-左上
							p.x = paths[i][j][m].x + (d2 * 0.5) * (sqrt(2) * 0.5);
							p.y = paths[i][j][m].y + (d2 * 0.5) * (sqrt(2) * 0.5);
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x;
							p.y = paths[i][j][m].y + sqrt(2) * d2 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);

						}
						else if (paths[i][j][m - 1].id == paths[i][j][m].id + x_count + 1) {//右上-左下
							p.x = paths[i][j][m].x - (d2 * 0.5) * (sqrt(2) * 0.5);
							p.y = paths[i][j][m].y + (d2 * 0.5) * (sqrt(2) * 0.5);
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x - sqrt(2) * d2 * 0.5;
							p.y = paths[i][j][m].y;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);

						}
						else if (paths[i][j][m - 1].id == paths[i][j][m].id - x_count - 1) {//左下-右上
							p.x = paths[i][j][m].x + (d2 * 0.5) * (sqrt(2) * 0.5);
							p.y = paths[i][j][m].y - (d2 * 0.5) * (sqrt(2) * 0.5);
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
							p.x = paths[i][j][m].x + sqrt(2) * d2 * 0.5;
							p.y = paths[i][j][m].y;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);

						}
					}
					else {
						if ((paths[i][j][m - 1].id == paths[i][j][m].id + x_count && paths[i][j][m].id == paths[i][j][m + 1].id + x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id + x_count && paths[i][j][m].id == paths[i][j][m + 1].id + x_count - 1) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id + x_count && paths[i][j][m].id == paths[i][j][m + 1].id + x_count + 1) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id + x_count - 1 && paths[i][j][m].id == paths[i][j][m + 1].id + x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id + x_count + 1 && paths[i][j][m].id == paths[i][j][m + 1].id + x_count)) { //case 1
							p.x = paths[i][j][m].x - d1 * 0.5;
							p.y = paths[i][j][m].y;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id - x_count && paths[i][j][m].id == paths[i][j][m + 1].id - x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - x_count + 1 && paths[i][j][m].id == paths[i][j][m + 1].id - x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - x_count - 1 && paths[i][j][m].id == paths[i][j][m + 1].id - x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - x_count && paths[i][j][m].id == paths[i][j][m + 1].id - x_count + 1) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - x_count && paths[i][j][m].id == paths[i][j][m + 1].id - x_count - 1)) {//case 2
							p.x = paths[i][j][m].x + d1 * 0.5;
							p.y = paths[i][j][m].y;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id - 1 && paths[i][j][m].id == paths[i][j][m + 1].id - 1) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - 1 && paths[i][j][m].id == paths[i][j][m + 1].id - 1 - x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - 1 && paths[i][j][m].id == paths[i][j][m + 1].id - 1 + x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - 1 + x_count && paths[i][j][m].id == paths[i][j][m + 1].id - 1) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - 1 - x_count && paths[i][j][m].id == paths[i][j][m + 1].id - 1)) {//case 3
							p.x = paths[i][j][m].x;
							p.y = paths[i][j][m].y - d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id + 1 && paths[i][j][m].id == paths[i][j][m + 1].id + 1) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id + 1 + x_count && paths[i][j][m].id == paths[i][j][m + 1].id + 1) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id + 1 - x_count && paths[i][j][m].id == paths[i][j][m + 1].id + 1) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id + 1 && paths[i][j][m].id == paths[i][j][m + 1].id + 1 - x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id + 1 && paths[i][j][m].id == paths[i][j][m + 1].id + 1 + x_count)) {//case 4
							p.x = paths[i][j][m].x;
							p.y = paths[i][j][m].y + d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if (paths[i][j][m - 1].id == paths[i][j][m].id + x_count - 1 && paths[i][j][m].id == paths[i][j][m + 1].id + x_count - 1) {//case 5
							p.x = paths[i][j][m].x - (d2 * 0.5) * (sqrt(2) * 0.5);
							p.y = paths[i][j][m].y - (d2 * 0.5) * (sqrt(2) * 0.5);
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if (paths[i][j][m - 1].id == paths[i][j][m].id - x_count + 1 && paths[i][j][m].id == paths[i][j][m + 1].id - x_count + 1) {//case 6
							p.x = paths[i][j][m].x + (d2 * 0.5) * (sqrt(2) * 0.5);
							p.y = paths[i][j][m].y + (d2 * 0.5) * (sqrt(2) * 0.5);
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if (paths[i][j][m - 1].id == paths[i][j][m].id - x_count - 1 && paths[i][j][m].id == paths[i][j][m + 1].id - x_count - 1) {//case 7
							p.x = paths[i][j][m].x + (d2 * 0.5) * (sqrt(2) * 0.5);
							p.y = paths[i][j][m].y - (d2 * 0.5) * (sqrt(2) * 0.5);
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if (paths[i][j][m - 1].id == paths[i][j][m].id + x_count + 1 && paths[i][j][m].id == paths[i][j][m + 1].id + x_count + 1) {//case 8
							p.x = paths[i][j][m].x - (d2 * 0.5) * (sqrt(2) * 0.5);
							p.y = paths[i][j][m].y + (d2 * 0.5) * (sqrt(2) * 0.5);
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id + x_count && paths[i][j][m].id == paths[i][j][m + 1].id - 1) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - 1 && paths[i][j][m].id == paths[i][j][m + 1].id + x_count)) {//case 9
							p.x = paths[i][j][m].x - d1 * 0.5;
							p.y = paths[i][j][m].y - d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id + 1 && paths[i][j][m].id == paths[i][j][m + 1].id - x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - x_count && paths[i][j][m].id == paths[i][j][m + 1].id + 1)) {//case 10
							p.x = paths[i][j][m].x + d1 * 0.5;
							p.y = paths[i][j][m].y + d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id - 1 && paths[i][j][m].id == paths[i][j][m + 1].id - x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - x_count && paths[i][j][m].id == paths[i][j][m + 1].id - 1)) {//case 11
							p.x = paths[i][j][m].x + d1 * 0.5;
							p.y = paths[i][j][m].y - d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id + x_count && paths[i][j][m].id == paths[i][j][m + 1].id + 1) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id + 1 && paths[i][j][m].id == paths[i][j][m + 1].id + x_count)) {//case 12
							p.x = paths[i][j][m].x - d1 * 0.5;
							p.y = paths[i][j][m].y + d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id + x_count && paths[i][j][m].id == paths[i][j][m + 1].id - x_count - 1) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - x_count - 1 && paths[i][j][m].id == paths[i][j][m + 1].id + x_count)) {//case 13
							p.x = paths[i][j][m].x - d1 * 0.5;
							p.y = paths[i][j][m].y - d1;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id + x_count + 1 && paths[i][j][m].id == paths[i][j][m + 1].id - x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - x_count && paths[i][j][m].id == paths[i][j][m + 1].id + x_count + 1)) {//case 14
							p.x = paths[i][j][m].x + d1 * 0.5;
							p.y = paths[i][j][m].y + d1;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id + x_count && paths[i][j][m].id == paths[i][j][m + 1].id - x_count + 1) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - x_count + 1 && paths[i][j][m].id == paths[i][j][m + 1].id + x_count)) {//case 15
							p.x = paths[i][j][m].x - d1 * 0.5;
							p.y = paths[i][j][m].y + d1;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id + x_count - 1 && paths[i][j][m].id == paths[i][j][m + 1].id - x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - x_count && paths[i][j][m].id == paths[i][j][m + 1].id + x_count - 1)) {//case 16
							p.x = paths[i][j][m].x + d1 * 0.5;
							p.y = paths[i][j][m].y - d1;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id - 1 && paths[i][j][m].id == paths[i][j][m + 1].id - x_count + 1) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - x_count + 1 && paths[i][j][m].id == paths[i][j][m + 1].id - 1)) {//case 17
							p.x = paths[i][j][m].x + d1;
							p.y = paths[i][j][m].y - d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id - 1 + x_count && paths[i][j][m].id == paths[i][j][m + 1].id + 1) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id + 1 && paths[i][j][m].id == paths[i][j][m + 1].id - 1 + x_count)) {//case 18
							p.x = paths[i][j][m].x - d1;
							p.y = paths[i][j][m].y + d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id + 1 + x_count && paths[i][j][m].id == paths[i][j][m + 1].id - 1) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - 1 && paths[i][j][m].id == paths[i][j][m + 1].id + 1 + x_count)) {//case 19
							p.x = paths[i][j][m].x - d1;
							p.y = paths[i][j][m].y - d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id + 1 && paths[i][j][m].id == paths[i][j][m + 1].id - 1 - x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - 1 - x_count && paths[i][j][m].id == paths[i][j][m + 1].id + 1)) {//case 20
							p.x = paths[i][j][m].x + d1;
							p.y = paths[i][j][m].y + d1 * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id + x_count - 1 && paths[i][j][m].id == paths[i][j][m + 1].id - 1 - x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - 1 - x_count && paths[i][j][m].id == paths[i][j][m + 1].id + x_count - 1)) {//case 21
							p.x = paths[i][j][m].x;
							p.y = paths[i][j][m].y - d1 * sqrt(2) * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id + x_count + 1 && paths[i][j][m].id == paths[i][j][m + 1].id + 1 - x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id + 1 - x_count && paths[i][j][m].id == paths[i][j][m + 1].id + x_count + 1)) {//case 22
							p.x = paths[i][j][m].x;
							p.y = paths[i][j][m].y + d1 * sqrt(2) * 0.5;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id + x_count + 1 && paths[i][j][m].id == paths[i][j][m + 1].id - 1 + x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - 1 + x_count && paths[i][j][m].id == paths[i][j][m + 1].id + x_count + 1)) {//case 23
							p.x = paths[i][j][m].x - d1 * sqrt(2) * 0.5;
							p.y = paths[i][j][m].y;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
						else if ((paths[i][j][m - 1].id == paths[i][j][m].id - x_count + 1 && paths[i][j][m].id == paths[i][j][m + 1].id - 1 - x_count) ||
							(paths[i][j][m - 1].id == paths[i][j][m].id - 1 - x_count && paths[i][j][m].id == paths[i][j][m + 1].id - x_count + 1)) {//case 24
							p.x = paths[i][j][m].x + d1 * sqrt(2) * 0.5;
							p.y = paths[i][j][m].y;
							p.z = paths[i][j][m].z;
							paths_buffer1.push_back(p);
						}
					}
				}
			}
			//paths_buffer1.push_back(paths_buffer1[0]);//终点与起点连接
			paths_buffer2.push_back(paths_buffer1);
			paths_buffer1.clear();
		}
		paths_buffer3.push_back(paths_buffer2);
		paths_buffer2.clear();
	}
	lines = paths;
	paths.clear();
	paths = paths_buffer3;
	paths_buffer3.clear();
	paths_buffer1.clear();
	for (int i = 0; i < paths.size(); i++) {
		for (int j = 0; j < paths[i].size(); j++) {
			for (int m = 0; m < paths[i][j].size() - 1; m += 2) {
				p.x = (paths[i][j][m].x + paths[i][j][m + 1].x) * 0.5;
				p.y = (paths[i][j][m].y + paths[i][j][m + 1].y) * 0.5;
				p.z = (paths[i][j][m].z + paths[i][j][m + 1].z) * 0.5;
				paths_buffer1.push_back(p);
				paths[i][j].insert(paths[i][j].begin() + m + 1, paths_buffer1.begin(), paths_buffer1.end());
				paths_buffer1.clear();

			}
		}
	}
}
void underfill_optimize() {
	path p;
	for (int i = 0; i < model.size(); i++) {
		for (int j = 0; j < model[i].size(); j++) {
			if (model[i][j].b == 0) {
				model[i][j].b++;
				for (int n = 0; n < 4; n++) {
					if (n == 0) {
						p.x = model[i][j].x - d1 * 0.5;
						p.y = model[i][j].y + d1 * 0.5;
						p.z = model[i][j].z;
						paths_buffer1.push_back(p);
					}
					else if (n == 1) {
						p.x = model[i][j].x + d1 * 0.5;
						p.y = model[i][j].y + d1 * 0.5;
						p.z = model[i][j].z;
						paths_buffer1.push_back(p);
					}
					else if (n == 2) {
						p.x = model[i][j].x + d1 * 0.5;
						p.y = model[i][j].y - d1 * 0.5;
						p.z = model[i][j].z;
						paths_buffer1.push_back(p);
					}
					else if (n == 3) {
						p.x = model[i][j].x - d1 * 0.5;
						p.y = model[i][j].y - d1 * 0.5;
						p.z = model[i][j].z;
						paths_buffer1.push_back(p);
					}
				}

				paths_buffer1.push_back(paths_buffer1[0]);
				paths[i].push_back(paths_buffer1);
				paths_buffer1.clear();
				p.x = model[i][j].x;
				p.y = model[i][j].y;
				p.z = model[i][j].z;
				paths_buffer1.push_back(p);
				lines[i].push_back(paths_buffer1);
				paths_buffer1.clear();
			}
		}
	}
}

void Continuous_path() {

	for (int i = 0; i < paths.size(); i++) {
		for (int j = 1; j < paths[i].size(); j++) {
			for (int n = 0; n < paths[i][j].size() - 1; n++) {
				for (int m = 0; m < paths[i][0].size() - 1; m++) {
					//上下
					if (paths[i][j][n].x != paths[i][j][n + 1].x && paths[i][j][n].y == paths[i][j][n + 1].y && paths[i][0][m].x != paths[i][0][m + 1].x && paths[i][0][m].y == paths[i][0][m + 1].y) {
						//顺序
						if (paths[i][j][n].x == paths[i][0][m].x && paths[i][j][n + 1].x == paths[i][0][m + 1].x && abs(paths[i][j][n].y - paths[i][0][m].y) > d1 - 0.01 && abs(paths[i][j][n].y - paths[i][0][m].y) < d1 + 0.01 && abs(paths[i][j][n + 1].y - paths[i][0][m + 1].y) > d1 - 0.01 && abs(paths[i][j][n + 1].y - paths[i][0][m + 1].y) < d1 + 0.01) {
							reverse(paths[i][j].begin(), paths[i][j].begin() + n + 1);
							reverse(paths[i][j].begin() + n + 1, paths[i][j].end());
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin(), paths[i][j].end());
							paths[i].erase(paths[i].begin() + j);
							j = 0; break;
						}
						////逆序
						else if (paths[i][j][n].x == paths[i][0][m + 1].x && paths[i][j][n + 1].x == paths[i][0][m].x && abs(paths[i][j][n].y - paths[i][0][m + 1].y) > d1 - 0.01 && abs(paths[i][j][n].y - paths[i][0][m + 1].y) < d1 + 0.01 && abs(paths[i][j][n + 1].y - paths[i][0][m].y) > d1 - 0.01 && abs(paths[i][j][n + 1].y - paths[i][0][m].y) < d1 + 0.01) {
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin(), paths[i][j].begin() + n + 1);
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin() + n + 1, paths[i][j].end());
							paths[i].erase(paths[i].begin() + j);
							j = 0; break;
						}
					}
					//左右
					else if (paths[i][j][n].y != paths[i][j][n + 1].y && paths[i][j][n].x == paths[i][j][n + 1].x && paths[i][0][m].y != paths[i][0][m + 1].y && paths[i][0][m].x == paths[i][0][m + 1].x) {
						//顺序
						if (paths[i][j][n].y == paths[i][0][m].y && paths[i][j][n + 1].y == paths[i][0][m + 1].y && abs(paths[i][j][n].x - paths[i][0][m].x) > d1 - 0.0l && abs(paths[i][j][n].x - paths[i][0][m].x) < d1 + 0.0l && abs(paths[i][j][n + 1].x - paths[i][0][m + 1].x) > d1 - 0.01 && abs(paths[i][j][n + 1].x - paths[i][0][m + 1].x) < d1 + 0.01) {
							reverse(paths[i][j].begin(), paths[i][j].begin() + n + 1);
							reverse(paths[i][j].begin() + n + 1, paths[i][j].end());
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin(), paths[i][j].end());
							paths[i].erase(paths[i].begin() + j);
							j = 0; break;
						}
						//逆序
						else if (paths[i][j][n].y == paths[i][0][m + 1].y && paths[i][j][n + 1].y == paths[i][0][m].y && abs(paths[i][j][n].x - paths[i][0][m + 1].x) > d1 - 0.001 && abs(paths[i][j][n].x - paths[i][0][m + 1].x) < d1 + 0.001 && abs(paths[i][j][n + 1].x - paths[i][0][m].x) > d1 - 0.001 && abs(paths[i][j][n + 1].x - paths[i][0][m].x) < d1 + 0.001) {
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin(), paths[i][j].begin() + n + 1);
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin() + n + 1, paths[i][j].end());
							paths[i].erase(paths[i].begin() + j);
							j = 0; break;
						}
					}
					//斜
					else if (paths[i][j][n].y != paths[i][j][n + 1].y && paths[i][j][n].x != paths[i][j][n + 1].x && paths[i][0][m].y != paths[i][0][m + 1].y && paths[i][0][m].x != paths[i][0][m + 1].x) {
						//顺序
						if (abs(paths[i][j][n].x - paths[i][0][m].x) < (sqrt(2) * d2 * 0.5) + 0.01 && abs(paths[i][j][n].x - paths[i][0][m].x) > (sqrt(2) * d2 * 0.5) - 0.01 && abs(paths[i][j][n].y - paths[i][0][m].y) < (sqrt(2) * d2 * 0.5) + 0.01 && abs(paths[i][j][n].y - paths[i][0][m].y) > (sqrt(2) * d2 * 0.5) - 0.01 &&
							abs(paths[i][j][n + 1].x - paths[i][0][m + 1].x) < (sqrt(2) * d2 * 0.5) + 0.01 && abs(paths[i][j][n + 1].x - paths[i][0][m + 1].x) > (sqrt(2) * d2 * 0.5) - 0.01 && abs(paths[i][j][n + 1].y - paths[i][0][m + 1].y) < (sqrt(2) * d2 * 0.5) + 0.01 && abs(paths[i][j][n + 1].y - paths[i][0][m + 1].y) > (sqrt(2) * d2 * 0.5) - 0.01) {
							paths[i][0][m + 1].x = (paths[i][0][m].x + paths[i][0][m + 1].x) * 0.5;
							paths[i][0][m + 1].y = (paths[i][0][m].y + paths[i][0][m + 1].y) * 0.5;
							paths[i][j][n + 1].x = (paths[i][j][n].x + paths[i][j][n + 1].x) * 0.5;
							paths[i][j][n + 1].y = (paths[i][j][n].y + paths[i][j][n + 1].y) * 0.5;
							reverse(paths[i][j].begin(), paths[i][j].begin() + n + 1);
							reverse(paths[i][j].begin() + n + 1, paths[i][j].end());
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin(), paths[i][j].end());
							paths[i].erase(paths[i].begin() + j);
							j = 0; break;
						}
						//逆序
						else if (abs(paths[i][j][n].x - paths[i][0][m + 1].x) < (sqrt(2) * d2 * 0.5) + 0.1 && abs(paths[i][j][n].x - paths[i][0][m + 1].x) > (sqrt(2) * d2 * 0.5) - 0.1 && abs(paths[i][j][n].y - paths[i][0][m + 1].y) < (sqrt(2) * d2 * 0.5) + 0.1 && abs(paths[i][j][n].y - paths[i][0][m + 1].y) > (sqrt(2) * d2 * 0.5) - 0.1 &&
							abs(paths[i][j][n + 1].x - paths[i][0][m].x) < (sqrt(2) * d2 * 0.5) + 0.1 && abs(paths[i][j][n + 1].x - paths[i][0][m].x) > (sqrt(2) * d2 * 0.5) - 0.1 && abs(paths[i][j][n + 1].y - paths[i][0][m].y) < (sqrt(2) * d2 * 0.5) + 0.1 && abs(paths[i][j][n + 1].y - paths[i][0][m].y) > (sqrt(2) * d2 * 0.5) - 0.1) {
							paths[i][0][m + 1].x = (paths[i][0][m].x + paths[i][0][m + 1].x) * 0.5;
							paths[i][0][m + 1].y = (paths[i][0][m].y + paths[i][0][m + 1].y) * 0.5;
							paths[i][j][n].x = (paths[i][j][n].x + paths[i][j][n + 1].x) * 0.5;
							paths[i][j][n].y = (paths[i][j][n].y + paths[i][j][n + 1].y) * 0.5;
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin(), paths[i][j].begin() + n + 1);//左开右闭
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin() + n + 1, paths[i][j].end());
							paths[i].erase(paths[i].begin() + j);
							j = 0; break;
						}
					}
				}
				if (j == 0) break;
			}
		}
	}


	//////孤立点连接
	for (int i = 0; i < paths.size(); i++) {
		for (int j = 1; j < paths[i].size(); j++) {
			for (int n = 0; n < paths[i][j].size() - 1; n++) {
				for (int m = 0; m < paths[i][0].size() - 1; m++) {
					//上下
					if (paths[i][j][n].x != paths[i][j][n + 1].x && paths[i][j][n].y == paths[i][j][n + 1].y && paths[i][0][m].x != paths[i][0][m + 1].x && paths[i][0][m].y == paths[i][0][m + 1].y) {
						//顺序
						if (abs(paths[i][j][n].x - paths[i][0][m].x) < (d1 * 0.5) + 0.4 && abs(paths[i][j][n].x - paths[i][0][m].x) > (d1 * 0.5) - 0.4 && abs(paths[i][j][n + 1].x - paths[i][0][m + 1].x) < (d1 * 0.5) + 0.4 && abs(paths[i][j][n + 1].x - paths[i][0][m + 1].x) > (d1 * 0.5) - 0.4 && abs(paths[i][j][n].y - paths[i][0][m].y) > d1 - 0.1 && abs(paths[i][j][n].y - paths[i][0][m].y) < d1 + 0.1 && abs(paths[i][j][n + 1].y - paths[i][0][m + 1].y) > d1 - 0.1 && abs(paths[i][j][n + 1].y - paths[i][0][m + 1].y) < d1 + 0.1) {
							reverse(paths[i][j].begin(), paths[i][j].begin() + n + 1);
							reverse(paths[i][j].begin() + n + 1, paths[i][j].end());
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin(), paths[i][j].end());
							paths[i].erase(paths[i].begin() + j);
							j = 0; break;
						}
						////逆序
						else if (abs(paths[i][j][n].x - paths[i][0][m + 1].x) < (d1 * 0.5) + 0.4 && abs(paths[i][j][n].x - paths[i][0][m + 1].x) > (d1 * 0.5) - 0.4 && abs(paths[i][j][n + 1].x - paths[i][0][m].x) < (d1 * 0.5) + 0.4 && abs(paths[i][j][n + 1].x - paths[i][0][m].x) > (d1 * 0.5) - 0.4 && abs(paths[i][j][n].y - paths[i][0][m + 1].y) > d1 - 0.1 && abs(paths[i][j][n].y - paths[i][0][m + 1].y) < d1 + 0.1 && abs(paths[i][j][n + 1].y - paths[i][0][m].y) > d1 - 0.1 && abs(paths[i][j][n + 1].y - paths[i][0][m].y) < d1 + 0.1) {
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin(), paths[i][j].begin() + n + 1);
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin() + n + 1, paths[i][j].end());
							paths[i].erase(paths[i].begin() + j);
							j = 0; break;
						}
					}
					//左右
					else if (paths[i][j][n].y != paths[i][j][n + 1].y && paths[i][j][n].x == paths[i][j][n + 1].x && paths[i][0][m].y != paths[i][0][m + 1].y && paths[i][0][m].x == paths[i][0][m + 1].x) {
						//顺序
						if (abs(paths[i][j][n].y - paths[i][0][m].y) < (d1 * 0.5) + 0.4 && abs(paths[i][j][n].y - paths[i][0][m].y) > (d1 * 0.5) - 0.4 && abs(paths[i][j][n + 1].y - paths[i][0][m + 1].y) < (d1 * 0.5) + 0.4 && abs(paths[i][j][n + 1].y - paths[i][0][m + 1].y) > (d1 * 0.5) - 0.4 && abs(paths[i][j][n].x - paths[i][0][m].x) > d1 - 0.l && abs(paths[i][j][n].x - paths[i][0][m].x) < d1 + 0.l && abs(paths[i][j][n + 1].x - paths[i][0][m + 1].x) > d1 - 0.1 && abs(paths[i][j][n + 1].x - paths[i][0][m + 1].x) < d1 + 0.1) {
							reverse(paths[i][j].begin(), paths[i][j].begin() + n + 1);
							reverse(paths[i][j].begin() + n + 1, paths[i][j].end());
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin(), paths[i][j].end());
							paths[i].erase(paths[i].begin() + j);
							j = 0; break;
						}
						//逆序
						else if (abs(paths[i][j][n].y - paths[i][0][m + 1].y) < (d1 * 0.5) + 0.4 && abs(paths[i][j][n].y - paths[i][0][m + 1].y) > (d1 * 0.5) - 0.4 && abs(paths[i][j][n + 1].y - paths[i][0][m].y) < (d1 * 0.5) + 0.4 && abs(paths[i][j][n + 1].y - paths[i][0][m].y) > (d1 * 0.5) - 0.4 && abs(paths[i][j][n].x - paths[i][0][m + 1].x) > d1 - 0.1 && abs(paths[i][j][n].x - paths[i][0][m + 1].x) < d1 + 0.1 && abs(paths[i][j][n + 1].x - paths[i][0][m].x) > d1 - 0.1 && abs(paths[i][j][n + 1].x - paths[i][0][m].x) < d1 + 0.1) {
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin(), paths[i][j].begin() + n + 1);
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin() + n + 1, paths[i][j].end());
							paths[i].erase(paths[i].begin() + j);
							j = 0; break;
						}
					}
					//斜
					else if (paths[i][j][n].y != paths[i][j][n + 1].y && paths[i][j][n].x != paths[i][j][n + 1].x && paths[i][0][m].y != paths[i][0][m + 1].y && paths[i][0][m].x != paths[i][0][m + 1].x) {
						//顺序
						if (abs(paths[i][j][n].x - paths[i][0][m].x) < (sqrt(2) * d2 * 0.5) + 0.4 && abs(paths[i][j][n].x - paths[i][0][m].x) > (sqrt(2) * d2 * 0.5) - 0.4 && abs(paths[i][j][n].y - paths[i][0][m].y) < (sqrt(2) * d2 * 0.5) + 0.4 && abs(paths[i][j][n].y - paths[i][0][m].y) > (sqrt(2) * d2 * 0.5) - 0.4 &&
							abs(paths[i][j][n + 1].x - paths[i][0][m + 1].x) < (sqrt(2) * d2 * 0.5) + 0.4 && abs(paths[i][j][n + 1].x - paths[i][0][m + 1].x) > (sqrt(2) * d2 * 0.5) - 0.4 && abs(paths[i][j][n + 1].y - paths[i][0][m + 1].y) < (sqrt(2) * d2 * 0.5) + 0.4 && abs(paths[i][j][n + 1].y - paths[i][0][m + 1].y) > (sqrt(2) * d2 * 0.5) - 0.4) {
							paths[i][0][m + 1].x = (paths[i][0][m].x + paths[i][0][m + 1].x) * 0.5;
							paths[i][0][m + 1].y = (paths[i][0][m].y + paths[i][0][m + 1].y) * 0.5;
							paths[i][j][n + 1].x = (paths[i][j][n].x + paths[i][j][n + 1].x) * 0.5;
							paths[i][j][n + 1].y = (paths[i][j][n].y + paths[i][j][n + 1].y) * 0.5;
							reverse(paths[i][j].begin(), paths[i][j].begin() + n + 1);
							reverse(paths[i][j].begin() + n + 1, paths[i][j].end());
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin(), paths[i][j].end());
							paths[i].erase(paths[i].begin() + j);
							j = 0; break;
						}
						//逆序
						else if (abs(paths[i][j][n].x - paths[i][0][m + 1].x) < (sqrt(2) * d2 * 0.5) + 0.4 && abs(paths[i][j][n].x - paths[i][0][m + 1].x) > (sqrt(2) * d2 * 0.5) - 0.4 && abs(paths[i][j][n].y - paths[i][0][m + 1].y) < (sqrt(2) * d2 * 0.5) + 0.4 && abs(paths[i][j][n].y - paths[i][0][m + 1].y) > (sqrt(2) * d2 * 0.5) - 0.4 &&
							abs(paths[i][j][n + 1].x - paths[i][0][m].x) < (sqrt(2) * d2 * 0.5) + 0.4 && abs(paths[i][j][n + 1].x - paths[i][0][m].x) > (sqrt(2) * d2 * 0.5) - 0.4 && abs(paths[i][j][n + 1].y - paths[i][0][m].y) < (sqrt(2) * d2 * 0.5) + 0.4 && abs(paths[i][j][n + 1].y - paths[i][0][m].y) > (sqrt(2) * d2 * 0.5) - 0.4) {
							paths[i][0][m + 1].x = (paths[i][0][m].x + paths[i][0][m + 1].x) * 0.5;
							paths[i][0][m + 1].y = (paths[i][0][m].y + paths[i][0][m + 1].y) * 0.5;
							paths[i][j][n].x = (paths[i][j][n].x + paths[i][j][n + 1].x) * 0.5;
							paths[i][j][n].y = (paths[i][j][n].y + paths[i][j][n + 1].y) * 0.5;
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin(), paths[i][j].begin() + n + 1);//左开右闭
							paths[i][0].insert(paths[i][0].begin() + m + 1, paths[i][j].begin() + n + 1, paths[i][j].end());
							paths[i].erase(paths[i].begin() + j);
							j = 0; break;
						}
					}
				}
				if (j == 0) break;
			}
		}
	}
	for (int i = 0; i < paths.size(); i++) {
		for (int j = 0; j < paths[i].size(); j++) {
			paths[i][j].push_back(paths[i][j][0]);
		}
	}
	//删除直线的中间坐标点
	for (int i = 0; i < paths.size(); i++) {
		for (int j = 0; j < paths[i].size(); j++) {
			if (paths[i][j].size() > 2) {
				for (int m = 2; m < paths[i][j].size(); m++) {
					if (((paths[i][j][m].x == paths[i][j][m - 1].x && paths[i][j][m - 1].x == paths[i][j][m - 2].x) || (paths[i][j][m].y == paths[i][j][m - 1].y && paths[i][j][m - 1].y == paths[i][j][m - 2].y))
						) {
						paths[i][j].erase(paths[i][j].begin() + m - 1);
						m = m - 1;
					}
				}
			}

		}
	}
}



