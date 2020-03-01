#include "SceneSkybox.h"
#include "GL\glew.h"
#include "Application.h"
#include <Mtx44.h>
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <string>
#include "SceneManager.h"

#define ROT_LIMIT 45.f;
#define SCALE_LIMIT 5.f;
#define LSPEED 10.f

SceneSkybox::SceneSkybox()
{
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
}

SceneSkybox::~SceneSkybox()
{
	for (int i = GEO_AXES; i < NUM_GEOMETRY; ++i) {
		delete meshList[i];
	}
}

void SceneSkybox::Init()
{
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
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

	for (int i = 0; i < 4; i++)
	{
		std::string number = std::to_string(i);

		m_parameters[8 + (i * 11)] = glGetUniformLocation(m_programID, ("lights[" + number + "].position_cameraspace").c_str());
		m_parameters[9 + (i * 11)] = glGetUniformLocation(m_programID, ("lights[" + number + "].color").c_str());
		m_parameters[10 + (i * 11)] = glGetUniformLocation(m_programID, ("lights[" + number + "].power").c_str());
		m_parameters[11 + (i * 11)] = glGetUniformLocation(m_programID, ("lights[" + number + "].kC").c_str());
		m_parameters[12 + (i * 11)] = glGetUniformLocation(m_programID, ("lights[" + number + "].kL").c_str());
		m_parameters[13 + (i * 11)] = glGetUniformLocation(m_programID, ("lights[" + number + "].kQ").c_str());
		m_parameters[14 + (i * 11)] = glGetUniformLocation(m_programID, ("lights[" + number + "].type").c_str());
		m_parameters[15 + (i * 11)] = glGetUniformLocation(m_programID, ("lights[" + number + "].spotDirection").c_str());
		m_parameters[16 + (i * 11)] = glGetUniformLocation(m_programID, ("lights[" + number + "].cosCutoff").c_str());
		m_parameters[17 + (i * 11)] = glGetUniformLocation(m_programID, ("lights[" + number + "].cosInner").c_str());
		m_parameters[18 + (i * 11)] = glGetUniformLocation(m_programID, ("lights[" + number + "].exponent").c_str());
	}
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	//Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");

	glUseProgram(m_programID);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < 4; i++)
	{
		light[i].type = Light::LIGHT_SPOT;
		if (i == 0)
		{
			light[i].position.Set(-20, 20, 0);
		}
		else if (i == 1)
		{
			light[i].position.Set(20, 20, 0);
		}
		else if (i == 2)
		{
			light[i].position.Set(-20, 20, 30);
		}
		else if (i == 3)
		{
			light[i].position.Set(20, 20, 30);
		}
		light[i].color.Set(1.f, 0.f, 0.f);
		light[i].power = 0.5f;
		light[i].kC = 1.f;
		light[i].kL = 0.01f;
		light[i].kQ = 0.001f;
		light[i].cosCutoff = cos(Math::DegreeToRadian(45));
		light[i].cosInner = cos(Math::DegreeToRadian(30));
		light[i].exponent = 10.f;
		light[i].spotDirection.Set(0.f, 1.f, 0.f);

		glUniform3fv(m_parameters[9 + (i * 11)], 1, &light[i].color.r);
		glUniform1f(m_parameters[10 + (i * 11)], light[i].power);
		glUniform1f(m_parameters[11 + (i * 11)], light[i].kC);
		glUniform1f(m_parameters[12 + (i * 11)], light[i].kL);
		glUniform1f(m_parameters[13 + (i * 11)], light[i].kQ);
		glUniform1i(m_parameters[14 + (i * 11)], light[i].type);
		glUniform3fv(m_parameters[15 + (i * 11)], 1, &light[i].spotDirection.x);
		glUniform1f(m_parameters[16 + (i * 11)], light[i].cosCutoff);
		glUniform1f(m_parameters[17 + (i * 11)], light[i].cosInner);
		glUniform1f(m_parameters[18 + (i * 11)], light[i].exponent);
	}
	glUniform1i(m_parameters[U_NUMLIGHTS], 5);

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

	meshList[GEO_LIGHTSPHERE] = MeshBuilder::GenerateSphere("lightBall", Color(1.f, 1.f, 0.f), 9, 36, 1.f);

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");

	meshList[GEO_NPC] = MeshBuilder::GenerateOBJ("npc", "OBJ//npc.obj");
	meshList[GEO_NPC]->textureID = LoadTGA("Image//npc.tga");
	ANPC.translate = Vector3(20, 3, 20);
	NPCSpeech[0] = "Guang Theng's car,~a unique design by Guang Theng himself,~has high speed and fuel";
	NPCSpeech[1] = "Ryan's car,~a unique design by Ryan himself,~has high speed and turbo";
	NPCSpeech[2] = "Jun Chen's car,~a unique design by Jun Chen himself,~has average stats across the board";
	NPCSpeech[3] = "Jian Feng's car,~a unique design by Jian Feng himself,~has high fuel and turbo";
	textLasttime = 0;
	carnum = 5;
	textnum = 0;
	NPCtext = "";

	meshList[GEO_PLAYERBODY] = MeshBuilder::GenerateOBJ("playerbody", "OBJ//playerbody.obj");
	meshList[GEO_PLAYERBODY]->textureID = LoadTGA("Image//player.tga");
	Aplayer.translate = Vector3(0, 5.5, 5);
	Aplayer.RotateY.degree = 180;
	Loadcoord("OBJ//playerbody.obj", Cplayer);

	meshList[GEO_PLAYERLEFTARM] = MeshBuilder::GenerateOBJ("playerleftarm", "OBJ//playerleftarm.obj");
	meshList[GEO_PLAYERLEFTARM]->textureID = LoadTGA("Image//player.tga");
	Aplayerleftarm.translate = Vector3(1, -0.3, -0.2);

	meshList[GEO_PLAYERRIGHTARM] = MeshBuilder::GenerateOBJ("playerrightarm", "OBJ//playerrightarm.obj");
	meshList[GEO_PLAYERRIGHTARM]->textureID = LoadTGA("Image//player.tga");
	Aplayerrightarm.translate = Vector3(-1, -0.3, -0.2);

	meshList[GEO_PLAYERLEFTLEG] = MeshBuilder::GenerateOBJ("playerleftleg", "OBJ//playerleftleg.obj");
	meshList[GEO_PLAYERLEFTLEG]->textureID = LoadTGA("Image//player.tga");
	Aplayerleftleg.translate = Vector3(0.3, -3.2, 0.5);

	meshList[GEO_PLAYERRIGHTLEG] = MeshBuilder::GenerateOBJ("playerrightleg", "OBJ//playerrightleg.obj");
	meshList[GEO_PLAYERRIGHTLEG]->textureID = LoadTGA("Image//player.tga");
	Aplayerrightleg.translate = Vector3(-0.3, -3.2, 0.5);

	rotatebodyparts = false;
	movebodyupdown = false;

	meshList[GEO_SHOP] = MeshBuilder::GenerateOBJ("shop", "OBJ//shop.obj");
	meshList[GEO_SHOP]->textureID = LoadTGA("Image//shop.tga");
	Shop.translate = Vector3(0, 0, -45);
	Shop.Scale = Vector3(7.5f, 7.5f, 7.5f);
	Shop.RotateY = Vector4(90, 0, 1, 0);
	Loadcoord("OBJ//shop.obj", CShop);

	ShopUI = HologramUI(10.f, 10.f);
	meshList[GEO_HOLO_SHOP] = MeshBuilder::GenerateQuad("shopUI", Color(0, 0, 1), ShopUI.lengthX, ShopUI.lengthY);
	meshList[GEO_HOLO_SHOP]->textureID = LoadTGA("image//shopUI.tga");
	ShopUI.UI.translate = Vector3(0, 0, -45);
	ShopUI_Scroll = 0;

	meshList[GEO_CAR_STAT] = MeshBuilder::GenerateQuad("car_stat", Color(1, 0, 0), 1.f, 1.f);
	meshList[GEO_CAR_STAT_UPGRADE] = MeshBuilder::GenerateQuad("car_stat_upgrade", Color(0, 1, 0), 1.f, 1.f);

	BounceTime = 0;

	meshList[GEO_PLATFORM] = MeshBuilder::GenerateOBJ("Platform", "OBJ//Platform.obj");
	meshList[GEO_PLATFORM]->textureID = LoadTGA("Image//Platform.tga");
	meshList[GEO_PLATFORM]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_PLATFORM]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_PLATFORM]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_PLATFORM]->material.kShininess = 1.f;
	Platform[0].translate = Vector3(-20, 0, 0);
	Platform[1].translate = Vector3(20, 0, 0);
	Platform[2].translate = Vector3(-20, 0, 30);
	Platform[3].translate = Vector3(20, 0, 30);
	Platform[0].Scale = Vector3(1.2, 1.2, 1.2);
	Platform[1].Scale = Vector3(1.2, 1.2, 1.2);
	Platform[2].Scale = Vector3(1.2, 1.2, 1.2);
	Platform[3].Scale = Vector3(1.2, 1.2, 1.2);

	lightball[0].translate = Vector3(light[0].position.x, light[0].position.y, light[0].position.z);
	lightball[1].translate = Vector3(light[1].position.x, light[1].position.y, light[1].position.z);
	lightball[2].translate = Vector3(light[2].position.x, light[2].position.y, light[2].position.z);
	lightball[3].translate = Vector3(light[3].position.x, light[3].position.y, light[3].position.z);

	Loadcoord("OBJ//Platform.obj", PlatformR);

	// Car 1
	meshList[GEO_CAR1BODY] = MeshBuilder::GenerateOBJ("Car1", "OBJ//GuangThengCarBody.obj");
	meshList[GEO_CAR1BODY]->textureID = LoadTGA("Image//GuangThengCarTex.tga");
	meshList[GEO_CAR1BODY]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_CAR1BODY]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR1BODY]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR1BODY]->material.kShininess = 1.f;
	Cars[0].translate = Vector3(0, 2.05, 0);
	Cars[0].Scale = Vector3(1.55, 1.55, 1.55);
	Loadcoord("OBJ//GuangThengCarBody.obj", CCars[0]);

	meshList[GEO_CAR1WHEEL] = MeshBuilder::GenerateOBJ("Car1Wheel", "OBJ//GuangThengCarWheel.obj");
	meshList[GEO_CAR1WHEEL]->textureID = LoadTGA("Image//GuangThengCarTex.tga");
	meshList[GEO_CAR1WHEEL]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_CAR1WHEEL]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR1WHEEL]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR1WHEEL]->material.kShininess = 1.f;
	CarWheel[0][0].translate = Vector3(2, -0.4, -2.8);
	CarWheel[0][1].translate = Vector3(-2.1, -0.4, -2.8);
	CarWheel[0][2].translate = Vector3(2.05, -0.4, 2.8);
	CarWheel[0][2].Scale = Vector3(1.18, 1.18, 1.18);
	CarWheel[0][3].translate = Vector3(-2.15, -0.4, 2.8);
	CarWheel[0][3].Scale = Vector3(1.18, 1.18, 1.18);

	// Car 2
	meshList[GEO_CAR2BODY] = MeshBuilder::GenerateOBJ("Car2", "OBJ//RyanCarBody.obj");
	meshList[GEO_CAR2BODY]->textureID = LoadTGA("Image//RyanCarBodyTex.tga");
	meshList[GEO_CAR2BODY]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_CAR2BODY]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR2BODY]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR2BODY]->material.kShininess = 1.f;
	Cars[1].translate = Vector3(0, 3.2, 0);
	Cars[1].Scale = Vector3(2, 2, 2);
	Loadcoord("OBJ//RyanCarBody.obj", CCars[1]);

	meshList[GEO_CAR2WHEEL] = MeshBuilder::GenerateOBJ("Car2Wheel", "OBJ//RyanCarWheel.obj");
	meshList[GEO_CAR2WHEEL]->textureID = LoadTGA("Image//RyanCarWheelTex.tga");
	meshList[GEO_CAR2WHEEL]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_CAR2WHEEL]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR2WHEEL]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR2WHEEL]->material.kShininess = 1.f;
	CarWheel[1][0].translate = Vector3(0.7, -0.95, -1.2);
	CarWheel[1][1].translate = Vector3(-1.3, -0.95, -1.2);
	CarWheel[1][2].translate = Vector3(0.75, -0.95, 1.7);
	CarWheel[1][3].translate = Vector3(-1.3, -0.95, 1.7);

	// Car 3
	meshList[GEO_CAR3BODY] = MeshBuilder::GenerateOBJ("Car3", "OBJ//JCCarBody.obj");
	meshList[GEO_CAR3BODY]->textureID = LoadTGA("Image//JCCarTex.tga");
	meshList[GEO_CAR3BODY]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_CAR3BODY]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR3BODY]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR3BODY]->material.kShininess = 1.f;
	Cars[2].translate = Vector3(0, 3, 0);
	Cars[2].Scale = Vector3(2, 2, 2);
	Loadcoord("OBJ//JCCarBody.obj", CCars[2]);

	meshList[GEO_CAR3WHEEL] = MeshBuilder::GenerateOBJ("Car3Wheel", "OBJ//JCCarWheel.obj");
	meshList[GEO_CAR3WHEEL]->textureID = LoadTGA("Image//JCCarTex.tga");
	meshList[GEO_CAR3WHEEL]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_CAR3WHEEL]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR3WHEEL]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR3WHEEL]->material.kShininess = 1.f;
	CarWheel[2][0].translate = Vector3(1.8, -0.86, -2.1);
	CarWheel[2][1].translate = Vector3(-1.8, -0.86, -2.1);
	CarWheel[2][2].translate = Vector3(1.8, -0.86, 2.1);
	CarWheel[2][3].translate = Vector3(-1.8, -0.86, 2.1);

	// Car 4
	meshList[GEO_CAR4BODY] = MeshBuilder::GenerateOBJ("Car4", "OBJ//JianFengCarBody.obj");
	meshList[GEO_CAR4BODY]->textureID = LoadTGA("Image//JianFengCarTex.tga");
	meshList[GEO_CAR4BODY]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_CAR4BODY]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR4BODY]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR4BODY]->material.kShininess = 1.f;
	Cars[3].translate = Vector3(0, 4.7, 0);
	Cars[3].Scale = Vector3(1.7, 1.7, 1.7);
	Loadcoord("OBJ//JianFengCarBody.obj", CCars[3]);

	meshList[GEO_CAR4WHEEL] = MeshBuilder::GenerateOBJ("Car4Wheel", "OBJ//JianFengCarWheel.obj");
	meshList[GEO_CAR4WHEEL]->textureID = LoadTGA("Image//JianFengCarTex.tga");
	meshList[GEO_CAR4WHEEL]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_CAR4WHEEL]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR4WHEEL]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_CAR4WHEEL]->material.kShininess = 1.f;
	CarWheel[3][0].translate = Vector3(1.45, -1.5, -2.5);
	CarWheel[3][1].translate = Vector3(-1.45, -1.5, -2.5);
	CarWheel[3][2].translate = Vector3(1.55, -1.35, 2.5);
	CarWheel[3][2].Scale = Vector3(1.2, 1.2, 1.2);
	CarWheel[3][3].translate = Vector3(-1.77, -1.35, 2.5);
	CarWheel[3][3].Scale = Vector3(1.2, 1.2, 1.2);

	// Door
	meshList[GEO_DOOR] = MeshBuilder::GenerateOBJ("door", "OBJ//door.obj");
	meshList[GEO_DOOR]->textureID = LoadTGA("Image//door.tga");
	meshList[GEO_DOOR]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_DOOR]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_DOOR]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_DOOR]->material.kShininess = 1.f;
	Loadcoord("OBJ//door.obj", CDoor);
	Door.translate = Vector3(45, 4, 35);
	Door.RotateY.degree = 270;
	Door.Scale = Vector3(1.5, 1.5, 1.5);


	meshList[GEO_DOORSCREEN] = MeshBuilder::GenerateQuad("doorscreen", Color(0, 0, 0), 8, 9);
	meshList[GEO_DOORSCREEN]->textureID = LoadTGA("Image//doorscreen.tga");
	meshList[GEO_DOORSCREEN]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_DOORSCREEN]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_DOORSCREEN]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_DOORSCREEN]->material.kShininess = 1.f;
	DoorScreen.translate = Vector3(0, -0.2, 2);
	DoorScreen.Scale = Vector3(1, 1, 1);
	DoorCheck = Door + DoorScreen;

	meshList[GEO_FAKEDOORSCREEN] = MeshBuilder::GenerateOBJ("doorscreen", "OBJ//doorscreen.obj");
	meshList[GEO_FAKEDOORSCREEN]->textureID = LoadTGA("Image//doorscreen.tga");
	meshList[GEO_FAKEDOORSCREEN]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_FAKEDOORSCREEN]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_FAKEDOORSCREEN]->material.kSpecular.Set(1.f, 1.f, 1.f);
	meshList[GEO_FAKEDOORSCREEN]->material.kShininess = 1.f;
	FakeScreen.translate = Vector3(40, 4, 35);
	FakeScreen.RotateY.degree = 90;
	FakeScreen.Scale = Vector3(2, 2, 2);
	Loadcoord("OBJ//doorscreen.obj", CdoorScreen);


	meshList[GEO_SLOT_BODY] = MeshBuilder::GenerateOBJ("slot body", "obj//slots_body.obj");
	meshList[GEO_SLOT_BODY]->textureID = LoadTGA("Image//slots.tga");
	meshList[GEO_SLOT_BODY]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SLOT_BODY]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_SLOT_BODY]->material.kSpecular.Set(1.f, 1.f, 1.f);
	Aslot_body.translate = Vector3(40, 5, -40);
	Loadcoord("obj//slots_body.obj", Cslot_body);

	meshList[GEO_SLOT_ARM] = MeshBuilder::GenerateOBJ("slot arm", "obj//slots_handle.obj");
	meshList[GEO_SLOT_ARM]->textureID = LoadTGA("Image//slots.tga");
	meshList[GEO_SLOT_ARM]->material.kAmbient.Set(0.7f, 0.7f, 0.7f);
	meshList[GEO_SLOT_ARM]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_SLOT_ARM]->material.kSpecular.Set(1.f, 1.f, 1.f);
	Aslot_arm.translate = Vector3(0, 0, 0);

	meshList[GEO_SLOT_SCREEN_IMAGE] = MeshBuilder::GenerateText("text", 3, 5);
	meshList[GEO_SLOT_SCREEN_IMAGE]->textureID = LoadTGA("Image//slots_screen.tga");
	Aslot_image[0].translate = Vector3(-2.5, 2.3, 0);
	Aslot_image[1].translate = Vector3(-0.5, 2.3, 0);
	Aslot_image[2].translate = Vector3(1.5, 2.3, 0);
	Aslot_image[3].translate = Vector3(-2.5, 0.8, 0);
	Aslot_image[4].translate = Vector3(-0.5, 0.8, 0);
	Aslot_image[5].translate = Vector3(1.5, 0.8, 0);
	Aslot_image[6].translate = Vector3(-2.5, -0.7, 0);
	Aslot_image[7].translate = Vector3(-0.5, -0.7, 0);
	Aslot_image[8].translate = Vector3(1.5, -0.7, 0);
	for (int i = 0; i < 9; ++i) {
		Aslot_image[i].Scale = Vector3(1.5, 1.5, 1.5);
	}
	slot_images[0] = slot_images[1] = slot_images[2] = 4;
	slot_images[3] = slot_images[4] = slot_images[5] = 0;
	slot_images[6] = slot_images[7] = slot_images[8] = 1;
	int tempfill[15] = { 10,11,12,13,14,5,6,7,8,9,0,1,2,3,4 };
	for (int i = 0; i < 5; ++i) {
		slot_rotation_lower[i] = tempfill[i];
	}
	for (int i = 0; i < 5; ++i) {
		slot_rotation_whole[i] = tempfill[i + 5];
	}
	for (int i = 0; i < 5; ++i) {
		slot_rotation_upper[i] = tempfill[i + 10];
	}
	activate_slot_machine = 0;
	row1_lastTime = 0;
	row2_lastTime = 0;
	row3_lastTime = 0;
	slot_stop_lasttime = 0;
	stop_machine = 0;	

	meshList[GEO_COIN] = MeshBuilder::GenerateOBJ("coin", "obj//coin.obj");
	meshList[GEO_COIN]->textureID = LoadTGA("Image//coin.tga");


	meshList[GEO_RAND_BOX] = MeshBuilder::GenerateOBJ("box", "OBJ//box.obj");
	meshList[GEO_RAND_BOX]->textureID = LoadTGA("image//box.tga");
	Loadcoord("obj//box.obj", Crandbox);
	Arandbox.translate = Vector3(0, 0, -20);
	Arandbox.Scale = Vector3(2, 2, 2);
	Arandbox.RotateY.degree = 45;
	godmode = 0;
	godmodelasttime = 0;

	meshList[GEO_PETROLSTATION] = MeshBuilder::GenerateOBJ("petrol station", "OBJ//petrolstation.obj");
	meshList[GEO_PETROLSTATION]->textureID = LoadTGA("image//petrolstation.tga");
	PetrolStation.translate = Vector3(-20.f, 0.f, -45.f);
	Loadcoord("OBJ//petrolstation.obj", CPetrolstation);

	PetrolStationUI = HologramUI(3.f, 3.f);
	PetrolStationUI.UI.translate = Vector3(-20.f, 0.f, -43.f);
	meshList[GEO_HOLO_PETROLSTATION] = MeshBuilder::GenerateQuad("petrol station UI", Color(0.f, 0.f, 1.f), PetrolStationUI.lengthX, PetrolStationUI.lengthY);

	InitPetrolStationCar();

	for (int i = 0; i < 4; ++i)
	{
		CarHologram[i] = HologramUI(10.f, 7.f);
		if (i % 2 != 0)
		{
			CarHologram[i].UI.RotateY.degree = -90.f;
			CarHologram[i].UI.translate = Vector3(-10, 0, 0) + Platform[i].translate;
		}
		else
		{
			CarHologram[i].UI.RotateY.degree = 90.f;
			CarHologram[i].UI.translate = Vector3(10, 0, 0) + Platform[i].translate;
		}
	}

	meshList[GEO_HOLO_PLATFORM] = MeshBuilder::GenerateQuad("holo0", Color(0, 0, 1.f), 10.f, 7.f);

	meshList[GEO_HOLO_PLATFORM]->textureID = LoadTGA("Image//carstatsUI.tga");

	if (playerdetails.IsInit()) EquippedCar_Scroll = playerdetails.car_number.cartype;
	else EquippedCar_Scroll = 2;

	playerdetails = PlayerDetails(CarSelection(playerdetails.allcardetails.getCarStats(EquippedCar_Scroll), 2), 500); 

	playerdetails.allcardetails = AllCarDetails();
	playerdetails.allcardetails.InitScene(car_Stats[0], car_Stats[1], car_Stats[2], car_Stats[3]);

	hologramcamera_leave = true;
	camera.Init(Aplayer.translate + Vector3(0, 8, 15), Vector3(Aplayer.translate) + Vector3(0, 5, 0), Vector3(0, 1, 0));
	firstpersoncamera.Init(Vector3(Aplayer.translate.x, Aplayer.translate.y + 2, Aplayer.translate.z + 5), Vector3(0, Aplayer.translate.y + 2, 0), Vector3(0, 1, 0));
	hologramcamera.Init(Aplayer.translate + Vector3(0, 5, 0), Vector3(ShopUI.UI.translate) + Vector3(0, 5, 0), Vector3(0, 1, 0));
	CameraSwitch = 0;

	if (playsound == true)
	{
		PlaySound(TEXT("Music/Jubilife.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	}
}

void SceneSkybox::Update(double dt)
{

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

	//UI Text (for movement) logic
	if (Application::IsKeyPressed('W') && CameraSwitch != 2) {
		PlayerMoveUp(dt);

		//player animation
		if (Aplayerleftarm.RotateX.degree > -30 && rotatebodyparts == false)
		{
			Aplayerleftarm.RotateX.degree--;
			if (Aplayerleftarm.RotateX.degree <= -30)
			{
				Aplayerleftarm.RotateX.degree = -30;
				rotatebodyparts = true;
			}
		}
		else if (Aplayerleftarm.RotateX.degree < 30 && rotatebodyparts == true)
		{
			Aplayerleftarm.RotateX.degree++;
			if (Aplayerleftarm.RotateX.degree >= 30)
			{
				Aplayerleftarm.RotateX.degree = 30;
				rotatebodyparts = false;
			}
		}
		if (Aplayerrightarm.RotateX.degree < 30 && rotatebodyparts == false)
		{
			Aplayerrightarm.RotateX.degree++;
			if (Aplayerrightarm.RotateX.degree >= 30)
			{
				Aplayerrightarm.RotateX.degree = 30;
			}
		}
		else if (Aplayerrightarm.RotateX.degree > -30 && rotatebodyparts == true)
		{
			Aplayerrightarm.RotateX.degree--;
			if (Aplayerrightarm.RotateX.degree <= -30)
			{
				Aplayerrightarm.RotateX.degree = -30;
			}
		}
		if (Aplayer.translate.y < 6 && movebodyupdown == false)
		{
			Aplayer.translate.y += 0.05;
			if (Aplayer.translate.y >= 6)
			{
				Aplayer.translate.y = 6;
				movebodyupdown = true;
			}
		}
		else if (Aplayer.translate.y > 5 && movebodyupdown == true)
		{
			Aplayer.translate.y -= 0.05;
			if (Aplayer.translate.y <= 5)
			{
				Aplayer.translate.y = 5;
				movebodyupdown = false;
			}
		}
	}
	if (Application::IsKeyPressed('A') && CameraSwitch != 2) {
		PlayerMoveLeft(dt);
	}
	if (Application::IsKeyPressed('S') && CameraSwitch != 2) {
		PlayerMoveDown(dt);
		//player animation
		if (Aplayerleftarm.RotateX.degree > -30 && rotatebodyparts == false)
		{
			Aplayerleftarm.RotateX.degree--;
			if (Aplayerleftarm.RotateX.degree <= -30)
			{
				Aplayerleftarm.RotateX.degree = -30;
				rotatebodyparts = true;
			}
		}
		else if (Aplayerleftarm.RotateX.degree < 30 && rotatebodyparts == true)
		{
			Aplayerleftarm.RotateX.degree++;
			if (Aplayerleftarm.RotateX.degree >= 30)
			{
				Aplayerleftarm.RotateX.degree = 30;
				rotatebodyparts = false;
			}
		}
		if (Aplayerrightarm.RotateX.degree < 30 && rotatebodyparts == false)
		{
			Aplayerrightarm.RotateX.degree++;
			if (Aplayerrightarm.RotateX.degree >= 30)
			{
				Aplayerrightarm.RotateX.degree = 30;
			}
		}
		else if (Aplayerrightarm.RotateX.degree > -30 && rotatebodyparts == true)
		{
			Aplayerrightarm.RotateX.degree--;
			if (Aplayerrightarm.RotateX.degree <= -30)
			{
				Aplayerrightarm.RotateX.degree = -30;
			}
		}
		if (Aplayer.translate.y < 6 && movebodyupdown == false)
		{
			Aplayer.translate.y += 0.05;
			if (Aplayer.translate.y >= 6)
			{
				Aplayer.translate.y = 6;
				movebodyupdown = true;
			}
		}
		else if (Aplayer.translate.y > 5 && movebodyupdown == true)
		{
			Aplayer.translate.y -= 0.05;
			if (Aplayer.translate.y <= 5)
			{
				Aplayer.translate.y = 5;
				movebodyupdown = false;
			}
		}
	}

	if (Application::IsKeyPressed('D') && CameraSwitch != 2) {
		PlayerMoveRight(dt);
	}
	if (Application::IsKeyPressed(VK_LEFT) && CameraSwitch != 2) {
		Aplayer.RotateY.degree += (float)(playerTurningSpeed * dt);
	}
	if (Application::IsKeyPressed(VK_RIGHT) && CameraSwitch != 2) {
		Aplayer.RotateY.degree -= (float)(playerTurningSpeed * dt);
	}
	// If player is not moving, check whether has the arms rotate back to the original position (if not, rotate the arms back) // Check if body is back at original height (if not, translate back)
	if (!Application::IsKeyPressed('W') && !Application::IsKeyPressed('S'))
	{
		if (Aplayerleftarm.RotateX.degree < 0)
		{
			Aplayerleftarm.RotateX.degree++;
			if (Aplayerleftarm.RotateX.degree > 0)
			{
				Aplayerleftarm.RotateX.degree = 0;
			}
		}
		else if (Aplayerleftarm.RotateX.degree > 0)
		{
			Aplayerleftarm.RotateX.degree--;
			if (Aplayerleftarm.RotateX.degree < 0)
			{
				Aplayerleftarm.RotateX.degree = 0;
			}
		}
		if (Aplayerrightarm.RotateX.degree < 0)
		{
			Aplayerrightarm.RotateX.degree++;
			if (Aplayerrightarm.RotateX.degree > 0)
			{
				Aplayerrightarm.RotateX.degree = 0;
			}
		}
		else if (Aplayerrightarm.RotateX.degree > 0)
		{
			Aplayerrightarm.RotateX.degree--;
			if (Aplayerrightarm.RotateX.degree < 0)
			{
				Aplayerrightarm.RotateX.degree = 0;
			}
		}
		if (Aplayer.translate.y > 5.5)
		{
			Aplayer.translate.y -= 0.5;
			if (Aplayer.translate.y <= 5.5)
			{
				Aplayer.translate.y = 5.5;
			}
		}
		else if (Aplayer.translate.y < 5.5)
		{
			Aplayer.translate.y += 0.5;
			if (Aplayer.translate.y >= 5.5)
			{
				Aplayer.translate.y = 5.5;
			}
		}
	}
	//NPC
	if ((ANPC.translate - Aplayer.translate).Length() > 8) {
		for (int i = 0; i < 4; ++i) {
			if (collision_detection::collision_detector(Aplayer, Cplayer, Platform[i], PlatformR + 2)) {
				ANPC.translate.x += ((Aplayer.translate) - ANPC.translate).Normalize().x * (float)(dt * playerMovementSpeed);
				ANPC.translate.z += ((Aplayer.translate) - ANPC.translate).Normalize().z * (float)(dt * playerMovementSpeed);
				if (carnum != i) {
					NPCtext = "";
					carnum = i;
					textnum = 0;
				}
			}
			else {
				NPCtext = "";
				textnum = 0;
			}
		}
	}
	else if ((ANPC.translate - Aplayer.translate).Length() < 8) {
		if (GetTickCount() * 0.001f - textLasttime > 0.08f && textnum < NPCSpeech[carnum].length() && carnum != 5) {
			NPCtext.insert(NPCtext.end(), NPCSpeech[carnum][textnum++]);
			if (NPCSpeech[carnum][textnum] == ' ') {
				textLasttime = GetTickCount() * 0.001f + 0.15f;
			}
			else {
				textLasttime = GetTickCount() * 0.001f;
			}
		}
	}
	ANPC.RotateY.degree = Math::RadianToDegree(acos((Aplayer.translate - ANPC.translate).Normalize().Dot(Vector3(0, 0, 1))));
	if ((Aplayer.translate - ANPC.translate).Normalize().x < 0) {
		ANPC.RotateY.degree *= -1;
	}

	//UI Text (for 1st/3rd person camera) logic
	if (Application::IsKeyPressed(VK_TAB) && BounceTime <= GetTickCount()) {
		if (CameraSwitch == 0) CameraSwitch = 1; //1st person
		else if (CameraSwitch != 2)
		{
			CameraSwitch = 0; //3rd person
		}
		BounceTime = GetTickCount() + 500.f;
	}

	if (CameraSwitch == 2 && hologramcamera_leave)
	{
		CameraSwitch = 0;
	}

	if (Application::IsKeyPressed('E') && BounceTime <= GetTickCount())
	{
		UpdateEquippedCar();
		BounceTime = GetTickCount() + 500.f;
	}

	if (Application::IsKeyPressed('G') && GetTickCount() * 0.001f - godmodelasttime > 1.0f) {
		godmodelasttime = GetTickCount() * 0.001f;
		if (godmode)
			godmode = 0;
		else
			godmode = 1;
	}
	if (godmode && Application::IsKeyPressed('N')) {
		Arandbox.RotateY.degree += (float)(playerTurningSpeed * dt);
	}
	if (godmode && Application::IsKeyPressed('M')) {
		Arandbox.RotateY.degree -= (float)(playerTurningSpeed * dt);
	}

	// Rotate platform
	if (Application::IsKeyPressed('N'))
	{
		for (int i = 0; i < 4; i++)
		{
			if ((Aplayer.translate - Platform[i].translate).Length() < 15)
			{
				Platform[i].RotateY.degree--;
			}
		}
	}
	if (Application::IsKeyPressed('M'))
	{
		for (int i = 0; i < 4; i++)
		{
			if ((Aplayer.translate - Platform[i].translate).Length() < 15)
			{
				Platform[i].RotateY.degree++;
			}
		}
	}

	// Open & Close Door
	if ((Aplayer.translate - Door.translate).Length() < 14)
	{
		if (DoorScreen.translate.x <= 8)
		{
			DoorScreen.translate.x += 0.25f;
		}
	}
	else
	{
		if (DoorScreen.translate.x > 0)
		{
			DoorScreen.translate.x -= 0.25f;
		}
	}
	//slot machine
	if ((GetTickCount() * 0.001f - slot_stop_lasttime) > 0.5f &&
		Application::IsKeyPressed(VK_SPACE) && (Aslot_body.translate - Aplayer.translate).Length() < 10) {
		if (activate_slot_machine) {
			++stop_machine;
			slot_stop_lasttime = GetTickCount() * 0.001f;
		}
		else {
			activate_slot_machine = 1;
			slot_stop_lasttime = GetTickCount() * 0.001f;
			stop_machine = 0;
			playerdetails.currency -= 10;
		}
	}
	if (activate_slot_machine == 1) {
		if (GetTickCount() * 0.001f - row1_lastTime > (0.2f) && stop_machine <= 0) {
			++slot_images[0];
			++slot_images[3];
			++slot_images[6];
			row1_lastTime = GetTickCount() * 0.001f;
		}
		if (GetTickCount() * 0.001f - row2_lastTime > (0.1f) && stop_machine <= 1) {
			++slot_images[1];
			++slot_images[4];
			++slot_images[7];
			row2_lastTime = GetTickCount() * 0.001f;
		}
		if (GetTickCount() * 0.001f - row3_lastTime > (0.05f) && stop_machine <= 2) {
			++slot_images[2];
			++slot_images[5];
			++slot_images[8];
			row3_lastTime = GetTickCount() * 0.001f;
		}
		if (stop_machine > 2) {
			if (slot_images[3] % 5 == slot_images[4] % 5 && slot_images[3] % 5 == slot_images[5] % 5) {
				int num = slot_images[3] % 5;
				switch (num) {
				case 0:
					playerdetails.currency += 1000;
				case 1:
					playerdetails.currency += 200;
				case 2:
					playerdetails.currency += 300;
				case 3:
					playerdetails.currency += 400;
				case 4:
					playerdetails.currency += 500;
				}
			}
			activate_slot_machine = 0;
		}
	}
	//Holograms and Camera logic
	UpdateHologram(ShopUI, car_Stats[ShopUI_Scroll], &Shop, 10.f);
	if (DistanceCheck(Aplayer.translate, Shop.translate) && !hologramcamera_leave)
	{
		CameraSwitch = 2;
		hologramcamera.Update(dt, ShopUI, Aplayer, 15.f);
		if (Application::IsKeyPressed(VK_LEFT) && BounceTime <= GetTickCount())
		{
			if (ShopUI_Scroll != 0.f) --ShopUI_Scroll;
			else ShopUI_Scroll = 3.f;
			BounceTime = GetTickCount() + 500.f;
		}
		else if (Application::IsKeyPressed(VK_RIGHT) && BounceTime <= GetTickCount())
		{
			if (ShopUI_Scroll != 3.f) ++ShopUI_Scroll;
			else ShopUI_Scroll = 0.f;
			BounceTime = GetTickCount() + 500.f;
		}
	}
	else if (DistanceCheck(Aplayer.translate, PetrolStation.translate) && !hologramcamera_leave)
	{
		CameraSwitch = 2;
		hologramcamera.Update(dt, PetrolStationUI, Aplayer, 5.f);
	}

	UpdateHologram(PetrolStationUI, car_Stats[EquippedCar_Scroll], &PetrolStation, 5.f);
	for (int i = 0; i < 4; ++i)
	{
		if (DistanceCheck(Aplayer.translate, Platform[i].translate) && !hologramcamera_leave)
		{
			CameraSwitch = 2;
			hologramcamera.Update(dt, CarHologram[i], Aplayer, 5.f);
		}
		UpdateHologram(CarHologram[i], car_Stats[i], &Platform[i], 5.f);
	}

	//change Scene
	if (collision_detection::collision_detection::collision_detector(FakeScreen, CdoorScreen, Aplayer, Cplayer, true)) {
		scenenumber = 2;
		scenechange = true;
	}

	if (CameraSwitch != 2)
	{
	camera.Update(dt, Aplayer);
	firstpersoncamera.Update(dt, Aplayer);
	}

	++framespersecond;
	currentTime = GetTickCount() * 0.001f;
	if (currentTime - lastTime > 1.0f) {
		lastTime = currentTime;
		fps = (int)framespersecond;
		framespersecond = 0;
	}

	for (int i = 0; i < 4; i++)
	{
		UpdateLight(i);
	}

	playerdetails.Update(CarSelection(car_Stats[EquippedCar_Scroll],EquippedCar_Scroll), playerdetails.currency);
	playerdetails.allcardetails.SaveData(car_Stats[0], car_Stats[1], car_Stats[2], car_Stats[3]);
}

void SceneSkybox::Render()
{
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	viewStack.LoadIdentity();
	switch (CameraSwitch)
	{
	case 1:
		viewStack.LookAt(firstpersoncamera.position.x, firstpersoncamera.position.y, firstpersoncamera.position.z, firstpersoncamera.target.x, firstpersoncamera.target.y, firstpersoncamera.target.z, firstpersoncamera.up.x, firstpersoncamera.up.y, firstpersoncamera.up.z);
		break;
	case 2:
		viewStack.LookAt(hologramcamera.position.x, hologramcamera.position.y, hologramcamera.position.z, hologramcamera.target.x, hologramcamera.target.y, hologramcamera.target.z, hologramcamera.up.x, hologramcamera.up.y, hologramcamera.up.z);
		break;
	default:
		viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
		break;
	}
	modelStack.LoadIdentity();

	// passing the light direction if it is a direction light
	for (int i = 0; i < 4; i++)
	{
		// passing the light direction if it is a direction light
		if (light[i].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(light[i].position.x, light[i].position.y, light[i].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[8 + (i * 11)], 1, &lightDirection_cameraspace.x);
		}
		// if it is spot light, pass in position and direction 
		else if (light[i].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * light[i].position;
			glUniform3fv(m_parameters[8 + (i * 11)], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * light[i].spotDirection;
			glUniform3fv(m_parameters[15 + (i * 11)], 1, &spotDirection_cameraspace.x);
		}
		else
		{
			// default is point light (only position since point light is 360 degrees)
			Position lightPosition_cameraspace = viewStack.Top() * light[i].position;
			glUniform3fv(m_parameters[8 + (i * 11)], 1, &lightPosition_cameraspace.x);
		}
	}

	//rand box
	if (godmode) {
		RenderObj(meshList[GEO_RAND_BOX], Arandbox, true, false);
	}
	//slots machine
	RenderObj(meshList[GEO_SLOT_BODY], Aslot_body, false, false);
	RenderObj(meshList[GEO_SLOT_ARM], Aslot_arm, true, false);
	RenderSlotImage(meshList[GEO_SLOT_SCREEN_IMAGE], Aslot_image[0], slot_rotation_lower[slot_images[0] % 5]);
	RenderSlotImage(meshList[GEO_SLOT_SCREEN_IMAGE], Aslot_image[1], slot_rotation_lower[slot_images[1] % 5]);
	RenderSlotImage(meshList[GEO_SLOT_SCREEN_IMAGE], Aslot_image[2], slot_rotation_lower[slot_images[2] % 5]);
	RenderSlotImage(meshList[GEO_SLOT_SCREEN_IMAGE], Aslot_image[3], slot_rotation_whole[slot_images[3] % 5]);
	RenderSlotImage(meshList[GEO_SLOT_SCREEN_IMAGE], Aslot_image[4], slot_rotation_whole[slot_images[4] % 5]);
	RenderSlotImage(meshList[GEO_SLOT_SCREEN_IMAGE], Aslot_image[5], slot_rotation_whole[slot_images[5] % 5]);
	RenderSlotImage(meshList[GEO_SLOT_SCREEN_IMAGE], Aslot_image[6], slot_rotation_upper[slot_images[6] % 5]);
	RenderSlotImage(meshList[GEO_SLOT_SCREEN_IMAGE], Aslot_image[7], slot_rotation_upper[slot_images[7] % 5]);
	RenderSlotImage(meshList[GEO_SLOT_SCREEN_IMAGE], Aslot_image[8], slot_rotation_upper[slot_images[8] % 5]);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 50, 0);
	RenderSkybox();
	modelStack.PopMatrix();

	RenderObj(meshList[GEO_NPC], ANPC, true, false);

	//player
	if (CameraSwitch != 2)
	{
	RenderObj(meshList[GEO_PLAYERBODY], Aplayer, false, false);
	RenderObj(meshList[GEO_PLAYERLEFTARM], Aplayerleftarm, true, false);
	RenderObj(meshList[GEO_PLAYERRIGHTARM], Aplayerrightarm, true, false);
	RenderObj(meshList[GEO_PLAYERLEFTLEG], Aplayerleftleg, true, false);
	RenderObj(meshList[GEO_PLAYERRIGHTLEG], Aplayerrightleg, true, false);
	modelStack.PopMatrix();
	}
	ShopUI.UI.RotateY.degree = 0.f;
	RenderObj(meshList[GEO_SHOP], Shop, true, false);

	//petrol station
	RenderPetrolStation();

	//shop
	RenderShopUI();

	//Render Platform, Car, Wheel

	for (int carnumber = 0; carnumber < 4; carnumber++)
	{
		if (hologramcamera_leave)
		{
			if (carnumber % 2 == 0) CarHologram[carnumber].UI.RotateY.degree = 90.f;
			else CarHologram[carnumber].UI.RotateY.degree = -90.f;
		}
		else CarHologram[carnumber].UI.RotateY.degree = 0.f;

		RenderObj(meshList[GEO_HOLO_PLATFORM], CarHologram[carnumber].UI, false, false);
		RenderStats(CarHologram[carnumber], car_Stats[carnumber]);
		modelStack.Translate(-2.5f, -(3.f / 7.f) * CarHologram[carnumber].lengthY, 0.f);
		modelStack.Scale(1, 0.5f, 1);
		if (!car_Stats[carnumber].lock) RenderText(meshList[GEO_TEXT], "Bought", Color(0, 1, 0));
		else RenderText(meshList[GEO_TEXT], "Cost: " + std::to_string(car_Stats[carnumber].cost), Color(0, 1, 0));
		modelStack.Scale(1, 2.f, 1);
		modelStack.Translate(1.f, (3.f / 7.f) * CarHologram[carnumber].lengthY, 0.f);
		modelStack.PopMatrix();

		RenderObj(meshList[GEO_PLATFORM], Platform[carnumber], false, true);

    RenderCar(carnumber, Cars[carnumber]);
		modelStack.PopMatrix();
	}

	RenderObj(meshList[GEO_DOOR], Door, false, false);
	RenderObj(meshList[GEO_DOORSCREEN], DoorScreen, true, false);
	modelStack.PopMatrix();

	if ((Aplayer.translate - Platform[0].translate).Length() < 15 || (Aplayer.translate - Platform[1].translate).Length() < 15 || (Aplayer.translate - Platform[2].translate).Length() < 15 || (Aplayer.translate - Platform[3].translate).Length() < 15)
	{
		RenderTextOnScreen(meshList[GEO_TEXT], "N and M to rotate platform", Color(0, 1, 0), 2, 0, 4); // Rotate Text
	}

	RenderTextOnScreen(meshList[GEO_TEXT], NPCtext, Color(0, 0, 0), 2, 0, 27);
	RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(fps) + " frames/second", Color(0, 1, 0), 2, 0, 0); //frames
	RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(fps) + " frames/second", Color(0, 1, 0), 2, 0, 0); //frames
	RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(playerdetails.currency), Color(0, 1, 0), 2, 25, 0); //Currency 
	RenderMeshOnScreen(getCarmeshList(EquippedCar_Scroll), 360, 15, 10, 10, 0, 90);
	RenderMeshOnScreen(meshList[GEO_COIN], 300, 20, 10, 10, 0, 0);
}

