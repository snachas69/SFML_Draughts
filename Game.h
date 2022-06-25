#pragma once
#include "DraughtsSFML.h"

class Game													//Класс з полем і методами для самої гри
{
public:
	Game();													//Конструктор класу
	~Game();												//Деструктор
	void Draw(sf::RenderWindow& window) const;				//Метод, що малює поле з фігурами в SFML
	void MakeMove(Cords from, Cords to, int tryMove);		//Метод, що змінює місцеположення об'єктів у масиві(робить хід) та відображає їх в консоль
	int TryMove(Cords from, Cords to, int scanField);		//Перевірка на валідність ходу, коли гравець уже вибрав підходящу фігуру
	bool TryMove(Cords from);								//Перевірка, чи гравець вибрав фігуру свого кольору
	bool NextMove(Cords cords);								//Перевірка чи може фігура продовжити серію шашкового геноциду
	int ScanField() const;									//Метод, що сканує поле і повертає результат(0 - не можливо походити взагалі, 1 - можна походити, -1 - можна(необхідно) побити)
	int GetTurn() const;									//Гетер для поля, що відповідає за те, чий зараз хід
	void SetTurn(int Turn);									//Сеттер для того ж поля
private:
	class Entity											//Вкладений клас, що відповідає за те, що зберігається у масиві(полі)
	{
	public:
		Entity(int x, int y);								//Конструктор
		~Entity();											//Деструктор
		void ReduceScore(int Color);						//Зменшити рахунок
		static bool IsDead();								//Перевірка чи хоча б у одного гравця закінчилися фігури
		virtual sf::Sprite* GetSprite();					//Віртуальний метод чисто для галочки, на його місці міг би опинитися будь хто. Геттер для спрайта цього об'єкту
		int GetColor() const;								//Гетер, що відповідає якого кольору об'єкт(0, якщо це - пуста клітинка, 4 - якщо червоний і 7 - якщо чорний)
		void SetCurrentCords(Cords cords);					//Змінити поточні кординати об'єкту
	protected:
		sf::Texture* pieceTexture;							//Покажчик на текстуру спрайту об'єкта
		sf::Sprite* pieceSprite;							//Покажчик на сам спрайт
		Cords currnet;										//Поточні координати
		static size_t scoreBlack;							//Рахунок чорних
		static size_t scoreRed;								//Рахунок білих
		int Color;											//Колір об'єкту
	};

	class Piece												//Потомок Entity(ми тут знаннями випендрюємося, тому я й зробив його). Цей містить функціонал притаманний лише фігурам
		: public Entity
	{
	public:
		Piece(int Color, int x, int y);						//Конструктор
		int IsMovePosible(Cords cords, Entity*** field) const;//Перевірка чи хід можливий для даного об'єкту
		int PosiibleMoves(Entity*** field) const;			//Генерація траекторій, по яким може ходити об'єкт у данному випадку(потрібен лише для скану поля, що програма розуміла коли гра закінчилася, а коли потрібно змусити гравця бити, а не ходити)
		sf::Sprite* GetSprite() override;					//Перевизначення того віртуального методу, що був вище, і який виконує ту ж саму функцію, лише для того, щоб клас був поліморфним і тому що у мене закінчилася фантазія
		bool GetIsKing();									//Чи цей об'єкт - дамка
		void ToKing();										//Зробити з об'єкту дамку
		void Kill();										//Fatality
	protected:
		int IsKingMovePosible(Cords cords, Entity*** field) const;//Перевірка чи хід можливий, але для дамки
		int PosiibleKingMoves(Entity*** field) const;		//Генерація траекторій для скану поля, але лише для для дамки
		bool isKing;										//Прапір для перевірки, чи є об'єк дамкою
	};

	int Turn;												//Чия зараз черга ходити
	Entity*** field;										//Поле у пам'яті у вигляді масиву
	sf::SoundBuffer soundBuffer;							//Буфер для звуку
	sf::Sound sound;										//Сам звук
	sf::Texture boardTexture;								//Тектурка для поля
	sf::Sprite boardSprite;									//Спрайт поля
};

