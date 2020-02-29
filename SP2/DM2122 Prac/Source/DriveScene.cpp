#include "DriveScene.h"
#include "GL\glew.h"
#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <string>
#include <stdio.h>   
#include <stdlib.h>  
#include <time.h>

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

DriveScene::DriveScene()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

DriveScene::~DriveScene()
{
}

void DriveScene::Init()
{
	srand(time(NULL));

	scenechange = false;

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);

	m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

	//m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Texture.fragmentshader"); 

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	//light 1
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1ENABLED] = glGetUniformLocation(m_programID, "light1Enabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");

	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	light[0].type = Light::LIGHT_DIRECTIONAL;
	light[0].position.Set(10, 10, 10);
	light[0].color.Set(0.5f, 0.5f, 0.5f);
	light[0].power = 1;
	light[0].kC = 1.f;
	light[0].kL = 0.01f;
	light[0].kQ = 0.001f;
	light[0].cosCutoff = cos(Math::DegreeToRadian(45));
	light[0].cosInner = cos(Math::DegreeToRadian(30));
	light[0].exponent = 10.f;
	light[0].spotDirection.Set(0.f, 1.f, 0.f);
	//light 1
	light[1].type = Light::LIGHT_POINT;
	light[1].position.Set(0, 28, -30);
	light[1].color.Set(0.5f, 0.5f, 0.5f);
	light[1].power = 1.f;
	light[1].kC = 1.f;
	light[1].kL = 0.01f;
	light[1].kQ = 0.001f;
	light[1].cosCutoff = cos(Math::DegreeToRadian(45));
	light[1].cosInner = cos(Math::DegreeToRadian(30));
	light[1].exponent = 30.f;
	light[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
	glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &light[0].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);
	//light 1
	glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
	glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &light[1].spotDirection.x);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);
	glUniform1i(m_parameters[U_NUMLIGHTS], 2);

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//trackleft.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//trackright.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//tracktop.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//trackbottom.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//trackfront.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//trackback.tga");

	meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 0.f), 9, 36, 1.f);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	playerdetails.GetData();

	Acarnumber = playerdetails.car_number.cartype;

	if (Acarnumber == 0)
	{
		meshList[GEO_CARBODY] = MeshBuilder::GenerateOBJ("Car1", "OBJ//GuangThengCarBody.obj");
		meshList[GEO_CARBODY]->textureID = LoadTGA("Image//GuangThengCarTex.tga");
		meshList[GEO_CARBODY]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARBODY]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kShininess = 1.f;
		ACarBody.translate = Vector3(-235, 2.05, 235);
		ACarBody.Scale = Vector3(1.55, 1.55, 1.55);
		ACarBody.RotateY.degree = 0;
		Loadcoord("OBJ//GuangThengCarBody.obj", CCarBody);

		meshList[GEO_CARWHEEL] = MeshBuilder::GenerateOBJ("Car1Wheel", "OBJ//GuangThengCarWheel.obj");
		meshList[GEO_CARWHEEL]->textureID = LoadTGA("Image//GuangThengCarTex.tga");
		meshList[GEO_CARWHEEL]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARWHEEL]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kShininess = 1.f;
		ACarWheel[0].translate = Vector3(2, -0.4, -2.8);
		ACarWheel[1].translate = Vector3(-2.1, -0.4, -2.8);
		ACarWheel[2].translate = Vector3(2.05, -0.4, 2.8);
		ACarWheel[2].Scale = Vector3(1.18, 1.18, 1.18);
		ACarWheel[3].translate = Vector3(-2.15, -0.4, 2.8);
		ACarWheel[3].Scale = Vector3(1.18, 1.18, 1.18);
	}
	else if (Acarnumber == 1)
	{
		meshList[GEO_CARBODY] = MeshBuilder::GenerateOBJ("Car2", "OBJ//RyanCarBody.obj");
		meshList[GEO_CARBODY]->textureID = LoadTGA("Image//RyanCarBodyTex.tga");
		meshList[GEO_CARBODY]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARBODY]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kShininess = 1.f;
		ACarBody.translate = Vector3(-235, 3.2, 235);
		ACarBody.Scale = Vector3(2, 2, 2);
		ACarBody.RotateY.degree = 0;
		Loadcoord("OBJ//RyanCarBody.obj", CCarBody);

		meshList[GEO_CARWHEEL] = MeshBuilder::GenerateOBJ("Car2Wheel", "OBJ//RyanCarWheel.obj");
		meshList[GEO_CARWHEEL]->textureID = LoadTGA("Image//RyanCarWheelTex.tga");
		meshList[GEO_CARWHEEL]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARWHEEL]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kShininess = 1.f;
		ACarWheel[0].translate = Vector3(0.7, -0.95, -1.2);
		ACarWheel[1].translate = Vector3(-1.3, -0.95, -1.2);
		ACarWheel[2].translate = Vector3(0.75, -0.95, 1.7);
		ACarWheel[3].translate = Vector3(-1.3, -0.95, 1.7);
	}
	else if (Acarnumber == 2)
	{
		meshList[GEO_CARBODY] = MeshBuilder::GenerateOBJ("Car3", "OBJ//JCCarBody.obj");
		meshList[GEO_CARBODY]->textureID = LoadTGA("Image//JCCarTex.tga");
		meshList[GEO_CARBODY]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARBODY]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kShininess = 1.f;
		ACarBody.translate = Vector3(-235, 3, 235);
		ACarBody.Scale = Vector3(2, 2, 2);
		ACarBody.RotateY.degree = 0;
		Loadcoord("OBJ//JCCarBody.obj", CCarBody);

		meshList[GEO_CARWHEEL] = MeshBuilder::GenerateOBJ("Car3Wheel", "OBJ//JCCarWheel.obj");
		meshList[GEO_CARWHEEL]->textureID = LoadTGA("Image//JCCarTex.tga");
		meshList[GEO_CARWHEEL]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARWHEEL]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kShininess = 1.f;
		ACarWheel[0].translate = Vector3(1.8, -0.86, -2.1);
		ACarWheel[1].translate = Vector3(-1.8, -0.86, -2.1);
		ACarWheel[2].translate = Vector3(1.8, -0.86, 2.1);
		ACarWheel[3].translate = Vector3(-1.8, -0.86, 2.1);
	}
	else if (Acarnumber == 3)
	{
		meshList[GEO_CARBODY] = MeshBuilder::GenerateOBJ("Car4", "OBJ//JianFengCarBody.obj");
		meshList[GEO_CARBODY]->textureID = LoadTGA("Image//JianFengCarTex.tga");
		meshList[GEO_CARBODY]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARBODY]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARBODY]->material.kShininess = 1.f;
		ACarBody.translate = Vector3(-235, 4.7, 235);
		ACarBody.Scale = Vector3(1.7, 1.7, 1.7);
		ACarBody.RotateY.degree = 0;
		Loadcoord("OBJ//JianFengCarBody.obj", CCarBody);

		meshList[GEO_CARWHEEL] = MeshBuilder::GenerateOBJ("Car4Wheel", "OBJ//JianFengCarWheel.obj");
		meshList[GEO_CARWHEEL]->textureID = LoadTGA("Image//JianFengCarTex.tga");
		meshList[GEO_CARWHEEL]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
		meshList[GEO_CARWHEEL]->material.kDiffuse.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kSpecular.Set(1.f, 1.f, 1.f);
		meshList[GEO_CARWHEEL]->material.kShininess = 1.f;
		ACarWheel[0].translate = Vector3(1.45, -1.5, -2.5);
		ACarWheel[1].translate = Vector3(-1.45, -1.5, -2.5);
		ACarWheel[2].translate = Vector3(1.55, -1.35, 2.5);
		ACarWheel[2].Scale = Vector3(1.2, 1.2, 1.2);
		ACarWheel[3].translate = Vector3(-1.77, -1.35, 2.5);
		ACarWheel[3].Scale = Vector3(1.2, 1.2, 1.2);
	}
	health = 10;
	iFrames = 0;

	meshList[GEO_SPIKE] = MeshBuilder::GenerateOBJ("spike", "OBJ//spike.obj");
	meshList[GEO_SPIKE]->textureID = LoadTGA("Image//spike.tga");
	meshList[GEO_SPIKE]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SPIKE]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_SPIKE]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_SPIKE]->material.kShininess = 1.f;
	Loadcoord("OBJ//spike.obj", CSpike);
	Spike.translate.Set(-172, -4, 178);
	Spike.Scale.Set(3, 1, 3);
	spikelist.addItem(Spike);
	Spike.translate.Set(-76, -4, 114);
	spikelist.addItem(Spike);
	Spike.translate.Set(-76, -4, -46);
	spikelist.addItem(Spike);
	Spike.translate.Set(-12, -4, -46);
	spikelist.addItem(Spike);
	Spike.translate.Set(52, -4, -46);
	spikelist.addItem(Spike);
	Spike.translate.Set(116, -4, -142);
	spikelist.addItem(Spike);
	Spike.translate.Set(148, -4, -110);
	spikelist.addItem(Spike);
	Spike.translate.Set(116, -4, 114);
	spikelist.addItem(Spike);
	Spike.translate.Set(212, -4, 50);
	spikelist.addItem(Spike);
	Spike.translate.Set(212, -4, 18);
	spikelist.addItem(Spike);
	Spike.translate.Set(-236, -4, -174);
	spikelist.addItem(Spike);

	coingeneratearray = true;
	Generatecoinposition();
	meshList[GEO_COIN] = MeshBuilder::GenerateOBJ("coin", "OBJ//coin.obj");
	meshList[GEO_COIN]->textureID = LoadTGA("Image//coin.tga");
	meshList[GEO_COIN]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_COIN]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_COIN]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_COIN]->material.kShininess = 1.f;
	Loadcoord("OBJ//coin.obj", CCoin);
	coin.Scale.Set(3, 3, 3);
	CoinpositionSelector(coingeneratearray);
	coingeneratearray = false;
	coinchecker = 0;
	randomcoin = 0;

	meshList[GEO_BOX] = MeshBuilder::GenerateOBJ("box", "OBJ//box.obj");
	meshList[GEO_BOX]->textureID = LoadTGA("Image//box.tga");
	meshList[GEO_BOX]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BOX]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_BOX]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_BOX]->material.kShininess = 1.f;
	Loadcoord("OBJ//box.obj", CBox);
	Box.translate.Set(-204, -4, 210);
	Box.Scale.Set(3, 3, 3);
	boxlist.addItem(Box);
	Box.translate.Set(-204,-4, 178);
	boxlist.addItem(Box);

	Box.translate.Set(-140,-4, 145);
	boxlist.addItem(Box);	
	Box.translate.Set(-140,-4, 177);
	boxlist.addItem(Box);	
	Box.translate.Set(-140,-4, 210);
	boxlist.addItem(Box);	
	Box.translate.Set(-108,-4, 210);
	boxlist.addItem(Box);
	Box.translate.Set(-76,-4, 210);
	boxlist.addItem(Box);
	Box.translate.Set(-44,-4, 210);
	boxlist.addItem(Box);
	Box.translate.Set(-12,-4, 210);
	boxlist.addItem(Box);

	Box.translate.Set(84,-4, 210);
	boxlist.addItem(Box);
	Box.translate.Set(116,-4, 210);
	boxlist.addItem(Box);
	Box.translate.Set(148,-4, 210);
	boxlist.addItem(Box);
	Box.translate.Set(212,-4, 210);
	boxlist.addItem(Box);

	Box.translate.Set(212,-4, 178);
	boxlist.addItem(Box);
	Box.translate.Set(212,-4, 146);
	boxlist.addItem(Box);
	Box.translate.Set(212,-4, 114);
	boxlist.addItem(Box);
	Box.translate.Set(212,-4, 82);
	boxlist.addItem(Box);
	Box.translate.Set(212,-4, -14);
	boxlist.addItem(Box);
	Box.translate.Set(212,-4, -110);
	boxlist.addItem(Box);
	Box.translate.Set(212,-4, -142);
	boxlist.addItem(Box);
	Box.translate.Set(212,-4, -174);
	boxlist.addItem(Box);
	Box.translate.Set(212,-4, -206);
	boxlist.addItem(Box);

	Box.translate.Set(148,-4, -206);
	boxlist.addItem(Box);
	Box.translate.Set(116,-4, -206);
	boxlist.addItem(Box);
	Box.translate.Set(84,-4, -206);
	boxlist.addItem(Box);
	Box.translate.Set(52,-4, -206);
	boxlist.addItem(Box);
	Box.translate.Set(20,-4, -206);
	boxlist.addItem(Box);
	Box.translate.Set(-76,-4, -206);
	boxlist.addItem(Box);
	Box.translate.Set(-108,-4, -206);
	boxlist.addItem(Box);
	Box.translate.Set(-140,-4, -206);
	boxlist.addItem(Box);
	Box.translate.Set(-204,-4, -206);
	boxlist.addItem(Box);

	Box.translate.Set(-204,-4, -174);
	boxlist.addItem(Box);	
	Box.translate.Set(-204,-4, -142);
	boxlist.addItem(Box);	
	Box.translate.Set(-204,-4, -78);
	boxlist.addItem(Box);	
	Box.translate.Set(-204,-4, -46);
	boxlist.addItem(Box);	
	Box.translate.Set(-204,-4, 18);
	boxlist.addItem(Box);	
	Box.translate.Set(-204,-4, 50);
	boxlist.addItem(Box);	
	Box.translate.Set(-204,-4, 82);
	boxlist.addItem(Box);

	Box.translate.Set(-172,-4, 18);
	boxlist.addItem(Box);  
	Box.translate.Set(-172,-4, 82);
	boxlist.addItem(Box);  
	Box.translate.Set(-140,-4, 82);
	boxlist.addItem(Box);
	Box.translate.Set(-44,-4, 82);
	boxlist.addItem(Box);
	Box.translate.Set(-12,-4, 82);
	boxlist.addItem(Box);
	Box.translate.Set(20,-4, 82);
	boxlist.addItem(Box);
	Box.translate.Set(52,-4, 82);
	boxlist.addItem(Box);
	Box.translate.Set(116,-4, 82);
	boxlist.addItem(Box);
	Box.translate.Set(148,-4, 82);
	boxlist.addItem(Box);

	Box.translate.Set(148,-4, -142);
	boxlist.addItem(Box);
	Box.translate.Set(148,-4, -78);
	boxlist.addItem(Box);
	Box.translate.Set(148,-4, -46);
	boxlist.addItem(Box);
	Box.translate.Set(148,-4, 18);
	boxlist.addItem(Box);
	Box.translate.Set(148,-4, 50);
	boxlist.addItem(Box);
	Box.translate.Set(148,-4, 114);
	boxlist.addItem(Box);

	Box.translate.Set(116,-4, -78);
	boxlist.addItem(Box);
	Box.translate.Set(84,-4, -78);
	boxlist.addItem(Box);
	Box.translate.Set(52,-4, -78);
	boxlist.addItem(Box);
	Box.translate.Set(-12,-4, -78);
	boxlist.addItem(Box);
	Box.translate.Set(-44,-4, -78);
	boxlist.addItem(Box);
	Box.translate.Set(-76,-4, -78);
	boxlist.addItem(Box);

	Box.translate.Set(-44,-4, -110);
	boxlist.addItem(Box);
	Box.translate.Set(-44,-4, -46);
	boxlist.addItem(Box);
	Box.translate.Set(-44,-4, -14);
	boxlist.addItem(Box);

	Box.translate.Set(84,-4, -110);
	boxlist.addItem(Box);
	Box.translate.Set(84,-4, -142);
	boxlist.addItem(Box);
	Box.translate.Set(84,-4, -46);
	boxlist.addItem(Box);
	Box.translate.Set(84,-4, -14);
	boxlist.addItem(Box);

	Box.translate.Set(-44,-4, 114);
	boxlist.addItem(Box);
	Box.translate.Set(-44,-4, 146);
	boxlist.addItem(Box);

	Box.translate.Set(20,-4, 50);
	boxlist.addItem(Box);
	Box.translate.Set(20,-4, 18);
	boxlist.addItem(Box);

	Box.translate.Set(20,-4, 146);
	boxlist.addItem(Box);
	Box.translate.Set(84,-4, 146);
	boxlist.addItem(Box);

	Box.translate.Set(-108,-4, -142);
	boxlist.addItem(Box);
	Box.translate.Set(-140,-4, -142);
	boxlist.addItem(Box);
	Box.translate.Set(-140,-4, -110);
	boxlist.addItem(Box);

	Box.translate.Set(20,-4, -174);
	boxlist.addItem(Box);

	Box.translate.Set(-108,-4, -14);
	boxlist.addItem(Box);

	meshList[GEO_BOOSTPAD] = MeshBuilder::GenerateOBJ("boost", "OBJ//boostpad.obj");
	meshList[GEO_BOOSTPAD]->textureID = LoadTGA("Image//boostpad.tga");
	meshList[GEO_BOOSTPAD]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_BOOSTPAD]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_BOOSTPAD]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_BOOSTPAD]->material.kShininess = 1.f;
	Loadcoord("OBJ//boostpad.obj", CBoostpad);

	Boostpad.translate.Set(-44, -4, 178);
	Boostpad.Scale.Set(3, 1, 3);
	boostpadlist.addItem(Boostpad);
	Boostpad.translate.Set(-76, -4, 242);
	boostpadlist.addItem(Boostpad);
	Boostpad.translate.Set(20, -4, 114);
	boostpadlist.addItem(Boostpad);
	Boostpad.translate.Set(180, -4, 114);
	boostpadlist.addItem(Boostpad);
	Boostpad.translate.Set(84, -4, -174);
	boostpadlist.addItem(Boostpad);
	Boostpad.translate.Set(84, -4, -238);
	boostpadlist.addItem(Boostpad);
	Boostpad.translate.Set(-108, -4, -174);
	boostpadlist.addItem(Boostpad);
	Boostpad.translate.Set(-204, -4, -14);
	boostpadlist.addItem(Boostpad);

	Generatepowerposition();
	meshList[GEO_GHOSTCOIN] = MeshBuilder::GenerateOBJ("coin", "OBJ//ghostpowerup.obj");
	meshList[GEO_GHOSTCOIN]->textureID = LoadTGA("Image//ghostpowerup.tga");
	meshList[GEO_GHOSTCOIN]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_GHOSTCOIN]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_GHOSTCOIN]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_GHOSTCOIN]->material.kShininess = 1.f;
	Loadcoord("OBJ//ghostpowerup.obj", CPowerups[0]);
	if (randompower == 0)
	{
		Powerups[0].translate.Set(-140, 6, 114);
	}
	if (randompower == 1)
	{
		Powerups[0].translate.Set(-204, 6, -110);
	}
	if (randompower == 2)
	{
		Powerups[0].translate.Set(20, 6, -46);
	}
	if (randompower == 3)
	{
		Powerups[0].translate.Set(84, 6, 82);
	}
	if (randompower == 4)
	{
		Powerups[0].translate.Set(180, 6, 210);
	}
	Powerups[0].Scale.Set(3, 3, 3);

	Generatepowerposition();
	meshList[GEO_SHIELDCOIN] = MeshBuilder::GenerateOBJ("shield", "OBJ//shieldpowerup.obj");
	meshList[GEO_SHIELDCOIN]->textureID = LoadTGA("Image//shieldpower.tga");
	meshList[GEO_SHIELDCOIN]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SHIELDCOIN]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_SHIELDCOIN]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_SHIELDCOIN]->material.kShininess = 1.f;
	Loadcoord("OBJ//shieldpowerup.obj", CPowerups[1]);
	if (randompower == 0)
	{
		Powerups[1].translate.Set(-172, 10, 210);
	}
	if (randompower == 1)
	{
		Powerups[1].translate.Set(20, 10, 178);
	}
	if (randompower == 2)
	{
		Powerups[1].translate.Set(240, 10, 114);
	}
	if (randompower == 3)
	{
		Powerups[1].translate.Set(180, 10, -110);
	}
	if (randompower == 4)
	{
		Powerups[1].translate.Set(-44, 10, -142);
	}
	Powerups[1].Scale.Set(3, 3, 3);

	meshList[GEO_SHIELD] = MeshBuilder::GenerateOBJ("shieldparticle", "OBJ//shieldparticle.obj");
	meshList[GEO_SHIELD]->textureID = LoadTGA("Image//coin.tga");
	meshList[GEO_SHIELD]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SHIELD]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_SHIELD]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_SHIELD]->material.kShininess = 1.f;
	AShields[0].translate = Vector3(0, 0, -5);
	AShields[1].translate = Vector3(-5, 0, 0);
	AShields[2].translate = Vector3(5, 0, 0);
	AShields[3].translate = Vector3(0, 0, 5);
	AShields[0].RotateY.degree = 180;
	AShields[1].RotateY.degree = 270;
	AShields[2].RotateY.degree = 90;
	AShields[3].RotateY.degree = 0;

	Powerup_onmap[0] = Powerup_onmap[1] = false;
	Powerup_onplayer[0] = Powerup_onplayer[1] = true;
	shield_health = 3;
	ghost_time = 0;
	collision_in_box_in_frame = 1;

	//StatLevel[0]: acceleration, [1]: Max speed, [2]: Turbo, [3]: Max Fuel, [4]: Current Fuel
	maxvelocity = playerdetails.car_number.SelectedCar.StatLevel[1] * 20.f;
	carVelocity = 0.f;
	carTurningSpeed = 135.f;
	carAcceleration = playerdetails.car_number.SelectedCar.StatLevel[0] * 10.f;
	friction = 10.f;
	boostVelocity = 0.f;
	boostAcceleration = playerdetails.car_number.SelectedCar.StatLevel[2];
	fuel = playerdetails.car_number.SelectedCar.StatLevel[4] * 2000.f;
	car_ismoving = false;

	countDown = 50;
	carcanmove = true;
	
	test.Init(ACarBody.translate + Vector3(0, 5, 30), ACarBody.translate + Vector3(0, 5, 0), Vector3(0, 1, 0));

	showcointexttime = 0;
	showtext = GetTickCount() + 3000;
	coinrespawned = "A coin has respawned!";
	startingtext = "Collect all 10 coins around the field~without destroying your car ~  or running out of fuel";

	Cskybox.Set(-262, 0, -262, 262, 0, 262);
	Askybox.translate = Vector3(0, 0, 0);

	playerdetails.coinCounter = 0;

	if (playsound == true)
	{
		PlaySound(TEXT("Music/Cycle.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	}
}

void DriveScene::Update(double dt)
{
	//std::cout << fuel << std::endl;
	if (Application::IsKeyPressed(0x31))
	{
		glDisable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(0x32))
	{
		glEnable(GL_CULL_FACE);
	}
	else if (Application::IsKeyPressed(0x33))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (Application::IsKeyPressed(0x34))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (carcanmove == true)
	{
		// Increase Car Velocity to move forward
		if (Application::IsKeyPressed('W'))
		{
			// If car was moving backwards and player wants to move forward
			if (carVelocity < 0)
			{
				carVelocity += (2 * ((carAcceleration * dt) + (friction * dt)));
				car_ismoving = true;
			}
			// Move forward normally
			else
			{
				if (carVelocity < maxvelocity)
				{
					carVelocity += (carAcceleration * dt);
					car_ismoving = true;
				}
			}

			// Animation for car wheel
			for (int i = 0; i < 2; i++)
			{
				ACarWheel[i].RotateY.degree = 0;
			}
		}
		// Decrease Car Velocity to move backwards
		if (Application::IsKeyPressed('S'))
		{
			// If car was moving forwards and player wants to move backward
			if (carVelocity > 0)
			{
				carVelocity -= (2 * ((carAcceleration * dt) + (friction * dt)));
				car_ismoving = true;
			}
			// Move backward normally
			else
			{
				if (carVelocity > -maxvelocity)
				{
					carVelocity -= (carAcceleration * dt);
					car_ismoving = true;
				}
			}
			// Animation for car wheel
			for (int i = 0; i < 2; i++)
			{
				ACarWheel[i].RotateY.degree = 0;
			}
		}
		// Turn car to the left
		if (carVelocity != 0.f && Application::IsKeyPressed('A'))
		{
			ACarBody.RotateY.degree += (float)(carTurningSpeed * dt);

			// Animation for car wheel
			for (int i = 0; i < 2; i++)
			{
				ACarWheel[i].RotateY.degree = 30;
			}
			car_ismoving = true;
		}
		// Turn car to the right
		if (carVelocity != 0.f && Application::IsKeyPressed('D'))
		{
			ACarBody.RotateY.degree -= (float)(carTurningSpeed * dt);

			// Animation for car wheel
			for (int i = 0; i < 2; i++)
			{
				ACarWheel[i].RotateY.degree = -30;
			}
			car_ismoving = true;
		}
		//Nitro booster
		if (Application::IsKeyPressed(VK_SPACE))
		{
			if (carVelocity < (maxvelocity * 1.5f))
			{
				boostVelocity += boostAcceleration;
				carVelocity += boostVelocity;
			}
			fuel -= dt * 150;
		}
		if (!Application::IsKeyPressed(VK_SPACE) && boostVelocity > 0)
		{
			carVelocity -= boostVelocity;
			boostVelocity = 0;
		}
		// If car is moving without key inputs, increase/decrease car velocity to being the car to a stop
		if (!Application::IsKeyPressed('W') && !Application::IsKeyPressed('S'))
		{
			if (carVelocity < 0.f)
			{
				carVelocity += (2 * (friction * dt));
				if (carVelocity > 0.f)
				{
					carVelocity = 0.f;

				}
			}
			else if (carVelocity > 0.f)
			{
				carVelocity -= (2 * (friction * dt));
				if (carVelocity < 0.f)
				{
					carVelocity = 0.f;
				}
			}
			// Animation for car wheel
			for (int i = 0; i < 2; i++)
			{
				ACarWheel[i].RotateY.degree = 0;
			}
		}
		if (carVelocity == 0)
		{
			car_ismoving = false;
		}
		//Fuel decreasing
		if (car_ismoving) fuel -= 15 * dt;
		
		//Car Moving
		carMovement(ACarBody, carVelocity, dt);
	}
	if (health <= 0)
	{
		carcanmove = false;
		countDown -= dt * 10;
		if (countDown == 0)
		{
			scenenumber = 3;
			scenechange = true;
		}
		//end driving
	}
	if (fuel <= 0)
	{
		carcanmove = false;
		countDown -= dt * 10;
		if (countDown == 0)
		{
			scenenumber = 3;
			scenechange = true;
		}
		//end driving
	}

	//ghost and shield interaction
	for (int i = 0; i < 2; ++i) {
		if (Powerup_onmap[i]) {
			if (collision_detector(ACarBody, CCarBody, Powerups[i], CPowerups[i], true)) {
				Powerup_onmap[i] = false;
				Powerup_onplayer[i] = true;
				if (i == 0)
					ghost_time = GetTickCount() * 0.001f;
			}
		}
	}
	if ((GetTickCount() * 0.001f - ghost_time) > 5.0f) {
		Powerup_onplayer[0] = 0;
	}
	if (shield_health == 0) {
		Powerup_onplayer[1] = 0;
	}

	if (Powerup_onplayer[1]) {
		Mtx44 rotation;
		float turn = (float)(shield_turning_speed * dt);
		rotation.SetToRotation(turn, 0, 1, 0);
		for (int i = 0; i < 4; ++i) {
			AShields[i].translate = rotation * AShields[i].translate;
			AShields[i].RotateY += turn;
		}
	}

	CoinRespawn();
	
	test.CarUpdate(dt, ACarBody);
	playerdetails.Update((float)fuel / 2000.f);
}

void DriveScene::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	viewStack.LookAt(test.position.x, test.position.y, test.position.z, test.target.x, test.target.y, test.target.z, test.up.x, test.up.y, test.up.z);
	
	modelStack.LoadIdentity();

	modelStack.PushMatrix();
	modelStack.Translate(0, 50, 0);
	RenderSkybox();
	modelStack.PopMatrix();

	RenderObj(meshList[GEO_CARBODY], ACarBody, false, false);
	for (int carnumwheel = 0; carnumwheel < 4; carnumwheel++)
	{
		RenderObj(meshList[GEO_CARWHEEL], ACarWheel[carnumwheel], true, false);
	}
	for (int shieldnum = 0; shieldnum < 4; shieldnum++)
	{
		RenderObj(meshList[GEO_SHIELD], AShields[shieldnum], true, false);
	}
	modelStack.PopMatrix();

	for (CNode* current = boxlist.gethead(); current != nullptr; current = current->getnext())
	{
		RenderObj(meshList[GEO_BOX], current->transformation, true, false);
	}
	for (CNode* current = spikelist.gethead(); current != nullptr; current = current->getnext())
	{
		RenderObj(meshList[GEO_SPIKE], current->transformation, true, false);
	}
	for (CNode* current = boostpadlist.gethead(); current != nullptr; current = current->getnext())
	{
		RenderObj(meshList[GEO_BOOSTPAD], current->transformation, true, false);
	}
	for (CNode* current = coinlist.gethead(); current != nullptr; current = current->getnext())
	{
		RenderObj(meshList[GEO_COIN], current->transformation, true, false);
	}
	if (Powerup_onmap[0]) {
		RenderObj(meshList[GEO_GHOSTCOIN], Powerups[0], true, false);
	}
	if (Powerup_onmap[1]) {
		RenderObj(meshList[GEO_SHIELDCOIN], Powerups[1], true, false);
	}

	if (showtext > GetTickCount())
	{
		RenderTextOnScreen(meshList[GEO_TEXT], startingtext, Color(0, 1, 0), 3, 0, 15);
	}
	if (GetTickCount() < showcointexttime)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], coinrespawned, Color(0, 1, 0), 3, 2, 10);
	}

	RenderTextOnScreen(meshList[GEO_TEXT], "  x" + std::to_string(playerdetails.coinCounter), Color(0, 1, 0), 2, 0, 4); //coins
	RenderMeshOnScreen(meshList[GEO_COIN], 8, 45, 3, 3, 0, 0);
	RenderTextOnScreen(meshList[GEO_TEXT], "Health: " + std::to_string(health), Color(0, 1, 0), 2, 0, 2); //health
	RenderTextOnScreen(meshList[GEO_TEXT], std::to_string((int)carVelocity) + " kmph", Color(0, 1, 0), 2, 0, 1); //speed
	RenderTextOnScreen(meshList[GEO_TEXT], "Fuel left: " + std::to_string((int)fuel), Color(0, 1, 0), 2, 0, 0); //fuel
}

