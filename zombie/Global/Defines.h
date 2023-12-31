#pragma once

#define _USE_MATH_DEFINES


enum class Origins
{
	TL,
	TC,
	TR,
	ML,
	MC,
	MR,
	BL,
	BC,
	BR,
	CUSTOM,
};

enum class ResourceTypes
{
	Texture,
	Font,
	SoundBuffer,
};

// minji, 230707
enum class SceneId
{
	None = -1,
	Title,
	Dev1,
	Count,
};

