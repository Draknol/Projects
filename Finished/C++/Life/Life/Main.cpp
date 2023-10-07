// Game of life using sfml
// optimised for large grids
// uses random seed for start

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <thread>

using namespace std;
using namespace sf;

int threadCount;


const int resolution = 1000;
bool Map1[resolution + 2][resolution + 2];
bool Map2[resolution + 2][resolution + 2];

RenderWindow window(VideoMode(resolution, resolution), "Life", Style::Close | Style::Titlebar);

VertexArray pixels(Points);

void calculate(bool (& currentMap)[resolution + 2][resolution + 2], bool (& otherMap)[resolution + 2][resolution + 2]);

int main()
{

	//random starting position

	mt19937 rng(std::random_device{}());
	uniform_real_distribution<float> dist(0.0f, 1.0f);

	for (int x = 1; x <= resolution; x++)
	{
		for (int y = 1; y <= resolution; y++)
		{
			Map1[x][y] = dist(rng) < 0.5;
		}
	}

	Clock time;

	bool isMap1 = true;

	while (window.isOpen())
	{

		cout << 1 / time.restart().asSeconds() << endl;

		Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			default:
				break;
			}
		}


		if (isMap1) calculate(Map1, Map2);
		else calculate(Map2, Map1);

		isMap1 = !isMap1;
	}

}

//used so i can swap which map is being used each frame

void calculate(bool(&currentMap)[resolution + 2][resolution + 2], bool(&otherMap)[resolution + 2][resolution + 2])
{

	for (int x = 1; x <= resolution; x++)
	{
		for (int y = 1; y <= resolution; y++)
		{
			//works out how many neibours the pixel has

			int count = 0;

			if (currentMap[x - 1][y - 1]) count++;
			if (currentMap[x - 1][y]) count++;
			if (currentMap[x - 1][y + 1]) count++;
			if (currentMap[x][y + 1]) count++;
			if (currentMap[x][y - 1]) count++;
			if (currentMap[x + 1][y - 1]) count++;
			if (currentMap[x + 1][y]) count++;
			if (currentMap[x + 1][y + 1]) count++;

			//updates next frame and adds pixel to vertex
			

			if (currentMap[x][y])
			{
				if (count != 2 && count != 3)
				{
						otherMap[x][y] = 0;
				}
				else
				{
					otherMap[x][y] = 1;
					Vertex vertex(Vector2f(x, y));
					pixels.append(vertex);
				}
			}
			else
			{
				if (count == 3)
				{
					otherMap[x][y] = 1;
					Vertex vertex(Vector2f(x, y), Color::White);
					pixels.append(vertex);
				}
				else otherMap[x][y] = 0;
			}
		}
	}

	//draws all pixles

	window.clear();
	window.draw(pixels);
	window.display();
	pixels.clear();
}
