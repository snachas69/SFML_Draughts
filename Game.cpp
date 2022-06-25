#include "Game.h"

Game::Game()
{
	Turn = 4;
	field = new Entity * *[8];
	for (size_t i = 0; i < 8; i++)
	{
		field[i] = new Entity * [8];
		for (size_t j = 0; j < 8; j++)
		{
			if (i < 3 || i > 4)
				if ((i % 2 == 0 && j % 2 == 0) || (i % 2 != 0 && j % 2 != 0)) 
				{
					field[i][j] = new Piece(i < 3 ? 7 : 4, j, i);
					continue;
				}
			field[i][j] = new Entity(j, i);
		}
	}
	soundBuffer.loadFromFile("audio/sound.ogg");
	sound.setBuffer(soundBuffer);
	boardTexture.loadFromFile("images/Game Board.png");
	boardSprite.setTexture(boardTexture);
}

Game::~Game()
{
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 8; j++)
			delete field[i][j];
		delete[] field[i];
	}
	delete[] field;
}

void Game::Draw(sf::RenderWindow& window) const
{
	window.draw(boardSprite);
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			if (field[i][j]->GetSprite())
			{
				field[i][j]->GetSprite()->setPosition(A1X + (EACH_SQUARE * j), A1Y + (EACH_SQUARE * i));
				window.draw(*(field[i][j]->GetSprite()));
			}
		}
	}
}

void Game::MakeMove(Cords from, Cords to, int tryMove)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), field[from.y][from.x]->GetColor()); 
	switch (tryMove)
	{
	case 0:
		return;
	case -1:
	{

		int x = (to.x - from.x) / abs(to.x - from.x); 
		int y = (to.y - from.y) / abs(to.y - from.y);
		
		for (size_t i = from.x + x, j = from.y + y; 
			i != to.x && j != to.y; 
			i += x, j += y)
		{
			if (field[j][i]->GetSprite())
				dynamic_cast<Piece*>(field[j][i])->Kill();
		}
		break;
	}
	}

	field[from.y][from.x]->SetCurrentCords(to);
	std::swap(field[from.y][from.x], field[to.y][to.x]);
	std::cout << char(from.y + 97) << from.x + 1 << char(to.y + 97) << to.x + 1 << '\n'; 
	sound.play();

	int edge = field[to.y][to.x]->GetColor() == 7 ? 7 : 0;  

	if (!dynamic_cast<Piece*>(field[to.y][to.x])->GetIsKing())
		if (to.y == edge)
			dynamic_cast<Piece*>(field[to.y][to.x])->ToKing();

	if (!NextMove(to) && tryMove == -1|| tryMove == 1)
		SetTurn(GetTurn() == 7 ? 4 : 7);
}

int Game::TryMove(Cords from, const Cords to, int scanField)
{
	int tryMove = dynamic_cast<Piece*>(field[from.y][from.x])->IsMovePosible(to, field);
	if (tryMove == scanField)
		return tryMove;
	return 0;
}

bool Game::TryMove(Cords from)
{
	return field[from.y][from.x]->GetColor() == Turn;
}

bool Game::NextMove(Cords cords)
{
	return dynamic_cast<Piece*>(field[cords.y][cords.x])->PosiibleMoves(field) == -1;
}

int Game::ScanField() const
{
	if (Entity::IsDead())
		return 0;
	int scan = 0;
	for (size_t i = 0; i < 8; i++)
		for (size_t j = 0; j < 8; j++)
			if (field[i][j]->GetColor() == Turn)
			{
				switch (dynamic_cast<Piece*>(field[i][j])->PosiibleMoves(field))
				{
				case 1:
					scan = 1;
					break;
				case -1:
					return -1;
				}
			}
	return scan;
}

int Game::GetTurn() const
{
	return Turn;
}

void Game::SetTurn(int Turn)
{
	this->Turn = Turn;
}