void SceneSkybox::Exit()
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

Mesh* SceneSkybox::getCarmeshList(int carNumber)
{
	switch (carNumber)
	{
	case 0:
		return meshList[GEO_CAR1BODY];
		break;
	case 1:
		return meshList[GEO_CAR2BODY];
		break;
	case 2:
		return meshList[GEO_CAR3BODY];
		break;
	case 3:
		return meshList[GEO_CAR4BODY];
		break;
	default:
		return meshList[GEO_CAR1BODY];
		break;
	}
}

void SceneSkybox::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneSkybox::RenderSkybox()
{
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(-50.f, 0.f, 0.f);
	modelStack.Scale(103.f, 103.f, 103.f);
	modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(50.f, 0.f, 0.f);
	modelStack.Scale(103.f, 103.f, 103.f);
	modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 50.f, 1.f);
	modelStack.Scale(103.f, 103.f, 103.f);
	modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
	modelStack.Rotate(0.f, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, -50.f, 0.f);
	modelStack.Scale(103.f, 103.f, 103.f);
	modelStack.Rotate(-90.f, 1.f, 0.f, 0.f);
	modelStack.Rotate(90.f, 0.f, 0.f, 1.f);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 0.f, -50.f);
	modelStack.Scale(103.f, 103.f, 103.f);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();
	modelStack.PushMatrix();
	///scale, translate, rotate 
	modelStack.Translate(0.f, 0.f, 50.f);
	modelStack.Scale(103.f, 103.f, 103.f);
	modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();
}

