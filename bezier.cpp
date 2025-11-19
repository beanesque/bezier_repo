#include <iostream>

using namespace std;

int precision = 100;

struct vector2{
	float x;
	float y;
	
	string as_str(){
		return "(" + to_string(x) + ", " + to_string(y) + ")";
	}

	vector2(float in_x, float in_y){
		x = in_x;
		y = in_y;
	}
};

vector2 a[7] = {
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

vector2 lerpv(float time, vector2 start, vector2 end){
	vector2 out = vector2(0, 0);
	out.x = lerp(time, start.x, end.x);
	out.y = lerp(time, start.y, end.y);
	return out;
}

vector2 bezier(float time, vector2 anchor_0, vector2 anchor_1, vector2 anchor_2){
	vector2 out = vector2(0, 0);
	vector2 start = vector2(0, 0);
	vector2 end = vector2(0, 0);

	start = lerpv(time, anchor_0, anchor_1);
	end = lerpv(time, anchor_1, anchor_2);
	out = lerpv(time, start, end);

	return out;
}

vector2 cubic_bezier(float time, vector2 anchor_0, vector2 anchor_1, vector2 anchor_2, vector2 anchor_3){
	vector2 out = vector2(0, 0);
	vector2 start = vector2(0, 0);
	vector2 end = vector2(0, 0);

	start = bezier(time, anchor_0, anchor_1, anchor_2);
	end = bezier(time, anchor_1, anchor_2, anchor_3);
	out = lerpv(time, start, end);

	return out;
}

vector2 quartic_bezier(float time, vector2 anchor_0, vector2 anchor_1, vector2 anchor_2, vector2 anchor_3, vector2 anchor_4){
	vector2 out = vector2(0, 0);
	vector2 start = vector2(0, 0);
	vector2 end = vector2(0, 0);

	start = cubic_bezier(time, anchor_0, anchor_1, anchor_2, anchor_3);
	end = cubic_bezier(time, anchor_1, anchor_2, anchor_3, anchor_4);
	out = lerpv(time, start, end);

	return out;
}

vector2 quintic_bezier(float time, vector2 anchor_0, vector2 anchor_1, vector2 anchor_2, vector2 anchor_3, vector2 anchor_4, vector2 anchor_5){
	vector2 out = vector2(0, 0);
	vector2 start = vector2(0, 0);
	vector2 end = vector2(0, 0);

	start = quartic_bezier(time, anchor_0, anchor_1, anchor_2, anchor_3, anchor_4);
	end = quartic_bezier(time, anchor_1, anchor_2, anchor_3, anchor_4, anchor_5);
	out = lerpv(time, start, end);

	return out;
}

vector2 sextic_bezier(float time, vector2 anchor_0, vector2 anchor_1, vector2 anchor_2, vector2 anchor_3, vector2 anchor_4, vector2 anchor_5, vector2 anchor_6){
	vector2 out = vector2(0, 0);
	vector2 start = vector2(0, 0);
	vector2 end = vector2(0, 0);

	start = quintic_bezier(time, anchor_0, anchor_1, anchor_2, anchor_3, anchor_4, anchor_5);
	end = quintic_bezier(time, anchor_1, anchor_2, anchor_3, anchor_4, anchor_5, anchor_6);
	out = lerpv(time, start, end);

	return out;
}

int main(void){
	cout << "precision (must be int, recommended to be 100): ";
	cin >> precision;
	cout << "point values must be float or int, formatted x \\n y\n-----";
	for(int i = 0; i < (sizeof(a)/sizeof(a[0])); i++){
		cout << "\n" << "point " << i << ":\n";
		cin >> a[i].x;
		cin >> a[i].y;
		cout << "-----";
	}

	cout << "----------\n";

	for(int i = 0; i <= precision; i++){
		float time = inverse_lerp(i, 0, precision);
		vector2 point = vector2(0, 0);
		point = sextic_bezier(time, a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
		cout << point.as_str() << "\n";
	}
	return 1;
}
