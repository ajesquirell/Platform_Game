#pragma once

#include <iostream>
#include <map>

#include "olcPixelGameEngine.h"
#include "olcPGEX_Sound.h"
#include "Maps.h"
#include "Items.h"
//class cMap; //Because we can't include Maps.h - Maps.h includes Tiles.h which includes this header.

/*+++++++++++++++
	Should really figure out how to throw exceptions instead of just outputing an error when things aren't loaded right here
+++++++++++++++*/


class Assets //Singleton     (Like managers with Hololens! or one gigantic global variable)
{
public:
	static Assets& get() //"static" makes it a method applied directly to class, don't need to create an instance to use
	{
		static Assets me;
		return me;
	}

	//Ensure use as singleton
	Assets(Assets const&) = delete; //Delete default copy constructor
	void operator=(Assets const&) = delete; //Make sure the one global instance can't be duplicated


	olc::Sprite* GetSprite(std::string name)
	{
		if (mapSprites[name] == nullptr)
			std::cerr << "\nError: Could not retrieve sprite with the name \"" << name << "\". Please ensure it exists on disk and is loaded from Assets class.\n";

		return mapSprites[name]; //If "name" is specified wrongly or doesn't exist, it will just return a blank sprite
	}

	cMap* GetMap(std::string name)
	{
		if (mapMaps[name] == nullptr)
			std::cerr << "\nError: Could not retrieve map/level with the name \"" << name << "\". Please ensure it exists on disk and is loaded from Assets class.\n";

		return mapMaps[name];
	}

	cItem* GetItem(std::string name)
	{
		if (mapItems[name] == 0)
			std::cerr << "\nError: Could not retrieve item with the name \"" << name << "\". Please ensure it exists on disk and is loaded from Assets class.\n";

		switch (mapItems[name])
		{
		case 1:
			return new cItem_Health;
			break;
		case 2:
			return new cItem_HealthBoost;
			break;
		case 3:
			return new cItem_FlamesCash;
			break;
		case 4:
			return new cWeapon_Sword;
			break;
		case 5:
			return new cWeapon_Pistol;
			break;
		}
	}

	int GetSound(std::string name)
	{
		if (mapSounds[name] == 0)
			std::cerr << "\nError: Could not retrieve sound with the name \"" << name << "\". Please ensure it exists on disk and is loaded from Assets class.\n";

		return mapSounds[name];
	}

	void LoadSprites();
	void LoadMaps();
	void LoadItems();
	void LoadSounds();

private:
	Assets();
	~Assets();

	std::map<std::string, olc::Sprite*> mapSprites;
	std::map<std::string, cMap*> mapMaps;
	std::map<std::string, int> mapItems;
	std::map<std::string, int> mapSounds;
};