void SceneSkybox::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneSkybox::RenderSlotImage(Mesh* mesh, TRS& trs, int image)
{
	Color color(1, 1, 1);
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
	for (unsigned i = 0; i < 1; ++i) {
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.5f, 0, 0);
		modelStack.PushMatrix();
		modelStack.Translate(trs.translate);
		modelStack.Rotate(trs.RotateX);
		modelStack.Rotate(trs.RotateY);
		modelStack.Rotate(trs.RotateZ);
		modelStack.Scale(trs.Scale);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)(image) * 6, 6);
		modelStack.PopMatrix();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
}

void SceneSkybox::RenderPetrolStation()
{
	RenderObj(meshList[GEO_PETROLSTATION], PetrolStation, true, false);
	RenderCar(EquippedCar_Scroll, PetrolStationCar[EquippedCar_Scroll]);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	RenderObj(meshList[GEO_HOLO_PETROLSTATION], PetrolStationUI.UI, false, false);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	modelStack.Translate(-1.f, 0.f, 0.f);
	modelStack.Scale(0.5f, 0.5f, 0.5f);
	if (car_Stats[EquippedCar_Scroll].isRecharged()) RenderText(meshList[GEO_TEXT], "Recharged", Color(0, 1, 0));
	else RenderText(meshList[GEO_TEXT], "Recharge?", Color(0, 1, 0));
	modelStack.Scale(2.f, 2.f, 2.f);
	modelStack.Translate(1.f, 0.f, 0.f);
	modelStack.PopMatrix();
}

