#ifndef DRIVESCENE_H
#define DRIVESCENE_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "Mesh.h"
#include "Light.h"
#include "CItemList.h"
#include "FirstPersonCamera.h"
#include "Loadcornercoord.h"
#include "Collision_detector.h"
#include "CarStats.h"

class DriveScene : public Scene
{
	enum GEOMETRY_TYPE
	{
		GEO_AXES = 0,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_LIGHTSPHERE,
		GEO_TEXT,
		GEO_CARBODY,
		GEO_CARWHEEL,
		GEO_COIN,
		GEO_BOOSTPAD,
		GEO_SPIKE,
		GEO_BOX,
		GEO_FIRE,
		GEO_GHOSTCOIN,
		GEO_SHIELDCOIN,
		GEO_SHIELD,
		NUM_GEOMETRY,
	};

	enum Scene5_UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		//add these enum in UNIFORM_TYPE before U_TOTAL
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		// light 1
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_LIGHT1ENABLED,
		U_NUMLIGHTS,
		// add these enum for texture
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,
		//text
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	/*unsigned m_indexBuffer;*/
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_parameters[U_TOTAL];

	MS modelStack, viewStack, projectionStack;
	Light light[2];

	//FirstPersonCamera firstpersoncamera;
	Camera2 test;
	//int CameraSwitch;
	//float SwitchCD;
	CarStats car_Stats;
	CItemList boxlist;
	CItemList spikelist;
	CItemList boostpadlist;
	CItemList coinlist;

	// Coin randomizer
	int check[20] = { 0 };
	int coinarray[10] = { 0 };
	int count = 0;

	//powerup randomizer
	int randompower;

	int fps;
	float framespersecond;
	float lastTime;
	float currentTime;

	// Car movements variable
	float carVelocity;
	float carTurningSpeed;
	float carAcceleration;
	float friction;
	float boostbar;
	float boostAcceleration;
	float boostVelocity;
	float maxvelocity;
	float fuel;
	bool car_ismoving;

	int countDown;
	bool carcanmove;

	int Acarnumber;
	int health;
	float iFrames;

	std::string startingtext;
	std::string coinrespawned;
	float showtext;
	float showcointexttime;
	float timetorespawncoin;
	bool coingeneratearray;
	const float playerMovementSpeed = 10.f;
	const float playerTurningSpeed = 135.f;

	TRS ABuilding[4];
	TRS Spike;
	cornercoord CSpike;
	TRS coin;
	cornercoord CCoin;
	TRS Box;
	cornercoord CBox;
	TRS Boostpad;
	cornercoord CBoostpad;

	//cars
	TRS ACarBody;
	cornercoord CCarBody;
	TRS ACarWheel[4];

	cornercoord Cskybox;
	TRS Askybox;

	//power-ups
	TRS AShields[4];
	float shield_turning_speed = 90;
	TRS Powerups[2];
	cornercoord CPowerups[2];
	// 0 = Ghost, 1 = Shield
	bool Powerup_onmap[2];
	bool Powerup_onplayer[2];
	int shield_health;
	float ghost_time;
	bool collision_in_box_in_frame;

	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderObj(Mesh* mesh, TRS& trs, bool end, bool enableLight);
	void carMovement(TRS carbody, float& velocity, double dt);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, float rotateX, float rotateY);

	//coin random generator
	void Generatecoinposition();
	void CoinpositionSelector(bool);
	void Generatepowerposition();
	void CoinRespawn();
	int coinchecker;
	int randomcoin;

public:
	DriveScene();
	~DriveScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};

#endif