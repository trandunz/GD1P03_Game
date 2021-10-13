//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CCraftbook.cpp
// Description : CCraftbook Implementation file.
// Author : William Inman
// Mail : william.inman@mds.ac.nz
//

#include "CCraftbook.h"

/// <summary>
/// CCraftbook Contructor
/// </summary>
/// <param name="_renderWindow"></param>
/// <param name="_font"></param>
CCraftbook::CCraftbook(sf::RenderWindow* _renderWindow, sf::Font& _font)
{
}

/// <summary>
/// CCraftbook Destructor
/// </summary>
CCraftbook::~CCraftbook()
{
}

/// <summary>
/// CCraftbook Update
/// </summary>
/// <param name="_event"></param>
void CCraftbook::Update(sf::Event& _event)
{
}

/// <summary>
/// CCraftbook Start
/// </summary>
void CCraftbook::Start()
{
}

/// <summary>
/// Inherited draw function (Note: call renderwindow->draw(*CCraftbook);)
/// </summary>
/// <param name="_target"></param>
/// <param name="_states"></param>
void CCraftbook::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
    // apply the transform
    _states.transform *= getTransform();

    // our particles don't use a texture
    _states.texture = NULL;

    // draw the vertex array
   // _target.draw(m_Shape, _states);
}
