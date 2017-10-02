#include "light.h"

phatLight::phatLight()
{
	gLightIndex++;

	// Default to directional light
	ZeroMemory(&mLight, sizeof(D3DLIGHT9));    // clear out the light struct for use
	mLight.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
	mLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set the light's color
	mLight.Direction = D3DXVECTOR3(1.0f, -0.3f, -1.0f);
	mLight.Range = 1000.0f;
	mLight.Falloff = 100.0f;

	ZeroMemory(&mMaterial, sizeof(D3DMATERIAL9));    // clear out the struct for use
	mMaterial.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
	mMaterial.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set ambient color to white
}

phatLight::~phatLight()
{
	gLightIndex--;
}

void phatLight::createPoint()
{
	// Fill in a mLight structure defining our mLight
	ZeroMemory( &mLight, sizeof(D3DLIGHT9) );
	mLight.Type			= D3DLIGHT_POINT;
	mLight.Diffuse.r	= 0.0f;
	mLight.Diffuse.g	= 1.0f;
	mLight.Diffuse.b	= 0.0f;
	mLight.Diffuse.a	= 1.0f;

	// Point mLights have no direction but do have a position
	mLight.Position = D3DXVECTOR3( 5.0f, 5.0f, 1.0f );

	mLight.Attenuation0 = 0.0f;
	mLight.Attenuation1 = 0.125f;    // only .125 inverse attenuation
	mLight.Attenuation2 = 0.0f;    // no square inverse attenuation

	mLight.Range=50.0f;
}

void phatLight::createDir()
{
}

void phatLight::createSpot()
{
}