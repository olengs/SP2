 #ifndef SCENESKYBOX_H
#define SCENESKYBOX_H

#include "Scene.h"
#include <MatrixStack.h>
#include "Camera2.h"
#include "FirstPersonCamera.h"
#include "Mesh.h"
#include "Light.h"
#include "Loadcornercoord.h"
#include "Collision_detector.h"
#include "CItemList.h"
#include "CarStats.h"
#include "HologramUI.h"
#include "HologramCamera.h"
#include "CarSelection.h"
#include "AllCarDetails.h"

class SceneSkybox : public Scene
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
		GEO_CHAR,
		GEO_DICE,
		GEO_LIGHTSPHERE,
		GEO_TEXT,
		GEO_NPC,
		GEO_SHOP,
		GEO_HOLO_SHOP,
		GEO_SHOP_NPC,
		GEO_CAR_STAT,
		GEO_CAR_STAT_UPGRADE,
		GEO_CAR1BODY,
		GEO_CAR2BODY,
		GEO_CAR3BODY,
		GEO_CAR4BODY,
		GEO_CAR1WHEEL,
		GEO_CAR2WHEEL,
		GEO_CAR3WHEEL,
		GEO_CAR4WHEEL,
		GEO_PLATFORM,
		GEO_SLOT_BODY,
		GEO_SLOT_ARM,
		GEO_SLOT_SCREEN_IMAGE,
		GEO_DOOR,
		GEO_DOORSCREEN,
		GEO_FAKEDOORSCREEN,
		GEO_PETROLSTATION,
		GEO_HOLO_PETROLSTATION,
		GEO_HOLO_PLATFORM,
		GEO_COIN,
		GEO_PLAYERLEFTARM,
		GEO_PLAYERRIGHTARM,
		GEO_PLAYERBODY,
		GEO_PLAYERLEFTLEG,
		GEO_PLAYERRIGHTLEG,
		GEO_RAND_BOX,
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
		U_LIGHTENABLED,

		//Light 0
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
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

		// light 2
		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_TYPE,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		// light 3
		U_LIGHT3_POSITION,
		U_LIGHT3_COLOR,
		U_LIGHT3_POWER,
		U_LIGHT3_KC,
		U_LIGHT3_KL,
		U_LIGHT3_KQ,
		U_LIGHT3_TYPE,
		U_LIGHT3_SPOTDIRECTION,
		U_LIGHT3_COSCUTOFF,
		U_LIGHT3_COSINNER,
		U_LIGHT3_EXPONENT,

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
	Light light[4];
	TRS lightball[4];

	Camera2 camera;
	FirstPersonCamera firstpersoncamera;
	HologramCamera hologramcamera;

	int fps;
	float framespersecond;
	float lastTime;
	float currentTime;

	//characters
	TRS Aplayer;
	cornercoord Cplayer;
	//NPC
	TRS ANPC;
	cornercoord CNPC;
	std::string NPCSpeech[4];
	int carnum;
	int textnum;
	float textLasttime;
	std::string NPCtext;

	TRS Aplayerleftarm;
	TRS Aplayerleftleg;
	TRS Aplayerrightarm;
	TRS Aplayerrightleg;
	bool rotatebodyparts;
	bool movebodyupdown;

	//player movement
	const float playerMovementSpeed = 15.f;
	const float playerTurningSpeed = 135.f;
	//petrol station
	TRS PetrolStation;
	cornercoord CPetrolstation;
	HologramUI PetrolStationUI;
	TRS PetrolStationCar[4];

	//shop
	TRS Shop;
	cornercoord CShop;
	HologramUI ShopUI;
	CarStats car_Stats[4];
	AllCarDetails allcardetails;
	Mesh* getCarmeshList(int carNumber);

	int ShopUI_Scroll;
	int EquippedCar_Scroll;
	//UIText[0] - Movement text
	//UIText[1] - Camera movement text
	//UIText[2] - Camera Toggle (between 1st and 3rd)

	int CameraSwitch;
	bool CameraToggle;
	bool hologramcamera_leave;	//Toggle between first and third person cam, false = 1st, true = 3rd
	double BounceTime;
	//For camera to toggle without immediately switching back

	//cars
	TRS Platform[4];
	float PlatformR;
	TRS Cars[4];
	TRS CarWheel[4][4];
	cornercoord CCars[4];

	//door 
	TRS Door;
	TRS DoorCheck;
	TRS DoorScreen;
	TRS FakeScreen;
	cornercoord CdoorScreen;
	cornercoord CDoor;

	//slot machine
	TRS Aslot_body;
	cornercoord Cslot_body;
	TRS Aslot_arm;
	TRS Aslot_image[9];
	//slot_images - top left , top middle , top right
//				middle left, middle middle, middle right
//				bottom left, bottom middle, bottom right
	int slot_images[9];

	int slot_rotation_whole[5];
	int slot_rotation_upper[5];
	int slot_rotation_lower[5];
	bool activate_slot_machine;
	float row1_lastTime;
	float row2_lastTime;
	float row3_lastTime;
	int stop_machine;
	float slot_stop_lasttime;

	//random box
	TRS Arandbox;
	cornercoord Crandbox;
	bool godmode;
	float godmodelasttime;

	std::string BuyText;
	HologramUI CarHologram[4];


	bool DistanceCheck(Vector3 Object1, Vector3 Object2);

	//RenderFunctions
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderObj(Mesh* mesh, TRS& trs, bool end, bool enableLight);
	void RenderStats(HologramUI UI, CarStats& car_Stats);
	void RenderShopStats(CarStats& car_Stats);
	void RenderSlotImage(Mesh* mesh, TRS& trs, int image);
	void RenderPetrolStation();
	void RenderCar(int carnumber, TRS Car);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey, float rotateX, float rotateY);
	void RenderShopText();
	void RenderShopUI();

	//Update Functions
	void UpdateHologram(HologramUI& UI, CarStats& car_Stats, TRS* ObjectDisplay, float targetY);
	void UpdateEquippedCar();
	void InitPetrolStationCar();
	void UpdateLight(int);

	//player movement
	void PlayerMoveUp(double dt);
	void PlayerMoveDown(double dt);
	void PlayerMoveRight(double dt);
	void PlayerMoveLeft(double dt);

public:
	SceneSkybox();
	~SceneSkybox();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
};
#endif