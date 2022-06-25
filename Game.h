#pragma once
#include "DraughtsSFML.h"

class Game													//����� � ����� � �������� ��� ���� ���
{
public:
	Game();													//����������� �����
	~Game();												//����������
	void Draw(sf::RenderWindow& window) const;				//�����, �� ����� ���� � �������� � SFML
	void MakeMove(Cords from, Cords to, int tryMove);		//�����, �� ����� ������������� ��'���� � �����(������ ���) �� �������� �� � �������
	int TryMove(Cords from, Cords to, int scanField);		//�������� �� �������� ����, ���� ������� ��� ������ �������� ������
	bool TryMove(Cords from);								//��������, �� ������� ������ ������ ����� �������
	bool NextMove(Cords cords);								//�������� �� ���� ������ ���������� ���� ��������� ��������
	int ScanField() const;									//�����, �� ����� ���� � ������� ���������(0 - �� ������� �������� ������, 1 - ����� ��������, -1 - �����(���������) ������)
	int GetTurn() const;									//����� ��� ����, �� ������� �� ��, ��� ����� ���
	void SetTurn(int Turn);									//������ ��� ���� � ����
private:
	class Entity											//��������� ����, �� ������� �� ��, �� ���������� � �����(���)
	{
	public:
		Entity(int x, int y);								//�����������
		~Entity();											//����������
		void ReduceScore(int Color);						//�������� �������
		static bool IsDead();								//�������� �� ���� � � ������ ������ ���������� ������
		virtual sf::Sprite* GetSprite();					//³��������� ����� ����� ��� �������, �� ���� ���� �� �� ��������� ���� ���. ������ ��� ������� ����� ��'����
		int GetColor() const;								//�����, �� ������� ����� ������� ��'���(0, ���� �� - ����� �������, 4 - ���� �������� � 7 - ���� ������)
		void SetCurrentCords(Cords cords);					//������ ������ ��������� ��'����
	protected:
		sf::Texture* pieceTexture;							//�������� �� �������� ������� ��'����
		sf::Sprite* pieceSprite;							//�������� �� ��� ������
		Cords currnet;										//������ ����������
		static size_t scoreBlack;							//������� ������
		static size_t scoreRed;								//������� ����
		int Color;											//���� ��'����
	};

	class Piece												//������� Entity(�� ��� �������� �������������, ���� � � ������ ����). ��� ������ ���������� ����������� ���� �������
		: public Entity
	{
	public:
		Piece(int Color, int x, int y);						//�����������
		int IsMovePosible(Cords cords, Entity*** field) const;//�������� �� ��� �������� ��� ������ ��'����
		int PosiibleMoves(Entity*** field) const;			//��������� ���������, �� ���� ���� ������ ��'��� � ������� �������(������� ���� ��� ����� ����, �� �������� ������� ���� ��� ����������, � ���� ������� ������� ������ ����, � �� ������)
		sf::Sprite* GetSprite() override;					//�������������� ���� ����������� ������, �� ��� ����, � ���� ������ �� � ���� �������, ���� ��� ����, ��� ���� ��� ���������� � ���� �� � ���� ���������� �������
		bool GetIsKing();									//�� ��� ��'��� - �����
		void ToKing();										//������� � ��'���� �����
		void Kill();										//Fatality
	protected:
		int IsKingMovePosible(Cords cords, Entity*** field) const;//�������� �� ��� ��������, ��� ��� �����
		int PosiibleKingMoves(Entity*** field) const;		//��������� ��������� ��� ����� ����, ��� ���� ��� ��� �����
		bool isKing;										//����� ��� ��������, �� � ��'�� ������
	};

	int Turn;												//��� ����� ����� ������
	Entity*** field;										//���� � ���'�� � ������ ������
	sf::SoundBuffer soundBuffer;							//����� ��� �����
	sf::Sound sound;										//��� ����
	sf::Texture boardTexture;								//�������� ��� ����
	sf::Sprite boardSprite;									//������ ����
};