void SceneSkybox::UpdateHologram(HologramUI& UI, CarStats& car_Stats, TRS* ObjectDisplay, float targetY)
{

	if (Application::IsKeyPressed('V') && BounceTime <= GetTickCount())
	{
		if (DistanceCheck(Aplayer.translate, Shop.translate) || DistanceCheck(Aplayer.translate, Platform[0].translate)
			|| DistanceCheck(Aplayer.translate, Platform[1].translate) || DistanceCheck(Aplayer.translate, Platform[2].translate)
			|| DistanceCheck(Aplayer.translate, Platform[3].translate) || DistanceCheck(Aplayer.translate, PetrolStation.translate))
		{
		if (hologramcamera_leave) hologramcamera_leave = false;
		else hologramcamera_leave = true;
		BounceTime = GetTickCount() + 500.f;
		}
	}

	if (DistanceCheck(Aplayer.translate, ObjectDisplay->translate))
	{
		if (ObjectDisplay != &Shop && car_Stats.lock) BuyText = "Cost:" + std::to_string(car_Stats.cost); //if at platform and car is not bought
		else if (ObjectDisplay != &Shop && !car_Stats.lock) BuyText = "Bought"; //if at platform and car is bought
		else if (ObjectDisplay == &Shop && car_Stats.lock) BuyText = "Car:" + std::to_string(car_Stats.cost) + ", CarUpgrade:" + std::to_string(car_Stats.cost_upgrade); //if at shop and car is not bought
		else if (ObjectDisplay == &Shop && !car_Stats.lock) BuyText = "Car:Bought, CarUpgrade:" + std::to_string(car_Stats.cost_upgrade); //if at shop and car is bought
		
		if (Application::IsKeyPressed(VK_RETURN) && BounceTime <= GetTickCount())
		{
			if (playerdetails.currency >= car_Stats.cost && car_Stats.lock && ObjectDisplay != &PetrolStation)
			{ //buying cars(anywhere except petrol station)
				
				car_Stats.BuyCar();
				playerdetails.currency -= car_Stats.cost;
				if (ObjectDisplay != &Shop) BuyText = "Bought";
				
			}
			else if (!car_Stats.lock)
			{
				if (car_Stats.current_upgrade < 5 && ObjectDisplay == &Shop && playerdetails.currency >= car_Stats.cost_upgrade)
				{ //buying upgrades in shop
					car_Stats.UpgradeOnce();
					playerdetails.currency -= car_Stats.cost_upgrade;
					BuyText = "Car:Bought, CarUpgrade:250";
					++car_Stats.current_upgrade;
					if (playerdetails.car_number.SelectedCar.current_upgrade == 4) BuyText = "Car:Bought, CarUpgrade:0";
					car_Stats.UpdateUpgradeCost();
				}
				else if (ObjectDisplay == &PetrolStation && !car_Stats.isRecharged())
				{
					car_Stats.RechargeCarFuel();
				}
			}
			
			BounceTime = GetTickCount() + 500.f;
		}

		if (UI.UI.translate.y < targetY) UI.UI.translate.y += (targetY / 20.f);
		if (UI.UI.Scale.x < 1.f) UI.UI.Scale.x += 0.05f;


		for (int i = 0; i < 5; ++i)
		{
			if (car_Stats.StatTRS[i].Scale.x < (car_Stats.StatLevel[i] / 14.f) * UI.lengthX) car_Stats.StatTRS[i].Scale.x += 0.05f;
			else if (car_Stats.StatUpgrade.Scale.x < 1.f / 14.f * UI.lengthX) car_Stats.StatUpgrade.Scale.x += 0.03f;
		}
	} //end distance check

	else
	{
		if (UI.UI.translate.y > 0.f) UI.UI.translate.y -= (targetY / 20.f);
		if (UI.UI.Scale.x > 0.f) UI.UI.Scale.x -= 0.05f;
	}


}

