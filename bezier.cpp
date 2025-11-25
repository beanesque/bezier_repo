#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int precision = 100;
int degree = 1;
float extpl = 1.0;

bool screen[50][50];

struct vec2{
	float x, y;
	
	string as_str(){
		return "(" + to_string(x) + ", " + to_string(y) + ")";
	}

	vec2(float x, float y) : x(x), y(y) {}
};

vec2 a[8] = {
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0}
};

float lerp(float time, float start, float end){
	float out;
	out = start + (end - start) * time;
	return out;
}

float inverse_lerp(float val, float start, float end){
	float out;
	out = val / (end - start);
	return out;
}

vec2 lerpv(float time, vec2 start, vec2 end){
	vec2 out = vec2(0, 0);
	out.x = lerp(time, start.x, end.x);
	out.y = lerp(time, start.y, end.y);
	return out;
}

vec2 bezier(float time, vec2 anchor_0, vec2 anchor_1, vec2 anchor_2){
	vec2 out = vec2(0, 0);
	vec2 start = vec2(0, 0);
	vec2 end = vec2(0, 0);

	start = lerpv(time, anchor_0, anchor_1);
	end = lerpv(time, anchor_1, anchor_2);
	out = lerpv(time, start, end);

	return out;
}

vec2 cubic_bezier(float time, vec2 anchor_0, vec2 anchor_1, vec2 anchor_2, vec2 anchor_3){
	vec2 out = vec2(0, 0);
	vec2 start = vec2(0, 0);
	vec2 end = vec2(0, 0);

	start = bezier(time, anchor_0, anchor_1, anchor_2);
	end = bezier(time, anchor_1, anchor_2, anchor_3);
	out = lerpv(time, start, end);

	return out;
}

vec2 quartic_bezier(float time, vec2 anchor_0, vec2 anchor_1, vec2 anchor_2, vec2 anchor_3, vec2 anchor_4){
	vec2 out = vec2(0, 0);
	vec2 start = vec2(0, 0);
	vec2 end = vec2(0, 0);

	start = cubic_bezier(time, anchor_0, anchor_1, anchor_2, anchor_3);
	end = cubic_bezier(time, anchor_1, anchor_2, anchor_3, anchor_4);
	out = lerpv(time, start, end);

	return out;
}

vec2 quintic_bezier(float time, vec2 anchor_0, vec2 anchor_1, vec2 anchor_2, vec2 anchor_3, vec2 anchor_4, vec2 anchor_5){
	vec2 out = vec2(0, 0);
	vec2 start = vec2(0, 0);
	vec2 end = vec2(0, 0);

	start = quartic_bezier(time, anchor_0, anchor_1, anchor_2, anchor_3, anchor_4);
	end = quartic_bezier(time, anchor_1, anchor_2, anchor_3, anchor_4, anchor_5);
	out = lerpv(time, start, end);

	return out;
}

vec2 sextic_bezier(float time, vec2 anchor_0, vec2 anchor_1, vec2 anchor_2, vec2 anchor_3, vec2 anchor_4, vec2 anchor_5, vec2 anchor_6){
	vec2 out = vec2(0, 0);
	vec2 start = vec2(0, 0);
	vec2 end = vec2(0, 0);

	start = quintic_bezier(time, anchor_0, anchor_1, anchor_2, anchor_3, anchor_4, anchor_5);
	end = quintic_bezier(time, anchor_1, anchor_2, anchor_3, anchor_4, anchor_5, anchor_6);
	out = lerpv(time, start, end);

	return out;
}

vec2 septic_bezier(float time, vec2 anchor_0, vec2 anchor_1, vec2 anchor_2, vec2 anchor_3, vec2 anchor_4, vec2 anchor_5, vec2 anchor_6, vec2 anchor_7){
	vec2 out = vec2(0, 0);
	vec2 start = vec2(0, 0);
	vec2 end = vec2(0, 0);

	start = sextic_bezier(time, anchor_0, anchor_1, anchor_2, anchor_3, anchor_4, anchor_5, anchor_6);
	end = sextic_bezier(time, anchor_1, anchor_2, anchor_3, anchor_4, anchor_5, anchor_6, anchor_7);
	out = lerpv(time, start, end);

	return out;
}

