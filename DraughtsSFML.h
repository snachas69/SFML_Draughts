#pragma once
#define NOMINMAX	//SFML може конфліктувати з Windows.h. Щоб уникнути цього -- потрібно зробити цю штуку

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <conio.h>
#include <Windows.h>
#include <string>

struct Cords	//Структура, щоб зберігати координати, та два конструктора, щоб витрачати менше коду
{
	Cords();
	Cords(int x, int y);
	int x;
	int y;
};

const int A1X = 93;				//Константна змінна з місцезнаходженням клітинки А1 по осі х у пікселях
const int A1Y = 83;				//Константна змінна з місцезнаходженням клітинки А1 по осі х у пікселях
const float EACH_SQUARE = 71.7f;//Довжина та ширина кожного квадратика на полі

static struct Menu   
{
	static bool PlayGame(std::string& player);		//Статичний метод структури, що відповідає за сам ігровий процес і запис результатів гри у файл(якщо гра була призупинена, то результат нікуди не записується)
	static std::string* LogInSingUp(int check);		//Статичний метод, що відповідає за регестрацію та вхід в акаунт
	static void Score(const std::string& account);  //Статичний метод, що виводить історію конкретного акаунту
	static void MainMenu();							//Метод, що реалізує методи вище і являє собою меню(натисніть на Exit:))
	static std::string time1;						//Статичне поле, що відповідає за початок ігрової сесії
	static tm* time2;								//Статичне поле, що відповідає за кінець сесії
	static int winner;								//Статичне поле, що відповідає за переможця в сесії 
};