void SceneSkybox::UpdateEquippedCar()
{	 	
	++EquippedCar_Scroll;
	if (EquippedCar_Scroll == 4) EquippedCar_Scroll = 0;

	while (car_Stats[EquippedCar_Scroll].lock)
	{
		++EquippedCar_Scroll;
	if (EquippedCar_Scroll == 4) EquippedCar_Scroll = 0;
	}
	
	playerdetails.car_number.EquipCar(car_Stats[EquippedCar_Scroll], EquippedCar_Scroll);
}

void SceneSkybox::InitPetrolStationCar()
{
	for (int i = 0; i < 4; ++i)
	{
		PetrolStationCar[i] = Cars[i];
		PetrolStationCar[i].translate = PetrolStation.translate + Vector3(-6.f, 0.f, 5.f) + Vector3(0.f,Cars[i].translate.y,0.f);
	}
}

void SceneSkybox::UpdateLight(int carnumber)
{
	if (car_Stats[carnumber].lock == false)
	{
		light[carnumber].color.Set(0.f, 1.f, 0.f);
		glUniform3fv(m_parameters[9 + (carnumber * 11)], 1, &light[carnumber].color.r);
	}
}


void SceneSkybox::RenderCar(int carnumber, TRS Car)
{

	RenderObj(getCarmeshList(carnumber), Car, false, false);

	for (int carnumwheel = 0; carnumwheel < 4; carnumwheel++)
	{
		if (carnumber == 0)
		{
			RenderObj(meshList[GEO_CAR1WHEEL], CarWheel[carnumber][carnumwheel], true, false);
		}
		else if (carnumber == 1)
		{
			RenderObj(meshList[GEO_CAR2WHEEL], CarWheel[carnumber][carnumwheel], true, false);
		}
		else if (carnumber == 2)
		{
			RenderObj(meshList[GEO_CAR3WHEEL], CarWheel[carnumber][carnumwheel], true, false);
		}
		else if (carnumber == 3)
		{
			RenderObj(meshList[GEO_CAR4WHEEL], CarWheel[carnumber][carnumwheel], true, false);
		}
	}
	modelStack.PopMatrix();
}

