#include "Game.h"

size_t Game::Entity::scoreBlack; //Статичні поля ми можемо об'явити і проініціалізувати в глобальному просторі. Глобальний простір їх обнуляє, тому не обов'язково їм дописувати " = 0"
size_t Game::Entity::scoreRed;

Game::Entity::Entity(int x, int y)
{
    pieceSprite = nullptr;
    pieceTexture = nullptr;
    currnet.x = x;
    currnet.y = y;
    Color = 0;
}

Game::Entity::~Entity()
{
    delete pieceSprite;
    delete pieceTexture;
}

void Game::Entity::ReduceScore(int Color)
{
    Color == 7 ? scoreRed-- : scoreBlack--;
}

bool Game::Entity::IsDead()
{
    return !(scoreBlack && scoreRed);
}

sf::Sprite* Game::Entity::GetSprite()
{
    return pieceSprite;
}

int Game::Entity::GetColor() const
{
    return Color;
}

void Game::Entity::SetCurrentCords(Cords cords)
{
    currnet.x = cords.x;
    currnet.y = cords.y;
}
