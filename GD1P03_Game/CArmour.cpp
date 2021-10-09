#include "CArmour.h"

CArmour::CArmour(CBlock::BLOCKTYPE _itemType, CBlock::ARMOURTYPE _armourtype)
{
	m_Scale = 50.0f;
	m_Body = nullptr;
	m_Type = _itemType;
	m_Texture = nullptr;
	m_Texture = new sf::Texture();
	m_ArmourType = _armourtype;

	LoadTextureBasedOnItemType(_itemType, _armourtype);

	// Sprite
	m_Shape.setTexture(*m_Texture, true);
	m_Shape.setScale(0.3f, 0.3f);
	m_Shape.setOrigin(m_Shape.getGlobalBounds().width / 2, m_Shape.getGlobalBounds().height / 2);
}

CArmour::~CArmour()
{
	delete m_Texture;

	m_Texture = nullptr;
	m_World = nullptr;
}

void CArmour::LoadTextureBasedOnItemType(CBlock::BLOCKTYPE _itemType, CBlock::ARMOURTYPE _armourType)
{
	if (_itemType == CBlock::BLOCKTYPE::HELMET)
	{
		CheckHelmetTypeAndAssignTexture(_armourType);
	}
	else if (_itemType == CBlock::BLOCKTYPE::CHESTPLATE)
	{
		CheckChestPlateTypeAndAssignTexture(_armourType);
	}
	else if (_itemType == CBlock::BLOCKTYPE::LEGGINGS)
	{
		CheckLeggingTypeAndAssignTexture(_armourType);
	}
}

void CArmour::CheckHelmetTypeAndAssignTexture(CBlock::ARMOURTYPE _armourType)
{
	switch (_armourType)
	{
	case CBlock::ARMOURTYPE::CACTUS:
		m_ArmourHeadLeft.loadFromFile("Images/Armour/CactusHeadLeft.png");
		m_Texture->loadFromFile("Images/Armour/CactusHeadItem.png");
		m_ArmourValue = 5;
		break;
	case CBlock::ARMOURTYPE::IRON:
		m_ArmourHeadLeft.loadFromFile("Images/Armour/IronHeadLeft.png");
		m_Texture->loadFromFile("Images/Armour/IronHeadItem.png");
		m_ArmourValue = 4;
		break;
	case CBlock::ARMOURTYPE::GOLD:
		m_ArmourHeadLeft.loadFromFile("Images/Armour/GoldHeadLeft.png");
		m_Texture->loadFromFile("Images/Armour/GoldHeadItem.png");
		m_ArmourValue = 6;
		break;
	case CBlock::ARMOURTYPE::DIAMOND:
		m_ArmourHeadLeft.loadFromFile("Images/Armour/DiamondHeadLeft.png");
		m_Texture->loadFromFile("Images/Armour/DiamondHeadItem.png");
		m_ArmourValue = 7;
		break;
	case CBlock::ARMOURTYPE::PURPLE:
		m_ArmourHeadLeft.loadFromFile("Images/Armour/PurpleHeadLeft.png");
		m_Texture->loadFromFile("Images/Armour/PurpleHeadItem.png");
		m_ArmourValue = 8;
		break;
	case CBlock::ARMOURTYPE::GOLDEN:
		m_ArmourHeadLeft.loadFromFile("Images/Armour/GoldenHeadLeft.png");
		m_Texture->loadFromFile("Images/Armour/GoldenHeadItem.png");
		m_ArmourValue = 9;
		break;
	default:
		break;
	}
}

void CArmour::CheckChestPlateTypeAndAssignTexture(CBlock::ARMOURTYPE _armourType)
{
	switch (_armourType)
	{
	case CBlock::ARMOURTYPE::CACTUS:
		m_ArmourChestLeft.loadFromFile("Images/Armour/CactusChestPlateLeft.png");
		m_Texture->loadFromFile("Images/Armour/CactusChestPlateItem.png");
		m_ArmourValue = 7;
		break;
	case CBlock::ARMOURTYPE::IRON:
		m_ArmourChestLeft.loadFromFile("Images/Armour/IronChestPlateLeft.png");
		m_Texture->loadFromFile("Images/Armour/IronChestPlateItem.png");
		m_ArmourValue = 6;
		break;
	case CBlock::ARMOURTYPE::GOLD:
		m_ArmourChestLeft.loadFromFile("Images/Armour/GoldChestPlateLeft.png");
		m_Texture->loadFromFile("Images/Armour/GoldChestPlateItem.png");
		m_ArmourValue = 8;
		break;
	case CBlock::ARMOURTYPE::DIAMOND:
		m_ArmourChestLeft.loadFromFile("Images/Armour/DiamondChestPlateLeft.png");
		m_Texture->loadFromFile("Images/Armour/DiamondChestPlateItem.png");
		m_ArmourValue = 9;
		break;
	case CBlock::ARMOURTYPE::PURPLE:
		m_ArmourChestLeft.loadFromFile("Images/Armour/PurpleChestPlateLeft.png");
		m_Texture->loadFromFile("Images/Armour/PurpleChestPlateItem.png");
		m_ArmourValue = 10;
		break;
	case CBlock::ARMOURTYPE::GOLDEN:
		m_ArmourChestLeft.loadFromFile("Images/Armour/GoldenChestPlateLeft.png");
		m_Texture->loadFromFile("Images/Armour/GoldenChestPlateItem.png");
		m_ArmourValue = 11;
		break;
	default:
		break;
	}
}

void CArmour::CheckLeggingTypeAndAssignTexture(CBlock::ARMOURTYPE _armourType)
{
	switch (_armourType)
	{
	case CBlock::ARMOURTYPE::CACTUS:
		m_ArmourLegsLeft.loadFromFile("Images/Armour/CactusLegsLeft.png");
		m_Texture->loadFromFile("Images/Armour/CactusLegsItem.png");
		m_ArmourValue = 3;
		break;
	case CBlock::ARMOURTYPE::IRON:
		m_ArmourLegsLeft.loadFromFile("Images/Armour/IronLegsLeft.png");
		m_Texture->loadFromFile("Images/Armour/IronLegsItem.png");
		m_ArmourValue = 2;
		break;
	case CBlock::ARMOURTYPE::GOLD:
		m_ArmourLegsLeft.loadFromFile("Images/Armour/GoldLegsLeft.png");
		m_Texture->loadFromFile("Images/Armour/GoldLegsItem.png");
		m_ArmourValue = 4;
		break;
	case CBlock::ARMOURTYPE::DIAMOND:
		m_ArmourLegsLeft.loadFromFile("Images/Armour/DiamondLegsLeft.png");
		m_Texture->loadFromFile("Images/Armour/DiamondLegsItem.png");
		m_ArmourValue = 5;
		break;
	case CBlock::ARMOURTYPE::PURPLE:
		m_ArmourLegsLeft.loadFromFile("Images/Armour/PurpleLegsLeft.png");
		m_Texture->loadFromFile("Images/Armour/PurpleLegsItem.png");
		m_ArmourValue = 6;
		break;
	case CBlock::ARMOURTYPE::GOLDEN:
		m_ArmourLegsLeft.loadFromFile("Images/Armour/GoldenLegsLeft.png");
		m_Texture->loadFromFile("Images/Armour/GoldenLegsItem.png");
		m_ArmourValue = 7;
		break;
	default:
		break;
	}
}