void DriveScene::Exit()
{
	// Cleanup here
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i] != NULL)
			delete meshList[i];
	}
	// Cleanup VBO here
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}


void DriveScene::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0) {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once in the whole function

	if (mesh->textureID > 0) glBindTexture(GL_TEXTURE_2D, 0);
}

void DriveScene::RenderSkybox()
{
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(-270.f, 50.f, 0.f);
	modelStack.Scale(553.f, 553.f, 553.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(270.f, 50.f, 0.f);
	modelStack.Scale(553.f, 553.f, 553.f);
	modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 150.f, 1.f);
	modelStack.Scale(553.f, 553.f, 553.f);
	modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
	modelStack.Rotate(0.f, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, -50.f, 0.f);
	modelStack.Scale(553.f, 553.f, 553.f);
	modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
	modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 50.f, -270.f);
	modelStack.Scale(553.f, 553.f, 553.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 50.f, 270.f);
	modelStack.Scale(553.f, 553.f, 553.f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
}

void DriveScene::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) {
		return;
	}
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i) {
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.5f, 0, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void DriveScene::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0) {
		return;
	}
	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	int moved = 0;
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i, ++moved) {
		if (text[i] == '~') {
			//newline += -1.f;
			modelStack.Translate(-moved * 0.7f, -2, 0);
			moved = 0;
			continue;
		}

		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.7f, 0, 0);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void DriveScene::RenderObj(Mesh* mesh, TRS& trs, bool end, bool enableLight)
{
	modelStack.PushMatrix();
	modelStack.Translate(trs.translate);
	modelStack.Rotate(trs.RotateX);
	modelStack.Rotate(trs.RotateY);
	modelStack.Rotate(trs.RotateZ);
	modelStack.Scale(trs.Scale);
	RenderMesh(mesh, enableLight);
	if (end)
		modelStack.PopMatrix();
}