void SceneSkybox::RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, float rotateX, float rotateY)
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

void SceneSkybox::RenderShopText()
{
	modelStack.Translate(-0.5f * ShopUI.lengthX, 3.f / 7.f * ShopUI.lengthY, 0.f);
	modelStack.Scale(0.5f, 1.f, 1.f);
	RenderText(meshList[GEO_TEXT], BuyText, Color(0, 1, 0));

	modelStack.Translate(0.f, -1.f / 14.f * ShopUI.lengthY, 0.f);
	RenderText(meshList[GEO_TEXT], "CurrentUpgrade:" + std::to_string(car_Stats[ShopUI_Scroll].current_upgrade), Color(0, 1, 0));

	modelStack.Translate(0.f, 1.f / 14.f * ShopUI.lengthY, 0.f);

	modelStack.Scale(2.f, 1.f, 1.f);
	modelStack.Translate(0.5f * ShopUI.lengthX, -3.f / 7.f * ShopUI.lengthY, 0.f);
}

void SceneSkybox::RenderShopUI()
{
	ShopUI.UI.RotateY.degree = 0.f;
	RenderObj(meshList[GEO_SHOP], Shop, true, false);
	//shop

	if (DistanceCheck(Aplayer.translate, Shop.translate))
	{
		RenderObj(meshList[GEO_HOLO_SHOP], ShopUI.UI, false, false);
		RenderShopStats(car_Stats[ShopUI_Scroll]);
		RenderShopText();

		modelStack.PushMatrix();
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(0.4f, 0.4f, 0.4f);
		RenderCar(ShopUI_Scroll, Cars[ShopUI_Scroll]);
		modelStack.Scale(2.5f, 2.5f, 2.5f);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.PopMatrix();

		modelStack.PopMatrix(); //shop
	}
}

