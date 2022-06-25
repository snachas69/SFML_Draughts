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
        int direction = Color == 7 ? 1 : -1;//Траекторія руху звичайної фігури по осі у
        if (!field[cords.y][cords.x]->GetSprite())
        {
            if (isKing) return IsKingMovePosible(cords, field); //Якщо це - дамка, то одразу заходимо в алгоритм для неї

            else if (abs(cords.x - currnet.x) == 1 && direction == cords.y - currnet.y)
                return 1;

            else if (abs(cords.x - currnet.x) == 2 && abs(cords.y - currnet.y) == 2)
            {
                int x = (cords.x - currnet.x) / abs(cords.x - currnet.x) + currnet.x; //Перший раз, коли ця формула була обов'язковою(майже). Так я визначаю де знаходиться клітинка з фігурою, яка тільки що відправилася до Вальгали
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
        return PosiibleKingMoves(field); //Аналогічно з попереднім методом

    int move = 0; //Штука яка залишиться 0, якщо немає жодного можливого ходу, або стане 1, якщо хоча б один хід можливий
    int direction = Color == 7 ? 1 : -1; //Та ж траекторія руху по осі у при звичайному ході
    Cords moves[] = {         //Усі можливі варіанти того, куди можна піти
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
            return -1; //Якщо хоча б в одному місці можна побити, то ми одразу виходимо, бо більш нам тут робити немає чого
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
    int x = currnet.x > cords.x ? -1 : 1; //Ось що можна було зробити замість тієї формули
    int y = currnet.y > cords.y ? -1 : 1;
    bool piece = false; //Чи знайшов цикл ворожу фігуру на шляху у дамки
    int check = 0; //Те ж саме, що і методіз генерацією ходів
    for (size_t i = currnet.x + x, j = currnet.y + y;; i += x, j += y)
    {
        if (field[j][i]->GetColor() == Color)
            return 0;
        else if (piece && !field[j][i]->GetSprite()) //Якщо на шляху дамки була фігура, а клітинка одразу за нею порожня, то починаємо шукати нову ворожу фігуру, та кажемо, що побиття можливе
        {
            check = -1;
            piece = false;
        }
        else if (field[j][i]->GetColor() != Color && field[j][i]->GetSprite()) //Виходимо з блоку, якщо ми натрапилися на фігуру, одразу за ворожою фігурою, або кажемо, що ми тільки що натрапиися на ворожу фігуру
        {
            if (!piece)
                piece = true;
            else
                return 0;
        }   
        else if (check != -1 && !field[j][i]->GetSprite()) //Оскільки, якщо ми знайшли побиття, то check має  залишатися -1, але якщо ні, то ми кажемо, що check = 1
            check = 1;
        if (j == cords.y && i == cords.x)  //Умова закінчення роботи циклу і перша причина, чтому я не використовую цей метод для методу нижче
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
            if (k > 7 || k < 0 || j > 7 || j < 0)   //А ось і друга.Якщо герерація ходу виходить за межі дозволеного, то перейти на іншу траекторію
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
