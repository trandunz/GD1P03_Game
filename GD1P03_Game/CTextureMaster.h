#pragma once
#ifndef _TEXTUREMASTER_H__
#define _TEXTUREMASTER_H__

#include <SFML/Graphics.hpp>
#include <iostream>
class CTextureMaster
{
public:
	CTextureMaster();
	~CTextureMaster();

	// Textures
	sf::Texture* m_MousePosTex;
	sf::Texture* m_Dirt;
	sf::Texture* m_Grass;
	sf::Texture* m_Sky;
	sf::Texture* m_SandBG;
	sf::Texture* m_ItemSpacer;
	sf::Texture* m_EquipmentSpacer;
	sf::Texture* m_HeartFull;
	sf::Texture* m_HeartEmpty;
	sf::Texture* m_CIITexture;
	sf::Texture* m_Planks;
	sf::Texture* m_Stone;
	sf::Texture* m_Wood;
	sf::Texture* m_Sand;
	sf::Texture* m_MossyBrick;
	sf::Texture* m_MousePointerTex;
	sf::Texture* m_Chest;
	sf::Texture* m_Leaves;
	sf::Texture* m_Obsidian;
	sf::Texture* m_Coal;
	sf::Texture* m_IronOre;
	sf::Texture* m_GoldOre;
	sf::Texture* m_DiamondOre;
	sf::Texture* m_Furnace;
	sf::Texture* m_DoorLeft;
	sf::Texture* m_Bow;
	sf::Texture* m_IronIngot;
	sf::Texture* m_GoldIngot;
	sf::Texture* m_DiamondIngot;
	sf::Texture* m_UnderGroundPlains;
	sf::Texture* m_UnderGroundSand;
	sf::Texture* m_Cloud;
	sf::Texture* m_CloudDark;
	sf::Texture* m_WorkBench;
	sf::Texture* m_PurpleIngot;
	sf::Texture* m_GoldenIngot;
	sf::Texture* m_Anvil;
	sf::Texture* m_Tourch;
	sf::Texture* m_GoldenOre;
	sf::Texture* m_PurpleOre;
	sf::Texture* m_Arrow;
	sf::Texture* m_FireArrow;
	sf::Texture* m_CursedArrow;
	sf::Texture* m_PoisonArrow;
	sf::Texture* m_GlassBeaker;
	sf::Texture* m_RedSlime;
	sf::Texture* m_HPSmall;
	sf::Texture* m_HPLarge;
	sf::Texture* m_GoldenHeart;
	sf::Texture* m_PortalKeyBlue;
	sf::Texture* m_PortalKeyRed;
	sf::Texture* m_PortalKeyYellow;
	sf::Texture* m_SandStone;
	sf::Texture* m_HardSandStone;
	sf::Texture* m_Cactas;
	sf::Texture* m_PortalKeyPlains;
	sf::Texture* m_UnderGroundIce;
	sf::Texture* m_IceBG;
	sf::Texture* m_Snow;
	sf::Texture* m_LeavesIce;
	sf::Texture* m_DirtHell;
	sf::Texture* m_StoneHell;
	sf::Texture* m_BrickHell;
	sf::Texture* m_UnderGroundHell;
	sf::Texture* m_HellBG;
	sf::Texture* m_TerrariaHellBG;

	sf::Texture* m_CactusHead;
	sf::Texture* m_CactusChestPlate;
	sf::Texture* m_CactusLegs;

	sf::Texture* m_IronHead;
	sf::Texture* m_IronChestPlate;
	sf::Texture* m_IronLegs;

	sf::Texture* m_EquipmentSpacerHelmet;
	sf::Texture* m_EquipmentSpacerChestPlate;
	sf::Texture* m_EquipmentSpacerLeggings;

	sf::Texture* m_EquipmentSpacerTourch;
	sf::Texture* m_EquipmentSpacerArrow;
	sf::Texture* m_EquipmentSpacerBullet;


};
#endif
