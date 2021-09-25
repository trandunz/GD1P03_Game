#include "CTextureMaster.h"

CTextureMaster::CTextureMaster()
{
	//Textures
	m_Dirt = new sf::Texture();
	m_Dirt->loadFromFile("Images/Dirt.png"); // Dirt
	m_Grass = new sf::Texture();
	m_Grass->loadFromFile("Images/Grass.png"); // Grass
	m_MousePosTex = new sf::Texture();
	m_MousePosTex->loadFromFile("Images/MousePos.png"); // Mouse Pos Indicator
	m_Sky = new sf::Texture();
	m_Sky->loadFromFile("Images/TerrariaBG.jpg"); // Sky Blue
	m_Sky->setSmooth(true);
	m_ItemSpacer = new sf::Texture();
	m_ItemSpacer->loadFromFile("Images/ItemSpacer.png");
	m_HeartFull = new sf::Texture();
	m_HeartFull->loadFromFile("Images/HeartFull.png");
	m_HeartEmpty = new sf::Texture();
	m_HeartEmpty->loadFromFile("Images/HeartEmpty.png");
	m_CIITexture = new sf::Texture();
	m_CIITexture->loadFromFile("Images/CurrentItem.png");
	m_Planks = new sf::Texture();
	m_Planks->loadFromFile("Images/Planks.png");
	m_Stone = new sf::Texture();
	m_Stone->loadFromFile("Images/Stone.png");
	m_Stone->setRepeated(true);
	m_Wood = new sf::Texture();
	m_Wood->loadFromFile("Images/Wood.png");
	m_Sand = new sf::Texture();
	m_Sand->loadFromFile("Images/Sand.png");
	m_MossyBrick = new sf::Texture();
	m_MossyBrick->loadFromFile("Images/MossyBrick.png");
	m_MousePointerTex = new sf::Texture();
	m_MousePointerTex->loadFromFile("Images/Mouse.png");
	m_Chest = new sf::Texture();
	m_Chest->loadFromFile("Images/Chest.png");
	m_Leaves = new sf::Texture();
	m_Leaves->loadFromFile("Images/Leaves.png");
	m_Obsidian = new sf::Texture();
	m_Obsidian->loadFromFile("Images/Obsidian.png");
	m_IronOre = new sf::Texture();
	m_IronOre->loadFromFile("Images/IronOre.png");
	m_Coal = new sf::Texture();
	m_Coal->loadFromFile("Images/CoalOre.png");
	m_GoldOre = new sf::Texture();
	m_GoldOre->loadFromFile("Images/GoldOre.png");
	m_DiamondOre = new sf::Texture();
	m_DiamondOre->loadFromFile("Images/DiamondOre.png");
	m_Furnace = new sf::Texture();
	m_Furnace->loadFromFile("Images/Furnace.png");
	m_DoorLeft = new sf::Texture();
	m_DoorLeft->loadFromFile("Images/DoorOpenLeft.png");
	m_Bow = new sf::Texture();
	m_Bow->loadFromFile("Images/BowLeft.png");
	m_IronIngot = new sf::Texture();
	m_IronIngot->loadFromFile("Images/IronIngot.png");
	m_GoldIngot = new sf::Texture();
	m_GoldIngot->loadFromFile("Images/GoldIngot.png");
	m_DiamondIngot = new sf::Texture();
	m_DiamondIngot->loadFromFile("Images/DiamondIngot.png");
	m_UnderGroundPlains = new sf::Texture();
	m_UnderGroundPlains->loadFromFile("Images/UnderGroundPlains.png");
	m_UnderGroundPlains->setRepeated(true);
	m_Cloud = new sf::Texture();
	m_Cloud->loadFromFile("Images/Cloud.png");
	m_CloudDark = new sf::Texture();
	m_CloudDark->loadFromFile("Images/CloudDark.png");
	m_WorkBench = new sf::Texture();
	m_WorkBench->loadFromFile("Images/WorkBench.png");

	// Set Smooth
	m_Dirt->setSmooth(true);
	m_Grass->setSmooth(true);
	m_Sky->setSmooth(true);
	m_ItemSpacer->setSmooth(true);
	m_HeartFull->setSmooth(true);
	m_HeartEmpty->setSmooth(true);
	m_CIITexture->setSmooth(true);
	m_Planks->setSmooth(true);
	m_Stone->setSmooth(true);
	m_Sand->setSmooth(true);
	m_Wood->setSmooth(true);
	m_MossyBrick->setSmooth(true);
	m_MousePointerTex->setSmooth(true);
	m_Chest->setSmooth(true);
	m_Leaves->setSmooth(true);
	m_Obsidian->setSmooth(true);
	m_IronOre->setSmooth(true);
	m_Coal->setSmooth(true);
	m_GoldOre->setSmooth(true);
	m_DiamondOre->setSmooth(true);
	m_Furnace->setSmooth(true);
	m_DoorLeft->setSmooth(true);
	m_Bow->setSmooth(true);
	m_IronIngot->setSmooth(true);
	m_GoldIngot->setSmooth(true);
	m_DiamondIngot->setSmooth(true);
	m_WorkBench->setSmooth(true);
}

CTextureMaster::~CTextureMaster()
{
	delete m_WorkBench;
	delete m_UnderGroundPlains;
	delete m_DoorLeft;
	delete m_Furnace;
	delete m_Planks;
	delete m_HeartEmpty;
	delete m_HeartFull;
	delete m_Sky;
	delete m_MousePosTex;
	delete m_Dirt;
	delete m_Grass;
	delete m_ItemSpacer;
	delete m_CIITexture;
	delete m_Stone;
	delete m_Wood;
	delete m_Sand;
	delete m_MossyBrick;
	delete m_MousePointerTex;
	delete m_Chest;
	delete m_Leaves;
	delete m_Obsidian;
	delete m_IronOre;
	delete m_Coal;
	delete m_GoldOre;
	delete m_DiamondOre;
	delete m_Bow;
	delete m_IronIngot;
	delete m_GoldIngot;
	delete m_DiamondIngot;
	delete m_Cloud;
	delete m_CloudDark;
	m_CloudDark = nullptr;
	m_Cloud = nullptr;
	m_UnderGroundPlains = nullptr;
	m_DiamondIngot = nullptr;
	m_GoldIngot = nullptr;
	m_IronIngot = nullptr;
	m_Bow = nullptr;
	m_DoorLeft = nullptr;
	m_Furnace = nullptr;
	m_IronOre = nullptr;
	m_Coal = nullptr;
	m_GoldOre = nullptr;
	m_DiamondOre = nullptr;
	m_Obsidian = nullptr;
	m_Leaves = nullptr;
	m_Chest = nullptr;
	m_MousePointerTex = nullptr;
	m_MossyBrick = nullptr;
	m_Stone = nullptr;
	m_Wood = nullptr;
	m_Sand = nullptr;
	m_Planks = nullptr;
	m_CIITexture = nullptr;
	m_HeartEmpty = nullptr;
	m_HeartFull = nullptr;
	m_ItemSpacer = nullptr;
	m_Sky = nullptr;
	m_MousePosTex = nullptr;
	m_Dirt = nullptr;
	m_Grass = nullptr;
}
