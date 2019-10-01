#pragma once

#include "olcPixelGameEngine.h"
#include <iostream>
#include <map>

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

	void LoadSprites();

private:
	Assets();
	~Assets();

	std::map<std::string, olc::Sprite*> mapSprites;
};

