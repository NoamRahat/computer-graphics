#include"Object.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "BB.h"
#include"light.h"
#include"renderer.h"
Object::Object()
{

}
Object::~Object()
{
}
glm::mat4 Object::RotationMatrix(float radians, char axis) {

	glm::mat4 rotate;
	if (axis == 'x') {
		rotate = glm::mat4{
			1, 0, 0, 0,
			0, cos(radians), -sin(radians), 0,
			0, sin(radians), cos(radians), 0,
			0, 0, 0, 1
		};
	}
	else if (axis == 'y') {
		rotate = glm::mat4{
			cos(radians), 0, sin(radians), 0,
			0, 1, 0, 0,
			-sin(radians), 0, cos(radians), 0,
			0, 0, 0, 1
		};
	}
	else if (axis == 'z') {
		rotate = glm::mat4{
			cos(radians), -sin(radians), 0, 0,
			sin(radians), cos(radians), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
	}
	return rotate;
}
void Object::XRotationMatrix(float radians) {

	glm::mat4 rotate = glm::mat4{
			1, 0, 0, 0,
			0, cos(radians), -sin(radians), 0,
			0, sin(radians), cos(radians), 0,
			0, 0, 0, 1
	};
	TotalRotationMatrix *= rotate; // Return the constructed translation matrix
}
void Object::YRotationMatrix(float radians) {

	glm::mat4 rotate = glm::mat4{
			cos(radians), 0, sin(radians), 0,
			0, 1, 0, 0,
			-sin(radians), 0, cos(radians), 0,
			0, 0, 0, 1
	};

	TotalRotationMatrix *= rotate; // Return the constructed translation matrix
}
void Object::ZRotationMatrix(float radians) {

	glm::mat4 rotate = glm::mat4{
			cos(radians), -sin(radians), 0, 0,
			sin(radians), cos(radians), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };


	TotalRotationMatrix *= rotate; // Return the constructed translation matrix
}
glm::mat4 Object::TranslateMatrix(float x, float y, float z) {
	glm::mat4 translate = {
	1.0f, 0.0f, 0.0f, 0.0f, // First column
	0.0f, 1.0f, 0.0f, 0.0f, // Second column
	0.0f, 0.0f, 1.0f, 0.0f, // Third column
	x,    y,    z,    1.0f  // Fourth column (translation)
	};
	return translate; // Return the constructed translation matrix
}
glm::mat4 Object::ScaleMatrix(float x, float y, float z) {
	glm::mat4 scale = {
	x,0.0f,0.0f,0.0f,
	0.0f,y,0.0f,0.0f,
	0.0f,0.0f,z,0.0f,
	0.0f,0.0f,0.0f,1.0f
	};
	return scale; // Return the constructed translation matrix
}
void Object::initializeZBuffer()
{
	for (int i = 0; i < 1000; i++) {
		for (int j = 0; j < 1540; j++) {
			zBuffer[i][j] = 1000;
		}
	}
}
void Object::print() {
	linesToPrint.clear();
	colorVec.clear();
	PointArrayPrint.clear();
	vertixNormalToPrint.clear();
	camera.aspectRatio = 1;
	updateCamera();
	glm::mat4 ScreenScale = ScaleMatrix(std::min(screenwith, screenhight), std::min(screenwith, screenhight), 1);
	glm::mat4 translation = TranslateMatrix(origen[0], origen[1], origen[2]);
	glm::mat4 NormalSize = ScaleMatrix(Nsize, Nsize, Nsize);
	glm::vec4 TP;
	glm::vec4 VTP;
	glm::vec4 NTP;
	std::vector<int> insideX;
	std::vector<int> insideY;
	std::vector<int> insideZ;
	transform = ScreenScale * camera.projectionMatrix * camera.viewMatrix * translation * TotalRotationMatrix * TotalScaleMatrix;
	glm::mat4 Tofrustum = camera.projectionMatrix * camera.viewMatrix * translation * TotalRotationMatrix * TotalScaleMatrix;
	for (int i = 0; i < PointArray.size(); i++) {
		 TP= Tofrustum * PointArray[i];


		 if(TP.x < -TP.w * camera.r || TP.x > TP.w * camera.r) { insideX.push_back(0); }
		 else { insideX.push_back(1); }
		if(TP.y < -TP.w * camera.t || TP.y > TP.w * camera.t) { insideY.push_back(0); }
		else { insideY.push_back(1); }
		if( TP.z < -TP.w || TP.z > TP.w) { insideZ.push_back(0); }
		 else { insideZ.push_back(1); }





		TP = ScreenScale * TP;

		if (TP.w != 0) {
			TP.x = TP.x / TP.w;
			TP.y = TP.y / TP.w;
			TP.z = TP.z / TP.w;
			TP.w = 1;
		}
		PointArrayPrint.push_back(TP);
		if (printVertixNormalsflag|shading != 0) {
		NTP = TotalRotationMatrix * TotalScaleMatrix * vertixNormal[i];
		vertixNormalToPrint.push_back(glm::normalize(NTP));
		}
		if (printVertixNormalsflag) {
			NTP =   camera.projectionMatrix * camera.viewMatrix * TotalRotationMatrix * (NormalSize*TotalScaleMatrix) * vertixNormal[i];

			linesToPrint.emplace_back(round(NTP.x + TP.x), round(NTP.y + TP.y), round(TP.x), round(TP.y));

		}
				
	}
		

	
	if (shading ==0) {
		for (const auto& edge : edgeSet) {//the first 3 points are for object origen
			if (PointArrayPrint[edge.v1].z > 1 || PointArrayPrint[edge.v1].z < -1) { continue; }
			if (PointArrayPrint[edge.v2].z > 1 || PointArrayPrint[edge.v2].z < -1) { continue; }
			linesToPrint.emplace_back(round(PointArrayPrint[edge.v1][0]), round(PointArrayPrint[edge.v1][1]),round(PointArrayPrint[edge.v2][0]), round(PointArrayPrint[edge.v2][1]));
			//colorVec.emplace_back(1);
		}
	}
	//print BB
	if (BBprintflag)
	{
		std::vector<glm::vec4> BBPrint;
		BBPrint = BB.BB;
		for (int i = 0; i < 8; i++) {
			BBPrint[i] = transform * BB.BB[i];

			if (BBPrint[i].w != 0) {
				BBPrint[i].x = BBPrint[i].x / BBPrint[i].w;
				BBPrint[i].y = BBPrint[i].y / BBPrint[i].w;
				BBPrint[i].z = BBPrint[i].z / BBPrint[i].w;
				BBPrint[i].w = 1;
			}

		}
		BB.printBB(linesToPrint, BBPrint);
	}
	origenToprint = ScreenScale * camera.projectionMatrix * camera.viewMatrix * origen;
	if (origenToprint.w != 0) {
		origenToprint.x = origenToprint.x / origenToprint.w;
		origenToprint.y = origenToprint.y / origenToprint.w;
		origenToprint.z = origenToprint.z / origenToprint.w;
	}
	if (worldAxis) {
		origenx = ScreenScale * camera.projectionMatrix * camera.viewMatrix * TranslateMatrix(origenDefultSize, 0, 0) * origen;
		origeny = ScreenScale * camera.projectionMatrix * camera.viewMatrix * TranslateMatrix(0, origenDefultSize, 0) * origen;
		origenz = ScreenScale * camera.projectionMatrix * camera.viewMatrix * TranslateMatrix(0, 0, origenDefultSize) * origen;

		// Normalize coordinates (if needed)
		if (origenx.w != 0) {
			origenx.x /= origenx.w;
			origenx.y /= origenx.w;
			origenx.z /= origenx.w;
		}
		if (origeny.w != 0) {
			origeny.x /= origeny.w;
			origeny.y /= origeny.w;
			origeny.z /= origeny.w;
		}
		if (origenz.w != 0) {
			origenz.x /= origenz.w;
			origenz.y /= origenz.w;
			origenz.z /= origenz.w;
		}

		// Add axis points to vector
	}

	// Add object axis points if objectAxis is enabled
	if (objectAxis) {
		localX = { origenDefultSize / origenScale, 0.0f, 0.0f, 1.0f };
		localY = { 0.0f, origenDefultSize / origenScale , 0.0f, 1.0f };
		localZ = { 0.0f, 0.0f, origenDefultSize / origenScale, 1.0f };

		// Apply transformation
		localX = transform * localX;
		localY = transform * localY;
		localZ = transform * localZ;

		// Normalize coordinates (if needed)
		if (localX.w != 0) {
			localX.x /= localX.w;
			localX.y /= localX.w;
			localX.z /= localX.w;
		}
		if (localY.w != 0) {
			localY.x /= localY.w;
			localY.y /= localY.w;
			localY.z /= localY.w;
		}
		if (localZ.w != 0) {
			localZ.x /= localZ.w;
			localZ.y /= localZ.w;
			localZ.z /= localZ.w;
		}

		// Add object axis points to vector
	}
	if (shading != 0) {
		//print edges

		//print faces
		initializeZBuffer();

		int count = 0;
		unsigned int blue = objcolor[2]; // Extract Red
		unsigned int green = objcolor[1]; // Extract Green
		unsigned int red = objcolor[0];         // Extract Blue

		unsigned int adjustedColor;

		float red_diffuse = 0;
		float green_diffuse = 0;
		float blue_diffuse = 0;
		float red_specular = 0;
		float green_specular = 0;
		float blue_specular = 0;
		float b;
		float a;
		float den;

		float nearplane = camera.n;
		//zBuffer = std::vector<std::vector<double>>(screenwith * 8, std::vector<double>(screenhight * 8));
		std::vector<Pixel> pixelsToDraw;

		glm::vec3 lambda;
		glm::vec4 triangleNormalsToPrint;
		glm::vec3 vertexColors[3];
		glm::vec3 triangleCenter;

		glm::vec3 o;
		glm::vec3 l;
		glm::vec3 m;

		glm::vec3 I;
		glm::vec3 V;
		glm::vec3 N;
		glm::vec3 Rn;

		glm::vec3 L1;
		glm::vec3 L2;
		glm::vec3 pos;
		float Ia[3] = { Ka * light.amb_intencity[0], Ka * light.amb_intencity[1], Ka * light.amb_intencity[2] };
		for (const auto& face : parser.m_faces) {//the first 3 points are for object origen
			if (insideX[face.v[0]] + insideX[face.v[1]] + insideX[face.v[2]]==0) { continue; }
			if (insideY[face.v[0]] + insideY[face.v[1]] + insideY[face.v[2]]==0) { continue; }
			if (insideZ[face.v[0]] + insideZ[face.v[1]] + insideZ[face.v[2]]==0) { continue; }
			glm::vec4 p0 = PointArrayPrint[face.v[0]];
			glm::vec4 p1 = PointArrayPrint[face.v[1]];
			glm::vec4 p2 = PointArrayPrint[face.v[2]];
			p0.x += screenwith * 2;
			p0.y += screenhight * 2;
			p1.x += screenwith * 2;
			p1.y += screenhight * 2;
			p2.x += screenwith * 2;
			p2.y += screenhight * 2;
			o = translation * TotalRotationMatrix * TotalScaleMatrix * PointArray[face.v[0]];
			l = translation * TotalRotationMatrix * TotalScaleMatrix * PointArray[face.v[1]];
			m = translation * TotalRotationMatrix * TotalScaleMatrix * PointArray[face.v[2]];
			int minX = std::min(p0.x, std::min(p1.x, p2.x));
			if (minX < 0) { minX = 0;}
			int maxX = std::max(p0.x, std::max(p1.x, p2.x));
			if (maxX > 1540) { maxX =1540; }
			int minY = std::min(p0.y, std::min(p1.y, p2.y));
			if (minY < 0) { minY = 0; }
			int maxY = std::max(p0.y, std::max(p1.y, p2.y));
			if (maxY > 1000) { maxY = 1000; }
			



				if (shading == 1) {

					triangleNormalsToPrint = TotalRotationMatrix * TotalScaleMatrix * glm::vec4(glm::normalize(triangleNormals[count]), 0);
					N = glm::vec3(triangleNormalsToPrint);

					pos = glm::vec3(
						(o.x + l.x + m.x) / 3.0f,
						(o.y + l.y + m.y) / 3.0f,
						(o.z + l.z + m.z) / 3.0f
					);

					if (!L1mode) {
						L1 = glm::vec3(-light.light1[0], -light.light1[1], -light.light1[2]);
					}
					else {
						glm::vec3 pos1 = transform * glm::vec4(light.L1_pos[0], light.L1_pos[1], light.L1_pos[2], 1);
						L1 = glm::vec3(glm::normalize(glm::vec3(pos1) - pos));
					}
					glm::normalize(L1);
					red_diffuse = 0;
					green_diffuse = 0;
					blue_diffuse = 0;
					red_specular = 0;
					green_specular = 0;
					blue_specular = 0;

					V = glm::normalize(camera.position - pos); // Camera direction
					L1 = glm::normalize(L1);
					a = glm::dot(N, L1); 
					bool S = 1;
					if (doubleSide && a < 0) {
						a = -a;
						S = 0;
					}

					if (a > 0) {
						red_diffuse += light.L1_intencity[0] * a;
						green_diffuse += light.L1_intencity[1] * a;
						blue_diffuse += light.L1_intencity[2] * a;
					}

					if (S)
					{
						Rn = glm::normalize(glm::reflect(-L1, N));
					}
					else {
						Rn = glm::normalize(glm::reflect(L1, N));
					}
					b =  std::pow(std::max(glm::dot(Rn, V), 0.0f), n);
					if (doubleSide && b < 0) { b = -b; }
					if (b > 0) {
						red_specular += light.L1_intencity[0] * b;
						green_specular += light.L1_intencity[1] * b;
						blue_specular += light.L1_intencity[2] * b;
					}
					if (L2Toggle) {

						if (!L2mode) {
							L2 = glm::vec3(-light.light2[0], -light.light2[1], -light.light2[2]);
						}
						else {
							glm::vec3 pos2 = transform * glm::vec4(light.L2_pos[0], light.L2_pos[1], light.L2_pos[2],1);
							L2 = glm::vec3(pos2 - pos);
						}
						L2=glm::normalize(L2);
						N=glm::normalize(N);
						

						
						a = glm::dot(N, L2);
						bool S = 1;
						if (doubleSide && a < 0) {
							a = -a;
							S = 0;
						}
						if (a > 0) {
							red_diffuse += light.L2_intencity[0] * a;
							green_diffuse += light.L2_intencity[1] * a;
							blue_diffuse += light.L2_intencity[2] * a;
						}

						if (S)
						{
							Rn = glm::normalize(glm::reflect(-L2, N));
						}
						else {
							Rn = glm::normalize(glm::reflect(L2, N));
						}

						b =  std::pow(std::max(glm::dot(Rn, V), 0.0f), n);
						if (doubleSide && b < 0) { b = -b; }
						if (b > 0) {
							red_specular += light.L2_intencity[0] * b;
							green_specular += light.L2_intencity[1] * b;
							blue_specular += light.L2_intencity[2] * b;
						}
					
					}

					red = round(objcolor[0] * (Ia[0] + Kd*red_diffuse + Ks*red_specular));
					green = round(objcolor[1] * (Ia[1] + Kd*green_diffuse + Ks*green_specular));
					blue = round(objcolor[2] * (Ia[2] + Kd*blue_diffuse + Ks*blue_specular));

					if (red > 255) { red = 255; }
					if (green > 255) { green = 255; }
					if (blue > 255) { blue = 255; }

					adjustedColor = (255 << 24) + ((red) << 16) + ((green) << 8) + (blue);
					
					count++;
				}

				//goro P1
				if (shading == 2) {
					pos = glm::vec3(
						(o.x + l.x + m.x) / 3.0f,
						(o.y + l.y + m.y) / 3.0f,
						(o.z + l.z + m.z) / 3.0f
					);
					
					if (!L1mode) {
						L1 = glm::vec3(-light.light1[0], -light.light1[1], -light.light1[2]);
					}
					else {

						glm::vec3 pos1 = transform * glm::vec4(light.L1_pos[0], light.L1_pos[1], light.L1_pos[2], 1);
						L1 = glm::vec3(glm::normalize(glm::vec3(pos1) - pos));
					}
					glm::normalize(L1);
					glm::vec3 vertices[3] = { o, l, m };
					for (int i = 0; i < 3; ++i) {
						red_diffuse = 0;
						green_diffuse = 0;
						blue_diffuse = 0;
						red_specular = 0;
						green_specular = 0;
						blue_specular = 0;
						N = glm::normalize(vertixNormalToPrint[face.v[i]]);

						a = glm::dot(N, L1);

						bool S = 1;
						if (doubleSide && a < 0) {
							a = -a;
							S = 0;
						}

						if (a > 0) {
							red_diffuse += light.L1_intencity[0] * a;
							green_diffuse += light.L1_intencity[1] * a;
							blue_diffuse += light.L1_intencity[2] * a;
						}
					
						if (S)
						{
							Rn = glm::normalize(glm::reflect(-L1, N));
						}
						else {
							Rn = glm::normalize(glm::reflect(L1, N));
						}


						V = glm::normalize(camera.position - vertices[i]); // Camera direction
						b = pow(std::max(glm::dot(V, Rn), 0.0f), n);
						if (doubleSide && b < 0) { b = -b; }
						if (b > 0) {
							red_specular += light.L1_intencity[0] * b;
							green_specular += light.L1_intencity[1] * b;
							blue_specular += light.L1_intencity[2] * b;
						}

						if (L2Toggle) {
							if (!L2mode) {
								L2 = glm::vec3(-light.light2[0], -light.light2[1], -light.light2[2]);
							}
							else {
								glm::vec3 pos2 = transform * glm::vec4(light.L2_pos[0], light.L2_pos[1], light.L2_pos[2], 1);
								L2 = glm::vec3(glm::normalize(glm::vec3(pos2) - pos));
							}
							glm::normalize(L2);
							a = glm::dot(N, L2);
							bool S = 1;
							if (doubleSide && a < 0) {
								a = -a;
								S = 0;
							}

							if (a > 0) {
								red_diffuse += light.L2_intencity[0] * a;
								green_diffuse += light.L2_intencity[1] * a;
								blue_diffuse += light.L2_intencity[2] * a;
							}
							if (S)
							{
								Rn = glm::normalize(glm::reflect(-L2, N));
							}
							else {
								Rn = glm::normalize(glm::reflect(L2, N));
							}

							b = pow(std::max(glm::dot(V, Rn), 0.0f), n);
							if (doubleSide && b < 0) { b = -b; }
							if (b > 0) {
								red_specular += light.L2_intencity[0] * b;
								green_specular += light.L2_intencity[1] * b;
								blue_specular += light.L2_intencity[2] * b;
							}
						}
						red = round(objcolor[0] * (Ia[0] + Kd * red_diffuse + Ks * red_specular));
						green = round(objcolor[1] * (Ia[1] + Kd * green_diffuse + Ks * green_specular));
						blue = round(objcolor[2] * (Ia[2] + Kd * blue_diffuse + Ks * blue_specular));

						if (red > 255) { red = 255; }
						if (green > 255) { green = 255; }
						if (blue > 255) { blue = 255; }

						vertexColors[i] = glm::vec3(red, green, blue);
					}
				}

				ComputeBarycentricCoordinates2(p0, p1, p2, den, lambda);

				for (int a1 = minY; a1 <= maxY; ++a1) {
					for (int b1 = minX; b1 <= maxX; ++b1) {
						int x = b1;
						int y = a1;
						//if(x<0||y<0){continue;}
						float alpha = ((p1.y - p2.y) * (x - p2.x) + (p2.x - p1.x) * (y - p2.y)) / den;
						float beta = ((p2.y - p0.y) * (x - p2.x) + (p0.x - p2.x) * (y - p2.y)) / den;
						float gamma = 1 - alpha - beta;

						if (alpha >= 0 && beta >= 0 && gamma >= 0) {

							double fz;
							double p0z;
							double p1z;
							double p2z;
						//fz = (camera.projectionMatrix[2][2] * fz + camera.projectionMatrix[3][2]) / (camera.projectionMatrix[2][3] * fz + camera.projectionMatrix[3][3]);
						//fz /= 2;
						switch (BufferMode)
						{
						case 0:
							fz = alpha * p0.z + beta * p1.z + gamma * p2.z;
						break;
						case 1:

							
							fz = alpha * p0.z + beta * p1.z + gamma * p2.z;
							fz = double(2 * camera.n * camera.farPlane) / ((-fz * (camera.farPlane - camera.n)) + (camera.farPlane + camera.n));
						break;


						case 2:
							
							p0z = double(2 * camera.n * camera.farPlane) / ((-p0.z * (camera.farPlane - camera.n)) + (camera.farPlane + camera.n));

							p1z = double(2 * camera.n * camera.farPlane) / ((-p1.z * (camera.farPlane - camera.n)) + (camera.farPlane + camera.n));

							p2z = double(2 * camera.n * camera.farPlane) / ((-p2.z * (camera.farPlane - camera.n)) + (camera.farPlane + camera.n));
							fz = alpha * p0z + beta * p1z + gamma * p2z;


						default:
							break;
						}




						
						//std::cout << x << y << std::endl;
						if (fz < zBuffer[int(y)][int(x)]) {
								zBuffer[int(y)][int(x)] = fz;

								if (shading == 3) {
									red_diffuse = 0;
									green_diffuse = 0;
									blue_diffuse = 0;
									red_specular = 0;
									green_specular = 0;
									blue_specular = 0;
									
									N = glm::normalize(glm::vec3(
										alpha * vertixNormalToPrint[face.v[0]] +
										beta * vertixNormalToPrint[face.v[1]] +
										gamma * vertixNormalToPrint[face.v[2]]
									));
									pos = glm::vec3(alpha * o + beta * l + gamma * m);
									V = glm::normalize(camera.position - pos);
									if (!L1mode) {
										L1 = glm::vec3(-light.light1[0], -light.light1[1], -light.light1[2]);
									}
									else {
										glm::vec3 pos1 = transform * glm::vec4(light.L1_pos[0], light.L1_pos[1], light.L1_pos[2], 1);
										L1 = glm::vec3(glm::normalize(glm::vec3(pos1) - pos));
									}
									
									
									glm::normalize(L1);
									a = Kd * glm::dot(N, L1);
									bool S = 1;
									if (doubleSide && a < 0) { 
									a = -a;
									S = 0;
									}
									
									if (a > 0) {
										red_diffuse += light.L1_intencity[0] * a;
										green_diffuse += light.L1_intencity[1] * a;
										blue_diffuse += light.L1_intencity[2] * a;		
									}
									if (S)
									{
										Rn = glm::normalize(glm::reflect(-L1, N));
									}
									else {
										Rn = glm::normalize(glm::reflect(L1, N));
									}
									
									b = Ks * pow(std::max(glm::dot(V, Rn), 0.0f), n);
									if(doubleSide&&b<0){b=-b;}
									if (b > 0) {
										red_specular += light.L1_intencity[0] * b;
										green_specular += light.L1_intencity[1] * b;
										blue_specular += light.L1_intencity[2] * b;
									}

									
									if (L2Toggle) {
										if (!L2mode) {
											L2 = glm::vec3(-light.light2[0], -light.light2[1], -light.light2[2]);
										}
										else {
											glm::vec3 pos2 = transform * glm::vec4(light.L2_pos[0], light.L2_pos[1], light.L2_pos[2], 1);
											L2 = glm::vec3(glm::normalize(glm::vec3(pos2) - pos));
										}

										glm::normalize(L2);
										a = Kd * glm::dot(N, L2);
										bool S = 1;
										if (doubleSide && a < 0) {
											a = -a;
											S = 0;
										}

										if (a > 0) {
											red_diffuse += light.L2_intencity[0] * a;
											green_diffuse += light.L2_intencity[1] * a;
											blue_diffuse += light.L2_intencity[2] * a;
										}
										if (S)
										{
											Rn = glm::normalize(glm::reflect(-L2, N));
										}
										else {
											Rn = glm::normalize(glm::reflect(L2, N));
										}

										b = Ks * pow(std::max(glm::dot(V, Rn), 0.0f), n);
										if (b > 0||doubleSide) {
											red_specular += light.L2_intencity[0] * b;
											green_specular += light.L2_intencity[1] * b;
											blue_specular += light.L2_intencity[2] * b;
											}
										}

								red = round(objcolor[0] * (Ia[0] + red_diffuse + red_specular));
								green = round(objcolor[1] * (Ia[1] + green_diffuse + green_specular));
								blue = round(objcolor[2] * (Ia[2] + blue_diffuse + blue_specular));

								if (red > 255) { red = 255; }
								if (green > 255) { green = 255; }
								if (blue > 255) { blue = 255; }

								adjustedColor = (255 << 24) + ((int(red)) << 16) + ((int(green)) << 8) + (int(blue));
									
								}
								//goro part2
								if (shading == 2) {
									float red = alpha * vertexColors[0].x + beta * vertexColors[1].x + gamma * vertexColors[2].x;
									float green = alpha * vertexColors[0].y + beta * vertexColors[1].y + gamma * vertexColors[2].y;
									float blue = alpha * vertexColors[0].z + beta * vertexColors[1].z + gamma * vertexColors[2].z;
									adjustedColor = (255 << 24) + ((int(red)) << 16) + ((int(green)) << 8) + (int(blue));

								}
								pixelsToDraw.push_back({ x, y, adjustedColor });
							}

						}

					
				}

			}

		}

		renderer.drawPixels(pixelsToDraw);

	}
}

void Object::ComputeBarycentricCoordinates2(
	const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, float& den, glm::vec3& lambda) {
	den = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
	lambda.x = (b.y - c.y);
	lambda.y = (c.x - b.x);
	lambda.z = (c.y - b.y) * (c.x - a.x);
}

void Object::ActiveObjectTranslation(float x, float y, float z) {
	glm::vec4 worldTranslation(x, y, z, 1.0f);
	glm::vec4 localTranslation = TotalRotationMatrix * worldTranslation;
	origen = TranslateMatrix(localTranslation.x, localTranslation.y, localTranslation.z) * origen;
}

void Object::ActiveCameraTranslation(float cx, float cy, float cz)
{
	camera.position.x += cx;
	camera.position.y += cy;
	camera.position.z += cz;
	//print();
}

void Object::ActiveWorldTranslation(float x, float y, float z) {
	origen = TranslateMatrix(x, y, z) * origen;
}

void Object::updateCamera()
{
	camera.updateViewMatrix();
	camera.updateProjectionMatrix(screenwith, screenhight);
}
void Object::lookAtOrigen()
{
	camera.target.x = origen[0];
	camera.target.y = origen[1];
	camera.target.z = origen[2];
}

void Object::ActiveWorldRotaionX(float xDegree)
{
	float xRadians = glm::radians(xDegree);
	origen = RotationMatrix(xRadians, 'x') * origen;
}
void Object::ActiveWorldRotaionY(float yDegree)
{
	float yRadians = glm::radians(yDegree);
	origen = RotationMatrix(yRadians, 'y') * origen;
}
void Object::ActiveWorldRotaionZ(float zDegree)
{
	float zRadians = glm::radians(zDegree);
	origen = RotationMatrix(zRadians, 'z') * origen;
}

void Object::ActiveObjectRotaionX(float xDegree)
{
	float xRadians = glm::radians(xDegree);
	XRotationMatrix(xRadians);
}
void Object::ActiveObjectRotaionY(float yDegree)
{
	float yRadians = glm::radians(yDegree);
	YRotationMatrix(yRadians);
}
void Object::ActiveObjectRotaionZ(float zDegree)
{
	float zRadians = glm::radians(zDegree);
	ZRotationMatrix(zRadians);
}

void Object::ActiveWorldScale(float scale)
{
	glm::mat4 WorldScale = ScaleMatrix(scale, scale, scale);
	origen = WorldScale * origen;
	origenScale *= scale;
	TotalScaleMatrix *= WorldScale;
	/*for (int i = 0; i < PointArray.size(); i++) {
		PointArray[i] = WorldScale * PointArray[i];
		VectorPoints[i] = WorldScale * VectorPoints[i];

	}
	for (int i = 0; i < 8; i++) {
		BB.BB[i] = WorldScale * BB.BB[i];
	}*/
}
void Object::ActiveObjectScale(float scale)
{

	origenScale *= scale;
	TotalScaleMatrix *= ScaleMatrix(scale, scale, scale);
}

void Object::ReadObj(Wavefront_obj obj) {
	this->parser = obj;
	//canonaize 
	//bb is the diamiter in every axis so center is at the middle of the diamiter
	float temp_max[3] = { -100000,-100000,-100000 };
	float temp_min[3] = { 100000,100000,100000 };
	for (int i = 0; i < parser.m_points.size(); i++) {
		if (parser.m_points[i][0] > temp_max[0])
		{
			temp_max[0] = parser.m_points[i][0];
		}
		if (parser.m_points[i][1] > temp_max[1])
		{
			temp_max[1] = parser.m_points[i][1];
		}
		if (parser.m_points[i][2] > temp_max[2])
		{
			temp_max[2] = parser.m_points[i][2];
		}
		if (parser.m_points[i][0] < temp_min[0])
		{
			temp_min[0] = parser.m_points[i][0];
		}
		if (parser.m_points[i][1] < temp_min[1])
		{
			temp_min[1] = parser.m_points[i][1];
		}
		if (parser.m_points[i][2] < temp_min[2])
		{
			temp_min[2] = parser.m_points[i][2];
		}
	}

	origen[0] = (temp_max[0] + temp_min[0]) / 2.0f;
	origen[1] = (temp_max[1] + temp_min[1]) / 2.0f;
	origen[2] = (temp_max[2] + temp_min[2]) / 2.0f;

	//culc center of BB
	//building BB
	BB_diamiter[0] = temp_max[0] - temp_min[0];
	BB_diamiter[1] = temp_max[1] - temp_min[1];
	BB_diamiter[2] = temp_max[2] - temp_min[2];



	float diamitermax = BB_diamiter[0];
	if (BB_diamiter[1] > diamitermax) { diamitermax = BB_diamiter[1]; }
	if (BB_diamiter[2] > diamitermax) { diamitermax = BB_diamiter[2]; }

	const float factor = 10 / diamitermax;


	//saving BB
	BB.BB.emplace_back(temp_min[0], temp_min[1], temp_min[2], 1.0f);
	BB.BB.emplace_back(temp_min[0], temp_min[1], temp_max[2], 1.0f);
	BB.BB.emplace_back(temp_min[0], temp_max[1], temp_min[2], 1.0f);
	BB.BB.emplace_back(temp_min[0], temp_max[1], temp_max[2], 1.0f);
	BB.BB.emplace_back(temp_max[0], temp_min[1], temp_min[2], 1.0f);
	BB.BB.emplace_back(temp_max[0], temp_min[1], temp_max[2], 1.0f);
	BB.BB.emplace_back(temp_max[0], temp_max[1], temp_min[2], 1.0f);
	BB.BB.emplace_back(temp_max[0], temp_max[1], temp_max[2], 1.0f);

	//build cannoize prob can improve by culc directly and saving this mul but it works
	glm::mat4 cannonize = ScaleMatrix(factor, factor, factor) * TranslateMatrix(-origen[0], -origen[1], -origen[2]);

	// Assuming Wavefront_obj contains vertex data as a vector of structs
	// Replace `parser.vertex_data` with the correct member for vertex data
	for (int i = 0; i < parser.m_points.size(); i++) {
		PointArray.emplace_back(parser.m_points[i][0], parser.m_points[i][1], parser.m_points[i][2], 1.0f);
	}
	for (int i = 0; i < parser.m_points.size(); i++) {
		PointArray[i] = cannonize * PointArray[i];
		//PointArray[i].w = 1.0f;
	}
	for (int i = 0; i < 8; i++) {
		BB.BB[i] = cannonize * BB.BB[i];
		BB.BB[i].w = 1.0f;
	}

	
	int count = 1;
	for (const auto& face : obj.m_faces) {
		vertexAdjacency[face.v[0] + 1].push_back(count);
		vertexAdjacency[face.v[1] + 1].push_back(count);
		vertexAdjacency[face.v[2] + 1].push_back(count);
		count++;
		// Retrieve the positions of the vertices
		glm::vec3 v0(obj.m_points[face.v[0]][0], obj.m_points[face.v[0]][1], obj.m_points[face.v[0]][2]);
		glm::vec3 v1(obj.m_points[face.v[1]][0], obj.m_points[face.v[1]][1], obj.m_points[face.v[1]][2]);
		glm::vec3 v2(obj.m_points[face.v[2]][0], obj.m_points[face.v[2]][1], obj.m_points[face.v[2]][2]);

		// Compute the edge vectors
		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;

		// Compute the cross product of the edge vectors to get the triangle normal
		glm::vec3 triangleNormal = glm::cross(edge1, edge2);
		float area = 0.5f * glm::length(triangleNormal);
		// Store the triangle normal
		triangleNormals.push_back(triangleNormal);


		triangleAreas.push_back(area);
		// Print the triangle normal (optional for debugging)
		//std::cout << "Triangle normal: (" << triangleNormal.x << ", " << triangleNormal.y << ", " << triangleNormal.z <<",area:"<< area<< ")\n";

	}
	//print adjecency list
	for (int i = 0; i < obj.m_points.size(); ++i) {  // Iterate over the vertex indices from 1 to m_points.size()
		// Note: i corresponds to 0-based index, so we print i+1 for 1-based index
		int vertexIndex = i + 1;  // Convert to 1-based index
		const std::vector<int>& adjacentFaces = vertexAdjacency[i + 1];
	}

	// Iterate over each vertex
	for (size_t i = 0; i < obj.m_points.size(); ++i) {
		glm::vec3 accumulatedNormal(0.0f);  // Accumulator for the vertex normal
		const std::vector<int>& adjacentFaces = vertexAdjacency[i + 1];

		// Get the faces adjacent to this vertex (assume you already have adjacency list)
		for (size_t j = 0; j < adjacentFaces.size(); ++j) {  // adjacencyList[i] holds the faces adjacent to vertex i
			float triangleArea = triangleAreas[adjacentFaces[j] - 1];
			// Accumulate the weighted normal (by area)
			accumulatedNormal += triangleNormals[adjacentFaces[j] - 1] * triangleArea;
		}
		vertixNormal.push_back(glm::vec4(glm::normalize(accumulatedNormal),0));
	}
	for (const auto& face : this->parser.m_faces) {
		edgeSet.insert(Edge(face.v[0], face.v[1]));
		edgeSet.insert(Edge(face.v[1], face.v[2]));
		edgeSet.insert(Edge(face.v[2], face.v[0]));
	}


	for (size_t i = 0; i < vertixNormal.size(); i++)
	{
		VectorPoints.push_back((vertixNormal[i])+PointArray[i]);
	}
	origen =glm::vec4(0, 0, 0, 1);
}
