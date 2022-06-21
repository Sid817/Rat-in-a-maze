#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <stack>
#include <windows.h>

int a[10][10];
int copy[10][10];
int n;
void put_a_wall(sf::RectangleShape tiles[10][10], int i, int j);
void create_maze(sf::RenderWindow* window);
bool find_path(sf::RenderWindow* window);
sf::RectangleShape rat(sf::Vector2f(100.f, 100.f));

struct position
{
	int row;
	int col;
	//	int option; //direction to look for
};
position final_path[100];
int index;
//structure to keep track of row and column of current posiiton
sf::RectangleShape tiles[10][10];

void put_a_wall(sf::RectangleShape tiles[10][10], int i, int j)
{
	tiles[i][j] = sf::RectangleShape(sf::Vector2f(100.f, 100.f));
	tiles[i][j].setPosition(100 * (j - 1) + 200, 100 * (i - 1) + 200);
	tiles[i][j].setFillColor(sf::Color::Blue);
	a[i][j] = 1;
}
void create_maze(sf::RenderWindow* window)
{
	float x_cor = 200;
	float y_cor = 200;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			tiles[i][j] = sf::RectangleShape(sf::Vector2f(100.f, 100.f));
			if (copy[i][j] == 1)
			{
				tiles[i][j].setFillColor(sf::Color::Magenta);
			}
			else if (copy[i][j] == 0)
			{
				tiles[i][j].setFillColor(sf::Color::Transparent);
			}
			else
			{
				tiles[i][j].setFillColor(sf::Color::Yellow);
			}
			tiles[i][j].setOutlineThickness(3);
			tiles[i][j].setOutlineColor(sf::Color::Blue);

			tiles[i][j].setPosition(x_cor, y_cor);
			x_cor = x_cor + 100;
			window->draw(tiles[i][j]);
		}

		y_cor = y_cor + 100;
		x_cor = 200;
	}
	//creating  a wall around the maze
	for (int i = 0; i <= n + 1; i++)
	{
		//					tiles[i][]0=sf::RectangleShape(sf::Vector2f(100.f,100.f));
		//					tiles[i][0].setPosition(100*(0-1)+200,100*(i-1)+200)
		//					tiles[i][0].setFillColor(sf::Color::Blue);
		put_a_wall(tiles, i, 0);
		window->draw(tiles[i][0]);
		put_a_wall(tiles, 0, i);
		window->draw(tiles[0][i]);
		put_a_wall(tiles, i, n + 1);
		window->draw(tiles[i][n + 1]);
		put_a_wall(tiles, n + 1, i);
		window->draw(tiles[n + 1][i]);

		copy[i][0] = copy[0][i] = -1;
		copy[i][n + 1] = copy[n + 1][i] = -1;
	}
}

bool find_path(sf::RenderWindow* window)
{
	for (int i = 0; i <= n + 1; i++)
	{
		for (int j = 0; j <= n + 1; j++)
		{
			a[i][j] = copy[i][j];
		}
	}

	std::stack<position> path; //path is the stack that will store all the locations we've been to.
	position here, exit;
	here.row = 1;
	here.col = 1;
	exit.row = n;
	exit.col = n;
	//	rat.setPosition((float)(100*(here.col-1)+200),(float)(100*(here.row-1)+200));
	//	window.draw(rat);

	//creating the move table
	position offset[8];
	offset[0].row = 0;
	offset[0].col = 1;
	offset[1].row = 1;
	offset[1].col = 1;
	offset[2].row = 1;
	offset[2].col = 0;
	offset[3].row = 1;
	offset[3].col = -1;
	offset[4].row = 0;
	offset[4].col = -1;
	offset[5].row = -1;
	offset[5].col = -1;
	offset[6].row = -1;
	offset[6].col = 0;
	offset[7].row = -1;
	offset[7].col = 1;
	a[here.row][here.col] = 1;
	//main loop
	while (!(here.row == exit.row && here.col == exit.col))
	{
		int r, c;
		int option;
		for (option = 0; option <= 7; option++)
		{
			r = here.row + offset[option].row;
			c = here.col + offset[option].col;
			if (a[r][c] == 0)
			{
				//				path->Add(here);
				a[r][c] = 1;
				path.push(here);
				tiles[r][c].setFillColor(sf::Color::White);
				window->draw(tiles[r][c]);
				tiles[here.row][here.col].setFillColor(sf::Color::Transparent);
				window->draw(tiles[here.row][here.col]);
				//				rat.setPoS
				//				rat.move(100*(c-1)-100*(here.col-1),100*(r-1)-100*(here.row-1));
				//				window.draw(rat);
				window->display();
				sf::sleep(sf::seconds(0.3));
				window->clear();
				create_maze(window);
				here.row = r;
				here.col = c;
				break;
			}
		}
		//		a[here.row][here.col]=1;   //this will prevent us from coming to this location while looking for potential neighbours again

		//if there is no neighbour to move, we pop the previous location from the stack

		if (option > 7)
		{
			if (path.empty())
			{
				window->close();
				return false;
			}
			else
			{

				//				rat.move(100*(path.top().col-1)-100*(here.col-1),100*(path.top().row-1)-100*(here.row-1));
				//				Sleep(1000);
				//				window.draw(rat);
				//				window.display();
				copy[here.row][here.col] = 5;
				tiles[here.row][here.col].setFillColor(sf::Color::Yellow);
				window->draw(tiles[here.row][here.col]);
				here = path.top();
				tiles[here.row][here.col].setFillColor(sf::Color::White);
				window->draw(tiles[here.row][here.col]);
				path.pop();

				window->display();
				sf::sleep(sf::seconds(0.3));
				window->clear();
				create_maze(window);
			}
		}
	}
	//print out the path
	std::cout << "\nThis is the path found." << std::endl;
	//	std::cout<<"("<<exit.row<<","<<exit.col<<")"<<std::endl;
	path.push(exit);

	index = path.size() - 1;
	int temp = index;
	while (!path.empty())
	{

		std::cout << "(" << path.top().row << "," << path.top().col << ")" << std::endl;
		final_path[temp] = path.top();
		temp = temp - 1;
		path.pop();
	}

	//	std::cout<<"Temp: "<<temp;
	//	std::cout<<"Index: "<<index;
	//	window.close();
	return true;
}