void DriveScene::carMovement(TRS carbody, float& velocity, double dt)
{
	ACarBody.translate.z -= cos(Math::DegreeToRadian(ACarBody.RotateY.degree)) * (float)(carVelocity * dt);
	ACarBody.translate.x -= sin(Math::DegreeToRadian(ACarBody.RotateY.degree)) * (float)(carVelocity * dt);
	if (!(collision_detector(ACarBody,CCarBody,Askybox,Cskybox,true))) {
		ACarBody.translate.z += cos(Math::DegreeToRadian(ACarBody.RotateY.degree)) * (float)(carVelocity * dt);
		ACarBody.translate.x += sin(Math::DegreeToRadian(ACarBody.RotateY.degree)) * (float)(carVelocity * dt);
		carVelocity = 0;
	}
	for (CNode* current = boxlist.gethead(); current != nullptr; current = current->getnext())
	{
		if (collision_detector(ACarBody, CCarBody, current->transformation, CBox, true)) {
			if (!Powerup_onplayer[0]) {
				ACarBody.translate.z += cos(Math::DegreeToRadian(ACarBody.RotateY.degree)) * (float)(carVelocity * dt);
				ACarBody.translate.x += sin(Math::DegreeToRadian(ACarBody.RotateY.degree)) * (float)(carVelocity * dt);
				carVelocity = 0;
				if (GetTickCount() * 0.001f - iFrames > 3.0f) {
					if (Powerup_onplayer[1])
						--shield_health;
					else 
						--health;
					iFrames = GetTickCount() * 0.001f;
				}
			}
			else {
				collision_in_box_in_frame = 1;
			}
		}
		else {
			collision_in_box_in_frame = 0;
		}
	}
	for (CNode* current = spikelist.gethead(); current != nullptr; current = current->getnext())
	{
		if (collision_detector(ACarBody, CCarBody, current->transformation, CSpike, true)) {
			if (GetTickCount() * 0.001f - iFrames > 3.0f) {
				if (Powerup_onplayer[1] && shield_health > 0)
					--shield_health;
				else
					--health;
				iFrames = GetTickCount() * 0.001f;
			}
		}
		for (CNode* current = boostpadlist.gethead(); current != nullptr; current = current->getnext())
		{
			if (collision_detector(ACarBody, CCarBody, current->transformation, CBoostpad, true)) {
				carVelocity += 0.1;
			}

		}
		CNode* curr2 = coinlist.gethead();
		for (CNode* current = coinlist.gethead(); current != nullptr;)
		{
			if (collision_detector(ACarBody, CCarBody, current->transformation, CCoin)) {
				coinlist.removeItem(current);
				playerdetails.currency += 10;
				playerdetails.coinCounter++;

				//coin/currency increase code here
			}
			if (coinlist.gethead() == nullptr) {
				break;
			}
			current = current->getnext();
		}
	}
}

