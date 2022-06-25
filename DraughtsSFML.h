#pragma once
#define NOMINMAX	//SFML ���� ������������ � Windows.h. ��� �������� ����� -- ������� ������� �� �����

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <fstream>
#include <filesystem>
#include <conio.h>
#include <Windows.h>
#include <string>

struct Cords	//���������, ��� �������� ����������, �� ��� ������������, ��� ��������� ����� ����
{
	Cords();
	Cords(int x, int y);
	int x;
	int y;
};

const int A1X = 93;				//���������� ����� � ���������������� ������� �1 �� �� � � �������
const int A1Y = 83;				//���������� ����� � ���������������� ������� �1 �� �� � � �������
const float EACH_SQUARE = 71.7f;//������� �� ������ ������� ���������� �� ���

static struct Menu   
{
	static bool PlayGame(std::string& player);		//��������� ����� ���������, �� ������� �� ��� ������� ������ � ����� ���������� ��� � ����(���� ��� ���� �����������, �� ��������� ����� �� ����������)
	static std::string* LogInSingUp(int check);		//��������� �����, �� ������� �� ����������� �� ���� � ������
	static void Score(const std::string& account);  //��������� �����, �� �������� ������ ����������� �������
	static void MainMenu();							//�����, �� ������ ������ ���� � ����� ����� ����(�������� �� Exit:))
	static std::string time1;						//�������� ����, �� ������� �� ������� ������ ���
	static tm* time2;								//�������� ����, �� ������� �� ����� ���
	static int winner;								//�������� ����, �� ������� �� ��������� � ��� 
};