void SceneSkybox::PlayerMoveUp(double dt)
{
	Aplayer.translate.z += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	Aplayer.translate.x += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);

	if (!(Aplayer.translate.x > -48 && Aplayer.translate.x < 48 && Aplayer.translate.z > -48 && Aplayer.translate.z < 48)) {
		Aplayer.translate.z -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detection::collision_detector(Aplayer, Cplayer, Aslot_body, Cslot_body, true)) {
		Aplayer.translate.z -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, ANPC, CNPC, true)) {
		Aplayer.translate.z -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, Shop, CShop, true)) {
		Aplayer.translate.z -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	for (int i = 0; i < 4; i++)
	{
		if (collision_detection::collision_detector(Aplayer, Cplayer, Platform[i], PlatformR))
		{
			Aplayer.translate.z -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
			Aplayer.translate.x -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		}
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, Arandbox, Crandbox, true) && godmode) {
		Aplayer.translate.z -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, Door, CDoor, true)) {
		Aplayer.translate.z -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, PetrolStation, CPetrolstation)) {
		Aplayer.translate.z -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, PetrolStationCar[EquippedCar_Scroll], CCars[EquippedCar_Scroll])) {
		Aplayer.translate.z -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
}

void SceneSkybox::PlayerMoveDown(double dt)
{
	Aplayer.translate.z -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	Aplayer.translate.x -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);

	if (!(Aplayer.translate.x > -48 && Aplayer.translate.x < 48 && Aplayer.translate.z > -48 && Aplayer.translate.z < 48)) {
		Aplayer.translate.z += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, Aslot_body, Cslot_body, true)) {
		Aplayer.translate.z += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	for (int i = 0; i < 4; i++)
	{
		if (collision_detection::collision_detector(Aplayer, Cplayer, Platform[i], PlatformR))
		{
			Aplayer.translate.z += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
			Aplayer.translate.x += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		}
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, ANPC, CNPC, true)) {
		Aplayer.translate.z += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, Shop, CShop, true)) {
		Aplayer.translate.z += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, Arandbox, Crandbox, true) && godmode) {
		Aplayer.translate.z += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, Door, CDoor, true)) {
		Aplayer.translate.z += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, PetrolStation, CPetrolstation)) {
		Aplayer.translate.z += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, PetrolStationCar[EquippedCar_Scroll], CCars[EquippedCar_Scroll])) {
		Aplayer.translate.z += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}

}