void render_point(vec2 point, float scale){
	if(point.x < (sizeof(screen)/sizeof(screen[0])) * scale && point.y < (sizeof(screen)/sizeof(screen[0]) * scale)){
		if(point.x > 0 && point.y > 0){
				screen[static_cast<int>(round(point.x)/scale)][static_cast<int>(round(point.y)/scale)] = true;
		}

	}
	return;
}

void prt_scr(float scale){
	cout << "  ";
	for(unsigned int j = 0; j < sizeof(screen[0])/sizeof(screen[0][0]); j++){
		if(j * scale < 10){
			cout << " 00" << j * scale;
		}else if(j * scale < 100){
			cout << " 0" << j * scale;
		}else{
			cout << " " << j * scale;
		}
	}
	cout << "\n";
	for(int i = sizeof(screen)/sizeof(screen[0]) - 1; i > -1; i--){
		if(i * scale < 10){
			cout << "00" << i * scale;
		}else if(i * scale < 100){
			cout << "0" << i * scale;
		}else{
			cout << i * scale;
		}
		for(unsigned int j = 0; j < sizeof(screen[i])/sizeof(screen[i][0]); j++){
			if(screen[j][i] == true){
				cout << "  X ";
			}else{
				cout << "    ";
			}
		}
		cout << "\n";
	}
}

int main(int argc, char* argv[]){
	string file_name = "bezier_points.txt";
	bool show_anchors, into_file, graph = false;
	
	ofstream output;

	for(unsigned int i = 1; i < argc; i++){
		if(argv[i] == "-show-anchors"sv){
			show_anchors = true;
		}else if(argv[i] == "-file"sv){
			into_file = true;
			file_name = argv[i + 1];
			output.open(file_name);
		}else if(argv[i] == "-graph"sv){
			graph = true;
		}
		if(i > 4){
			cout << "too many parameters\n";
			return 0;
		}
	}


	cout << "degree (int from 0 to " << sizeof(a)/sizeof(a[0]) - 1 << "): ";
	cin >> degree;
	if(degree > sizeof(a)/sizeof(a[0]) - 1){
		degree = sizeof(a)/sizeof(a[0]) - 1;
	}else if(degree < 0){
		degree = 0;
	}

	cout << "precision (must be int, recommended to be 100): ";
	cin >> precision;

	cout << "amount of extrapolation (how far it stretches past the curve, float, default 1): ";
	cin >> extpl;

	cout << "-----\npoint values must be float or int, formatted x \\n y\n-----";

	cout << "\nstarting point: \n";
	cin >> a[0].x;
	cin >> a[0].y;

	cout << "\nending point: \n";
	cin >> a[degree].x;
	cin >> a[degree].y;

	for(int i = 1; i < degree; i++){
		cout << "\n" << "anchor " << i << ":\n";
		cin >> a[i].x;
		cin >> a[i].y;
		cout << "-----";
	}

	cout << "----------\n";
	
	for(int i = -(extpl - 1) * precision; i <= extpl * precision; i++){
		float time = inverse_lerp(i, 0, precision);
		vec2 point = vec2(0, 0);

		switch(degree){
			case 0:
				point = a[0];
				break;
			case 1:
				point = lerpv(time, a[0], a[1]);
				break;
			case 2:
				point = bezier(time, a[0], a[1], a[2]);
				break;
			case 3:
				point = cubic_bezier(time, a[0], a[1], a[2], a[3]);
				break;
			case 4:
				point = quartic_bezier(time, a[0], a[1], a[2], a[3], a[4]);
				break;
			case 5:
				point = quintic_bezier(time, a[0], a[1], a[2], a[3], a[4], a[5]);
				break;
			case 6:
				point = sextic_bezier(time, a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
				break;
			case 7:
				point = septic_bezier(time, a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7]);
				break;
			default:
				break;
		}

		if(into_file == true){
			output << point.as_str() << "\n";
		}else if(graph == false){
			cout << point.as_str() << "\n";
		}
		if(graph == true){
			render_point(point, 5);
		}

	}

	if(show_anchors == true){
		cout << "\n";
		for(unsigned int i = 0; i <= degree; i++){
			if(into_file == true){
				output << a[i].as_str() << "\n";
			}else{
				cout << a[i].as_str() << "\n";
			}
			if(graph == true){
				render_point(a[i], 5);
			}
		}
	}
	if(graph == true){
		prt_scr(5);
	}
	output.close();
	
	return 0;
}
