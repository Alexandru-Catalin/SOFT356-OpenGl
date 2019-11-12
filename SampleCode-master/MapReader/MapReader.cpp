//////////////////////////////////////////////////////////////////////////////
//
//  MapReader.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include<fstream>
#include<string>
#include<iostream>
#include<vector>
#include<conio.h>

using namespace std;

void fill_map(std::vector<string>& map, int& x, int& y)
{
	string line;
	ifstream myFile("media/map.txt");

	if (myFile.is_open())
	{
		x = 0;
		y = 0;
		map.clear();

		while (getline(myFile, line))
		{
			std::cout << line << std::endl;
			std::cout << "X " << x << " y " << y << std::endl;
			map.push_back(line);
			for (int j = 0; j < line.length(); j++)
			{
				if (map[y][j] == 'S') {} //player found
				std::cout << map[y].at(j);
				x++;
			}
			std::cout << "\n";
			y++;
		}
		x = x / y;
	}
}

int main() {
	int x = 10;
	int y = 5;
	vector<string> map(y);



	for (int i = 0; i < y; i++)
	{

		map[i] = "";
		for (int j = 0; j < x; j++)
		{
			map[i].append(".");
			std::cout << map[i][j];
		}
		std::cout << std::endl;
	}
	std::cout << "X " << x << " y " << y << std::endl;
	fill_map(map, x, y);
	std::cout << "\n checking?\n" << "X " << x << " y " << y << endl;
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			std::cout << map[i][j];
		}
		std::cout << std::endl;
	}

	char key = ' ';
	while (key != 'q')
	{
		cout << "Enter a key" << endl;
		key = _getch();
		cout << "You entered: " << key << endl;
	}

}