void SceneSkybox::PlayerMoveRight(double dt)
{
	Aplayer.translate.z += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	Aplayer.translate.x -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);

	if (!(Aplayer.translate.x > -48 && Aplayer.translate.x < 48 && Aplayer.translate.z > -48 && Aplayer.translate.z < 48)) {
		Aplayer.translate.z -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, Aslot_body, Cslot_body)) {
		Aplayer.translate.z -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	for (int i = 0; i < 4; i++)
	{
		if (collision_detection::collision_detector(Aplayer, Cplayer, Platform[i], PlatformR))
		{
			Aplayer.translate.z -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
			Aplayer.translate.x += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		}
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, ANPC, CNPC, true)) {
		Aplayer.translate.z -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, Shop, CShop, true)) {
		Aplayer.translate.z -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, Arandbox, Crandbox, true) && godmode) {
		Aplayer.translate.z -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, Door, CDoor, true)) {
		Aplayer.translate.z -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, PetrolStation, CPetrolstation)) {
		Aplayer.translate.z -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, PetrolStationCar[EquippedCar_Scroll], CCars[EquippedCar_Scroll])) {
		Aplayer.translate.z -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
}

void SceneSkybox::PlayerMoveLeft(double dt)
{
	Aplayer.translate.z -= sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	Aplayer.translate.x += cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);

	if (!(Aplayer.translate.x > -48 && Aplayer.translate.x < 48 && Aplayer.translate.z > -48 && Aplayer.translate.z < 48)) {
		Aplayer.translate.z += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, Aslot_body, Cslot_body)) {
		Aplayer.translate.z += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	for (int i = 0; i < 4; i++)
	{
		if (collision_detection::collision_detector(Aplayer, Cplayer, Platform[i], PlatformR))
		{
			Aplayer.translate.z += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
			Aplayer.translate.x -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		}
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, ANPC, CNPC, true)) {
		Aplayer.translate.z += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, Shop, CShop, true)) {
		Aplayer.translate.z += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, Arandbox, Crandbox, true) && godmode) {
		Aplayer.translate.z += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, Door, CDoor, true)) {
		Aplayer.translate.z += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, PetrolStation, CPetrolstation)) {
		Aplayer.translate.z += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}
	if (collision_detection::collision_detector(Aplayer, Cplayer, PetrolStationCar[EquippedCar_Scroll], CCars[EquippedCar_Scroll])) {
		Aplayer.translate.z += sin(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
		Aplayer.translate.x -= cos(Math::DegreeToRadian(Aplayer.RotateY.degree)) * (float)(playerMovementSpeed * dt);
	}

}

void SceneSkybox::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SceneSkybox::RenderObj(Mesh* mesh, TRS& trs, bool end, bool enableLight)
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

bool SceneSkybox::DistanceCheck(Vector3 Object1, Vector3 Object2) //true: if they are close enough
{
	Vector3 Distance = Object1 - Object2;
	if (Distance.Length() <= 15.f) return true;
	else return false;
}

void SceneSkybox::RenderStats(HologramUI UI, CarStats& car_Stats)
{
	if (UI.UI.Scale.x >= 1.f)
	{
		car_Stats.InitStatsinHolo(UI);
		for (int i = 0; i < 5; ++i)
		{
			car_Stats.StatTRS[i].Scale.y = 1.f / 14.f * UI.lengthY;
			RenderObj(meshList[GEO_CAR_STAT], car_Stats.StatTRS[i], true, false);
		}

	} //end ui scale.x if 

}

void SceneSkybox::RenderShopStats(CarStats& car_Stats)
{
	if (ShopUI.UI.Scale.x >= 1.f)
	{
		car_Stats.InitStatsinShop(ShopUI);

		for (int i = 0; i < 5; ++i)
		{
			car_Stats.StatTRS[i].Scale.y = 0.25f / 14.f * ShopUI.lengthY;
			RenderObj(meshList[GEO_CAR_STAT], car_Stats.StatTRS[i], true, false);
		}

		if (car_Stats.current_upgrade < 5)
		{
			for (int i = 0; i < 5; ++i)
			{
				car_Stats.StatUpgrade.Scale.y = 0.25f / 14.f * ShopUI.lengthY;
				car_Stats.StatUpgrade.translate = Vector3(car_Stats.StatTRS[i].translate.x + (((1.f + car_Stats.StatLevel[i]) / 28.f) * ShopUI.lengthX), car_Stats.StatTRS[i].translate.y, 0.1f);
				RenderObj(meshList[GEO_CAR_STAT_UPGRADE], car_Stats.StatUpgrade, true, false);
			}
		}


	} //end ui scale.x if 
}