void DriveScene::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, float rotateX, float rotateY)
{
	if (!mesh || mesh->textureID <= 0) {
		return;
	}
	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 400, 0, 300, -10, 10); //size of screen
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //no need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(sizex, sizey, 1);
	modelStack.Rotate(rotateX, 1, 0, 0);
	modelStack.Rotate(rotateY, 0, 1, 0);
	RenderMesh(mesh, false);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);

}

void DriveScene::Generatecoinposition()
{
	if (count < 10)
	{
		while (count < 10)
		{
			randomcoin = rand() % 20;

			if (check[randomcoin] == 0)
			{
				check[randomcoin] = 1;
				coinarray[count++] = randomcoin;
			}
		}
	}
	else
	{
		randomcoin = rand() % 20;
	}
}

void DriveScene::CoinpositionSelector(bool generatearray)
{
	if (generatearray == true)
	{
		for (int i = 0; i < 10; i++)
		{
			if (coinarray[i] == 0)
			{
				coin.translate.Set(-108, 6, 178);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 1)
			{
				coin.translate.Set(-236, 6, 82);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 2)
			{
				coin.translate.Set(-236, 6, -238);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 3)
			{
				coin.translate.Set(-172, 6, -174);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 4)
			{
				coin.translate.Set(-172, 6, 50);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 5)
			{
				coin.translate.Set(-76, 6, -14);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 6)
			{
				coin.translate.Set(-76, 6, -110);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 7)
			{
				coin.translate.Set(-12, 6, -110);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 8)
			{
				coin.translate.Set(-12, 6, -14);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 9)
			{
				coin.translate.Set(52, 6, 50);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 10)
			{
				coin.translate.Set(-12, 6, 114);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 11)
			{
				coin.translate.Set(20, 6, 210);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 12)
			{
				coin.translate.Set(84, 6, 242);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 13)
			{
				coin.translate.Set(116, 6, 146);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 14)
			{
				coin.translate.Set(240, 6, 146);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 15)
			{
				coin.translate.Set(148, 6, -174);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 16)
			{
				coin.translate.Set(240, 6, -142);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 17)
			{
				coin.translate.Set(116, 6, -110);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 18)
			{
				coin.translate.Set(116, 6, -46);
				coinlist.addItem(coin);
			}
			if (coinarray[i] == 19)
			{
				coin.translate.Set(52, 6, -174);
				coinlist.addItem(coin);
			}
		}
	}
	else
	{
		if (randomcoin == 0)
		{
			coin.translate.Set(-108, 6, 178);
		}
		else if (randomcoin == 1)
		{
			coin.translate.Set(-236, 6, 82);
		}
		else if (randomcoin == 2)
		{
			coin.translate.Set(-236, 6, -238);
		}
		else if (randomcoin == 3)
		{
			coin.translate.Set(-172, 6, -174);
		}
		else if (randomcoin == 4)
		{
			coin.translate.Set(-172, 6, 50);
		}
		else if (randomcoin == 5)
		{
			coin.translate.Set(-76, 6, -14);
		}
		else if (randomcoin == 6)
		{
			coin.translate.Set(-76, 6, -110);
		}
		else if (randomcoin == 7)
		{
			coin.translate.Set(-12, 6, -110);
		}
		else if (randomcoin == 8)
		{
			coin.translate.Set(-12, 6, -14);
		}
		else if (randomcoin == 9)
		{
			coin.translate.Set(52, 6, 50);
		}
		else if (randomcoin == 10)
		{
			coin.translate.Set(-12, 6, 114);
		}
		else if (randomcoin == 11)
		{
			coin.translate.Set(20, 6, 210);
		}
		else if (randomcoin == 12)
		{
			coin.translate.Set(84, 6, 242);
		}
		else if (randomcoin == 13)
		{
			coin.translate.Set(116, 6, 146);
		}
		else if (randomcoin == 14)
		{
			coin.translate.Set(240, 6, 146);
		}
		else if (randomcoin == 15)
		{
			coin.translate.Set(148, 6, -174);
		}
		else if (randomcoin == 16)
		{
			coin.translate.Set(240, 6, -142);
		}
		else if (randomcoin == 17)
		{
			coin.translate.Set(116, 6, -110);
		}
		else if (randomcoin == 18)
		{
			coin.translate.Set(116, 6, -46);
		}
		else if (randomcoin == 19)
		{
			coin.translate.Set(52, 6, -174);
		}
	}
}

void DriveScene::Generatepowerposition()
{
	randompower = rand() % 5;
}

void DriveScene::CoinRespawn()
{
	if (timetorespawncoin < GetTickCount())
	{
		for (CNode* current = coinlist.gethead(); current != nullptr; current = current->getnext())
		{
			if (coinlist.gethead() != nullptr)
			{
				coinchecker++;
			}
		}
		if (coinchecker != 10)
		{
			Generatecoinposition();
			CoinpositionSelector(coingeneratearray);

			for (CNode* current = coinlist.gethead(); current != nullptr; current = current->getnext())
			{
				if (coinlist.gethead() != nullptr)
				{
					if (coin.translate != coinlist.gethead()->transformation.translate)
					{
						coinlist.addItem(coin);
						showcointexttime = GetTickCount() + 3000;
						timetorespawncoin = GetTickCount() + 10000;
						break;
					}
				}
			}
		}
	}
	coinchecker = 0;
}