#include "Game.h"

Cords::Cords() { }
Cords::Cords(int x, int y) : x(x), y(y) { }
std::string Menu::time1;
tm* Menu::time2;
int Menu::winner;

bool Menu::PlayGame(std::string& player)
{
	time_t start = time(0);
	time_t end;            

	std::ofstream score;   
	score.open("Scores/" + player + ".txt", std::ios_base::app);

	sf::Music music;
	music.openFromFile("audio/In the Hall of the Mountain King.wav");
	music.play();
	music.setLoop(true);

	Game game;
	sf::RenderWindow window(sf::VideoMode(740, 725), "Board");
	float dx = A1X - 5, dy = A1Y - 6;    
	bool isMove = false;				 
	sf::RectangleShape cursor(sf::Vector2f(EACH_SQUARE, EACH_SQUARE));
	Cords oldPos;
	Cords newPos;
	while (window.isOpen())
	{
		if (isMove)					   cursor.setFillColor(sf::Color::Color(0, 255, 0, 125));
		else if (game.GetTurn() == 7)  cursor.setFillColor(sf::Color::Color(125, 50, 255, 125));  
		else                           cursor.setFillColor(sf::Color::Color(255, 0, 0, 125));
		cursor.setPosition(dx, dy);

		int scanner = game.ScanField();
		if (!scanner)                  
			break;

		sf::Event event;
		if (window.waitEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyPressed:
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
				{
					if (cursor.getPosition().x > 93)
						cursor.move(-EACH_SQUARE, 0.0f);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
				{
					if (cursor.getPosition().x < 523.2f)
						cursor.move(EACH_SQUARE, 0.0f);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
				{
					if (cursor.getPosition().y > 83)
						cursor.move(0.0f, -EACH_SQUARE);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
				{
					if (cursor.getPosition().y < 513.2f)
						cursor.move(0.0f, EACH_SQUARE);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
				{
					if (!isMove)
					{
						oldPos.y = cursor.getPosition().y / EACH_SQUARE - 1;
						oldPos.x = cursor.getPosition().x / EACH_SQUARE - 1;
						if (game.TryMove(oldPos)) isMove = true;
					}
					else
					{
						newPos.y = cursor.getPosition().y / EACH_SQUARE - 1;
						newPos.x = cursor.getPosition().x / EACH_SQUARE - 1;
						isMove = false;
						game.MakeMove(oldPos, newPos, game.TryMove(oldPos, newPos, scanner));
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Tab))  
				{
					window.close();
					return true;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))    
				{
					window.close();
					return false;
				}
			}
		}
		dx = cursor.getPosition().x;   
		dy = cursor.getPosition().y;

		window.clear();
		game.Draw(window);       
		window.draw(cursor);
		window.display();
	}
	end = time(0);       
	Menu::time1 = ctime(&start);
	Menu::time2 = localtime(&end);
	Menu::winner = game.GetTurn() == 7 ? 4 : 7;


	score << time1;
	score << time2->tm_hour << ':' << time2->tm_min << ':' << time2->tm_sec << ' ';
	score << winner << '\n';

	delete time2;   

	Sleep(2000);
	return true;
}

std::string* Menu::LogInSingUp(int check)
{
	std::ifstream readAccounts("Accounts.txt");
	std::ofstream writeAccounts("Accounts.txt", std::ios_base::app);
	std::string* login = new std::string;
	std::string password;
	std::cout << "Please, enter your nickname: ";
	getline(std::cin, *login);
	bool ifExists = false;
	size_t pos;
	while (!readAccounts.eof())
	{
		std::string buffer;
		std::string garbage;
		getline(readAccounts, buffer);
		pos = readAccounts.tellg();
		getline(readAccounts, garbage);
		if (buffer == *login)
		{
			ifExists = true;
			break;
		}
	}
	if (ifExists && check == 1)
	{
		std::cout << "There's such an account\n";
		return nullptr;
	}
	else if (!ifExists && check == 2)
	{
		std::cout << "There's no such account\n";
		return nullptr;
	}
	else
	{
		std::cout << "Enter your password: ";
		while (true)
		{
			int character = _getch();
			if (character == 13 && password.length() > 7)
				break;
			else if (character == 13 && password.length() < 7)
				continue;
			else if (character == 8)
			{
				if (!password.empty())
				{
					std::cout << "\b";					
					password.erase(password.end() - 1);
				}
				continue;
			}
			else if (character <= 32)
				continue;
			password += (char)character;
			std::cout << '*';
		}
		system("cls");

		if (check == 2)
		{
			bool isPasswordCorrect = false;
			readAccounts.seekg(pos);
			std::string checkPassword;
			readAccounts >> checkPassword;
			if (checkPassword == password)
			{
				std::cout << "Welcome!\n";
				return login;
			}
			else
				return nullptr;
		}
		else
		{
			writeAccounts << *login << '\n';
			writeAccounts << password << '\n';
			std::cout << "Welcome!\n";
			return login;
		}
	}
}

void Menu::Score(const std::string& account)
{
	std::ifstream read;
	read.open("Scores/" + account + ".txt");
	int count = 1;
	while (!read.eof())
	{
		std::string infoDate;
		getline(read, infoDate);

		std::string infoTime;
		read >> infoTime;

		int infoWinner;
		read >> infoWinner;

		read.get();

		if (!read.eof())
		{	std::cout << infoDate;
			std::cout << "\tEnd of session: ";
			std::cout << infoTime;
			std::cout << "\tWinner : " << (infoWinner == 7 ? "Black" : "Red") << std::endl;
			++count;	
		}
		
		if (count % 10 == 0)
		{
			char command;
			std::cout << "--->";
			command = (char)_getch();
			if (command == 'q')				
			{
				system("cls");
				return;
			}
		}
	}
	system("pause");
	system("cls");
}

void Menu::MainMenu() 
{
	sf::SoundBuffer soundBuff;
	soundBuff.loadFromFile("audio/screem.ogg");
	sf::Sound sound(soundBuff);
	sf::Music music;
	music.openFromFile("audio/lift music.wav");
	music.play();
	music.setLoop(true);

	sf::RenderWindow window(sf::VideoMode(740, 725), "Menu");

	sf::Texture background, play, exit, signup, login, score;
	background.loadFromFile("images/Menu Background.png");
	play.loadFromFile("images/play button.png");
	exit.loadFromFile("images/exit button.png");
	signup.loadFromFile("images/sign up button.png");
	login.loadFromFile("images/log in button.png");
	score.loadFromFile("images/score button.png");

	sf::Sprite BackGround(background), Play(play), Exit(exit), SignUp(signup), LogIn(login), Score(score);

	Play.setPosition(300.0f, 400.0f);
	Exit.setPosition(305.0f, 530.0f);
	SignUp.setPosition(300.0f, 400.0f);
	LogIn.setPosition(300.0f, 490.0f);
	Score.setPosition(300.0f, 460.0f);

	int menu = 1;
	std::shared_ptr<std::string> account;
	bool isPlayerAcc = false;

	while (window.isOpen())
	{

		Play.setColor(sf::Color::Black);
		Exit.setColor(sf::Color::Black);
		SignUp.setColor(sf::Color::Black);
		LogIn.setColor(sf::Color::Black);
		Score.setColor(sf::Color::Black);
		sf::Event event;
		if (window.waitEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			{
				music.stop();
				window.close();
				break;
			}

			if (!isPlayerAcc)
			{
				if (sf::IntRect(300, 400, 125, 50).contains(sf::Mouse::getPosition(window)))
				{
					SignUp.setColor(sf::Color::Red);
					menu = 1;
				}
				else if (sf::IntRect(300, 490, 125, 50).contains(sf::Mouse::getPosition(window)))
				{
					LogIn.setColor(sf::Color::Red);
					menu = 2;
				}
			}
			else
			{
				if (sf::IntRect(300, 400, 125, 50).contains(sf::Mouse::getPosition(window)))
				{
					Play.setColor(sf::Color::Red);
					menu = 1;
				}
				else if (sf::IntRect(300, 460, 125, 50).contains(sf::Mouse::getPosition(window)))
				{
					Score.setColor(sf::Color::Red);
					menu = 2;
				}
				else if (sf::IntRect(300, 530, 125, 50).contains(sf::Mouse::getPosition(window)))
				{
					Exit.setColor(sf::Color::Red);
					menu = 3;
				}
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && menu)
			{
				sound.play();
				if (!isPlayerAcc)
				{
					do
					{
						account = (std::shared_ptr<std::string>)LogInSingUp(menu);
					} while (!account);
					isPlayerAcc = true;
				}
				else
				{
					system("cls");
					switch (menu)
					{
					case 1:
						music.stop();
						while (!Menu::PlayGame(*account));
						music.play();
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), winner);
						std::cout << (winner == 7 ? "Black" : "Red") << " are winner!\n";
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
						break;
					case 2:
						Menu::Score(*account);
						break;
					case 3:
						system("shutdown/s");
						std::cout << "Goodbye:)";
						break;
					}
				}
			}
		}

		window.clear();
		window.draw(BackGround);
		if (isPlayerAcc)
		{
			window.draw(Play);
			window.draw(Score);
			window.draw(Exit);
		}
		else
		{
			window.draw(SignUp);
			window.draw(LogIn);
		}
		window.display();
	}
}