#include "HologramUI.h"

HologramUI::HologramUI()
{ 
	UI.Scale = Vector3(0.f, 1.f, 1.f); 
}

HologramUI::HologramUI(float lengthX, float lengthY)
{
	this->lengthX = lengthX;
	this->lengthY = lengthY;
	UI.Scale = Vector3(0.f, 1.f, 1.f);
}
