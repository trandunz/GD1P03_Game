//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CInputField.cpp
// Description : CInputField Implementation file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

// Local Includes
#include "CInputField.h"

/// <summary>
/// InputField Constructor.
/// Takes in:	sf::RenderWindow* (Intended To Be The PopOutMenu Render Window).
/// </summary>
CInputField::CInputField(sf::RenderWindow* _renderWindow)
{
    // Load Textures And Fonts
    m_IdleTexture.loadFromFile("Images/ButtonHovering.png");
    m_HoverTexture.loadFromFile("Images/ButtonHovering.png");
    m_Font.loadFromFile("Fonts/times.TTF");

    // Set Up Font
    m_tLabel.setFont(m_Font);
    m_tLabel.setCharacterSize(24);
    m_tLabel.setFillColor(sf::Color::Black);

    // Set InputField Texture To Idle
    Sprite.setTexture(m_IdleTexture);

    // Set Origin's
    m_tLabel.setOrigin(sf::Vector2f(m_tLabel.getGlobalBounds().width / 2, m_tLabel.getGlobalBounds().height / 2));
    Sprite.setOrigin(sf::Vector2f(Sprite.getGlobalBounds().width / 2, Sprite.getGlobalBounds().height / 2));

    // Set Label
    m_Label = "Input Value";
    SetLabel(m_Label);

    // Initialization
    m_RenderWindow = _renderWindow;
    m_bIsHovering = false;
    m_bIsPressed = false;
    m_OnClickLabel = m_Label;
    m_HoverLabel = m_Label;
}

/// <summary>
/// InputField Destructor
/// </summary>
CInputField::~CInputField()
{
    // Cleanup Pointer
    m_RenderWindow = nullptr;
}

/// <summary>
/// Update Definition For InputField.
/// </summary>
void CInputField::Update()
{
    // Set m_vMousePosition To Mouse Position
    sf::Vector2i pixelPos = sf::Mouse::getPosition((*m_RenderWindow));

    SetMousePosition(m_RenderWindow->mapPixelToCoords(pixelPos));

    // Checks If Mouse Is On InputField
    bIsinBounds(m_vMousePosition);
}

/// <summary>
/// Render Definition For InputField.
/// </summary>
void CInputField::Render()
{
    m_RenderWindow->draw(Sprite);
    m_RenderWindow->draw(m_tLabel);
}

/// <summary>
/// Assigns sf::Vector2f(Position) to m_vMousePosition 
/// </summary>
/// <param name="Position"></param>
void CInputField::SetMousePosition(sf::Vector2f _position)
{
    m_vMousePosition = _position;
}

/// <summary>
/// Returns m_vMousePosition (sf::Vector2f)
/// </summary>
/// <returns></returns>
sf::Vector2f CInputField::GetMousePosition()
{
    return m_vMousePosition;
}

/// <summary>
/// Checks If The sf::Vector2f (_vMousePosition) Is Inside Of The Input Field GlobalBounds.
/// Takes In: sf::Vector2f (_vMousePosition)
/// </summary>
/// <param name="_vMousePosition"></param>
/// <returns></returns>
bool CInputField::bIsinBounds(sf::Vector2f _vMousePosition)
{
    // If Mouse Is In Sprite
    if (Sprite.getGlobalBounds().contains(sf::Vector2f(_vMousePosition.x, _vMousePosition.y)))
    {
        m_bIsHovering = true;

        // Checks For Pressed
        if (m_bIsPressed == false) 
        { 
            Sprite.setTexture(m_HoverTexture);
            SetState("Hover");
        }

        // Label Is Default Check
        if (m_Label == "Input Value")
        {
            SetLabel("");
        }
        else
        {
            SetLabel(m_Label);
        }

        /*std::cout << "MousedOverButton" << std::endl;*/

        return true;
    }
    // Mouse Is Not In Sprite
    else
    {
        m_bIsHovering = false;

        // Check For Pressed
        if (m_bIsPressed == false) 
        {
            Sprite.setTexture(m_IdleTexture);
            SetLabel(m_Label);
        }

        // Set State 
        SetState("Idle");
        
        // Label Is Default Check
        if (m_Label == "")
        {
            SetLabel("Input Value");
        }
        else
        {
            SetLabel(m_Label);
        }

        return false;
    }
}

/// <summary>
/// Sets The Label Of The InputField To Specified Parameter.
/// Takes In: std::string (_label)
/// </summary>
/// <param name="_newLabel"></param>
void CInputField::SetLabel(std::string _label)
{
    // Temp Creation
    std::string oldLabel = m_Label;
    std::string oldClickLabel = m_OnClickLabel;

    // Checks
    if (_label != m_OnClickLabel && _label != m_HoverLabel) 
    {
        m_Label = _label;
    }
    if (m_OnClickLabel == oldLabel && _label != m_HoverLabel)
    {
        m_OnClickLabel = _label;
    }
    if (m_HoverLabel == oldLabel && _label != oldClickLabel) 
    {
        m_HoverLabel = _label;
    }

    // Update Label
    m_tLabel.setString(_label);
    m_tLabel.setCharacterSize(20);

    // Set Origin
    m_tLabel.setOrigin(sf::Vector2f(m_tLabel.getGlobalBounds().width / 2, m_tLabel.getGlobalBounds().height / 2));
}

/// <summary>
/// Sets The State Of The InputField To Sepecified Parameter.
/// Takes In: std::string (type).
/// </summary>
/// <param name="type"></param>
void CInputField::SetState(std::string _type)
{
    if (_type == "Idle")
    {
        Sprite.setTexture(m_IdleTexture);
    }
    else if (_type == "Hover")
    {
        Sprite.setTexture(m_HoverTexture);
    }
}

/// <summary>
/// Set The Position Of The InputField To float (newX), float (newY).
/// </summary>
/// <param name="newX"></param>
/// <param name="newY"></param>
void CInputField::SetPosition(float _newX, float _newY)
{
    Sprite.setPosition(sf::Vector2f(_newX, _newY));
    m_tLabel.setPosition(sf::Vector2f(_newX, _newY));
}

/// <summary>
/// Removes The Last Character From The Input String.
/// Takes In: std::string (string to change)
/// </summary>
/// <param name="_string"></param>
/// <returns></returns>
std::string CInputField::removeLastChar(std::string _string)
{
    // String Is Empty Check
    if (_string == "" || _string.length() == 0)
    {
        return _string;
    }

    // Return String Minus Last Character
    return _string.std::string::substr(0, _string.length() - 1);
}

