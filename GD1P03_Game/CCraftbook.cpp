#include "CCraftbook.h"

CCraftbook::CCraftbook(sf::RenderWindow* _renderWindow, sf::Font& _font)
{
}

CCraftbook::~CCraftbook()
{
}

void CCraftbook::Update(sf::Event& _event)
{
}

void CCraftbook::Start()
{
}

void CCraftbook::draw(sf::RenderTarget& _target, sf::RenderStates _states) const
{
    // apply the transform
    _states.transform *= getTransform();

    // our particles don't use a texture
    _states.texture = NULL;

    // draw the vertex array
   // _target.draw(m_Shape, _states);
}