int main()
{

	//	sf::SoundBuffer buffer;
	//	buffer.loadFromFile("success.wav.wav");
	//	sf::Sound sound;
	//	sound.setBuffer(buffer);

	system("Color 0A");
	sf::RenderWindow window(sf::VideoMode(1300, 1300), "Maze problem", sf::Style::Close | sf::Style::Titlebar | sf::Style::Resize);
	std::cout << "WELCOME TO RAT IN A MAZE!" << std::endl;
	std::cout << "======= == === == = ====";
	std::cout << "\nEnter size of the maze(upto 8):: ";
	std::cin >> n;
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			copy[i][j] = 0;
		}
	}

	while (window.isOpen())
	{
		sf::Event evnt;

		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{

				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::KeyPressed:
					if (evnt.key.code == sf::Keyboard::Return)
					{
						if (find_path(&window) == false)
						{
							std::cout << "\nSorry,Path not found!";
						}
						else
						{
							window.clear();

							for (int i = 0; i <= index; i++)
							{
								copy[final_path[i].row][final_path[i].col] = 2; //'2' represents the path
							}
							for (int i = 0; i <= n + 1; i++)
							{
								for (int j = 0; j <= n + 1; j++)
								{
									if (copy[i][j] == 0 || copy[i][j] == 5)
									{
										tiles[i][j].setFillColor(sf::Color::Transparent);
									}
									else if (copy[i][j] == 1)
									{
										tiles[i][j].setFillColor(sf::Color::Magenta);
									}
									else if (copy[i][j] == 2)
									{
										tiles[i][j].setFillColor(sf::Color::White);
									}
									else
									{
										tiles[i][j].setFillColor(sf::Color::Blue);
									}
									window.draw(tiles[i][j]);
								}
							}
							window.display();

							Sleep(5000);
							//							sound.play();
							window.close();
							break;
						}
					}

				default: break;
			}
			float x_cor = 200;
			float y_cor = 200;
			for (int i = 1; i <= n; i++)
			{
				for (int j = 1; j <= n; j++)
				{
					tiles[i][j] = sf::RectangleShape(sf::Vector2f(100.f, 100.f));
					tiles[i][j].setFillColor(sf::Color::Transparent);
					tiles[i][j].setOutlineThickness(3);
					tiles[i][j].setOutlineColor(sf::Color::Blue);

					tiles[i][j].setPosition(x_cor, y_cor);
					x_cor = x_cor + 100;
					window.draw(tiles[i][j]);
				}

				y_cor = y_cor + 100;
				x_cor = 200;
			}
			//creating  a wall around the maze
			for (int i = 0; i <= n + 1; i++)
			{
				//					tiles[i][]0=sf::RectangleShape(sf::Vector2f(100.f,100.f));
				//					tiles[i][0].setPosition(100*(0-1)+200,100*(i-1)+200)
				//					tiles[i][0].setFillColor(sf::Color::Blue);
				put_a_wall(tiles, i, 0);
				window.draw(tiles[i][0]);
				put_a_wall(tiles, 0, i);
				window.draw(tiles[0][i]);
				put_a_wall(tiles, i, n + 1);
				window.draw(tiles[i][n + 1]);
				put_a_wall(tiles, n + 1, i);
				window.draw(tiles[n + 1][i]);

				copy[i][0] = copy[0][i] = -1;
				copy[i][n + 1] = copy[n + 1][i] = -1;
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2i mouse_loc = sf::Mouse::getPosition(window);

				for (int i = 1; i <= n; i++)
				{
					for (int j = 1; j <= n; j++)
					{
						if (mouse_loc.x >= 100 * (j - 1) + 200 && mouse_loc.x <= 100 * j + 200 && mouse_loc.y >= 100 * (i - 1) + 200 && mouse_loc.y <= 100 * i + 200)
						{
							copy[i][j] = 1;
							tiles[i][j].setFillColor(sf::Color::Magenta);
							window.draw(tiles[i][j]);
						}
					}
				}
			}
		}

		window.display();
	}
	window.close();

	//	for(int i=0;i<=n+1;i++){
	//		for(int j=0;j<=n+1;j++){
	//			std::cout<<a[i][j]<<" ";
	//		}
	//		std::cout<<std::endl;
	//	}
	//

	//	if(find_path()==false){
	//		std::cout<<"\nSorry,Path not found!";
	//	}
	return 0;
}