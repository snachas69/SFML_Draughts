#include "Game.h"

Game::Piece::Piece(int Color, int x, int y)
    : Entity(x, y)
{
    pieceSprite = new sf::Sprite;
    pieceTexture = new sf::Texture;
    this->Color = Color;
    isKing = false;
    switch (Color)
    {
    case 7:
        scoreBlack++;
        pieceTexture->loadFromFile("images/Black Piece.png");
        break;
    case 4:
        scoreRed++;
        pieceTexture->loadFromFile("images/Red Piece.png");
        break;
    }
    pieceSprite->setTexture(*pieceTexture);
}

int Game::Piece::IsMovePosible(Cords cords, Entity*** field) const
{
    if (cords.x <= 7 && cords.x >= 0 && cords.y <= 7 && cords.y >= 0)
    {
        int direction = Color == 7 ? 1 : -1;
        if (!field[cords.y][cords.x]->GetSprite())
        {
            if (isKing) return IsKingMovePosible(cords, field);

            else if (abs(cords.x - currnet.x) == 1 && direction == cords.y - currnet.y)
                return 1;

            else if (abs(cords.x - currnet.x) == 2 && abs(cords.y - currnet.y) == 2)
            {
                int x = (cords.x - currnet.x) / abs(cords.x - currnet.x) + currnet.x;
                int y = (cords.y - currnet.y) / abs(cords.y - currnet.y) + currnet.y;

                if (field[y][x]->GetSprite() && field[y][x]->GetColor() != Color)
                    return -1;
            }
        }
    }
    return 0;
}

int Game::Piece::PosiibleMoves(Entity*** field) const
{
    if (isKing)
        return PosiibleKingMoves(field);

    int move = 0; 
    int direction = Color == 7 ? 1 : -1;
    Cords moves[] = {         
        Cords(direction, 1), 
        Cords(direction, -1), 
        Cords(2, 2), 
        Cords(2, -2), 
        Cords(-2, -2),
        Cords(-2, 2)
    };
    for (size_t i = 0; i < 6; i++)
    {
        Cords nextMove(moves[i].x + currnet.x, moves[i].y + currnet.y);
        switch (dynamic_cast<Piece*>(field[currnet.y][currnet.x])->IsMovePosible(nextMove, field))
        {
        case 1:
            move = 1;
            break;
        case -1:
            return -1;
        }
    }
	return move;
}

sf::Sprite* Game::Piece::GetSprite()
{
    return pieceSprite;
}

bool Game::Piece::GetIsKing()
{
    return isKing;
}

void Game::Piece::ToKing()
{
    isKing = true;
    delete pieceSprite;
    delete pieceTexture;
    pieceSprite = new sf::Sprite;
    pieceTexture = new sf::Texture;
    switch (Color)
    {
    case 7:
        pieceTexture->loadFromFile("images/Black King.png");
        break;
    case 4:
        pieceTexture->loadFromFile("images/Red King.png");
        break;
    }
    pieceSprite->setTexture(*pieceTexture);
}

void Game::Piece::Kill()
{
    ReduceScore(Color);
    delete pieceSprite;
    delete pieceTexture;
    pieceSprite = nullptr;
    pieceTexture = nullptr;
    Color = 0;
    isKing = false;
}

int Game::Piece::IsKingMovePosible(Cords cords, Entity*** field) const
{
    int x = currnet.x > cords.x ? -1 : 1; 
    int y = currnet.y > cords.y ? -1 : 1;
    bool piece = false; 
    int check = 0; 
    for (size_t i = currnet.x + x, j = currnet.y + y;; i += x, j += y)
    {
        if (field[j][i]->GetColor() == Color)
            return 0;
        else if (piece && !field[j][i]->GetSprite()) 
        {
            check = -1;
            piece = false;
        }
        else if (field[j][i]->GetColor() != Color && field[j][i]->GetSprite())
        {
            if (!piece)
                piece = true;
            else
                return 0;
        }   
        else if (check != -1 && !field[j][i]->GetSprite()) 
            check = 1;
        if (j == cords.y && i == cords.x)
            break;
    }
    return check;
}

int Game::Piece::PosiibleKingMoves(Entity*** field) const
{
    Cords move[] = {
        Cords(1, -1),
        Cords(1, 1),
        Cords(-1, 1),
        Cords(-1, -1),
    };
    int check = 0;
    for (size_t i = 0; i < 4; i++)
    {
        bool piece = false;
        for (size_t j = currnet.x + move[i].x, k = currnet.y + move[i].y;
            ; j += move[i].x, k += move[i].y)
        {
            if (k > 7 || k < 0 || j > 7 || j < 0) 
                break;
            if (field[k][j]->GetColor() == Color)
                break;
            if (!field[k][j]->GetSprite() && check != -1)
                check = 1;
            if (field[k][j]->GetSprite() && field[k][j]->GetColor() != Color)
            {
                if (!piece) 
                    piece = true;
                else        
                    break;
            }
            if (!field[k][j]->GetSprite() && piece)
                return -1;
        }
    }
    return check;
}
