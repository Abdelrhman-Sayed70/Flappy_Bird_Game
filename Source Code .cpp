#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <deque>
#include <fstream>
#include<sstream>

using namespace sf;
using namespace std;



void get_BirdColor(char& colorChar); //Random Generator (Bird color)
void get_BirdColorNums(char colorChar, int& birdColorIndex, float& flySpeed, int& counter); //Getting the correct numbers for the selected color
int get_topPipeYpos(int topPipe_minYpos, int topPipe_maxYpos); //Getting a random Y-position for the TOP pipe

int main()
{
	RenderWindow window(VideoMode(1280, 860), "FlappyBird", Style::Titlebar | Style::Close);
	//window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	/*     TEXTURES & SPRITES     */


#pragma region Font

	Font gameFont;
	if (!gameFont.loadFromFile("fonts/Flappy_Bird.ttf"))
		return EXIT_FAILURE;

#pragma endregion 

#pragma region File

	ofstream previousScores("previousScores.txt");

#pragma endregion 


#pragma region Sound

	// struct for Sounds ------

	struct Allsounds
	{
		SoundBuffer pointBuffer, flyBuffer, hitBuffer, swooshBuffer, dieBuffer;
	} Sounds;

	if (!Sounds.pointBuffer.loadFromFile("Flappy_bird audios/assets_audio_point.ogg")
		|| !Sounds.flyBuffer.loadFromFile("Flappy_bird audios/assets_audio_Wing.ogg")
		|| !Sounds.hitBuffer.loadFromFile("Flappy_bird audios/assets_audio_hit.ogg")
		|| !Sounds.swooshBuffer.loadFromFile("Flappy_bird audios/assets_audio_swoosh.ogg")
		|| !Sounds.dieBuffer.loadFromFile("Flappy_bird audios/assets_audio_die.ogg")
		)
		return EXIT_FAILURE;

	Sound point_sound, fly_sound, hit_sound, swoosh_sound, die_sound;
	point_sound.setBuffer(Sounds.pointBuffer);
	fly_sound.setBuffer(Sounds.flyBuffer);
	hit_sound.setBuffer(Sounds.hitBuffer);
	swoosh_sound.setBuffer(Sounds.swooshBuffer);
	die_sound.setBuffer(Sounds.dieBuffer);


#pragma endregion		 
	// Here you could find a Struct -----


#pragma region Main Menu

	// Play Button
	Texture play_button;
	if (!play_button.loadFromFile("Flappy_bird Assets/PlayButton.png"))
		return EXIT_FAILURE;

	RectangleShape playButton(Vector2f(251, 135));
	playButton.setTexture(&play_button);
	playButton.setOrigin(251 / 2, 135 / 2);
	playButton.setPosition(window.getSize().x / 2, 450);
	bool isPlayButton_Pressed = false;

	// FlappBird Word
	Text flappyBird_word("FlappyBird", gameFont, 150);
	flappyBird_word.setFillColor(Color::White);
	flappyBird_word.setOutlineThickness(10);
	flappyBird_word.setOutlineColor(Color(47, 79, 79));
	flappyBird_word.setPosition(235, 65);

	// Credits Word
	Text credits_word("Credits", gameFont, 60);
	credits_word.setPosition(60, window.getSize().y - 120);
	credits_word.setFillColor(Color(255, 245, 0));
	credits_word.setOutlineThickness(3.0);
	credits_word.setOutlineColor(Color::Black);
	bool isCredits_Pressed = false;

	// HighScore Word
	Text highScore_word("HighScore", gameFont, 60);
	highScore_word.setPosition(window.getSize().x - 336, window.getSize().y - 120);
	highScore_word.setFillColor(Color(255, 245, 0));
	highScore_word.setOutlineThickness(2.0);
	highScore_word.setOutlineColor(Color::Black);
	bool isHighScore_Pressed = false;

#pragma endregion

#pragma region Creidts

	Text credit_names("Abdelrahman Khatab\tSuhail Mahmoud\tAbdo Said\n\nSalma Ayman\tAhmed NaserEl-Den\tMohamed Mobark", gameFont, 40);
	Text madyBy("A Game By ", gameFont, 70);
	Text return_to_mainMenu("Press \"ESC\" to return to Main Menu", gameFont, 25);

	Texture credit_names_BG;
	if (!credit_names_BG.loadFromFile("Flappy_bird Assets/Credit Back ground.jpg"))  // with the bird 
		return EXIT_FAILURE;
	Sprite credit_names_BGSprite(credit_names_BG);

	// Names
	credit_names.setFillColor(Color::White);
	credit_names.setOutlineThickness(2.0);
	credit_names.setOutlineColor(Color::Black);
	credit_names.setPosition(90, 370);

	// Made By
	madyBy.setFillColor(Color::Black);
	madyBy.setOutlineThickness(2.5);
	madyBy.setOutlineColor(Color::White);
	madyBy.setPosition(55, 150);

	// Return to Main Menu
	return_to_mainMenu.setFillColor(Color(50, 168, 82));
	return_to_mainMenu.setOutlineThickness(2.5);
	return_to_mainMenu.setOutlineColor(Color::White);
	return_to_mainMenu.setPosition(50, window.getSize().y - 240);

#pragma endregion

#pragma region Get Ready

	Texture get_Ready;
	if (!get_Ready.loadFromFile("Flappy_bird Assets/GetReady.png"))
		return EXIT_FAILURE;

	RectangleShape getReady(Vector2f(400, 400));
	getReady.setTexture(&get_Ready);
	getReady.setOrigin(getReady.getSize().x / 2, getReady.getSize().y / 2);
	getReady.setPosition(window.getSize().x / 2, window.getSize().y / 2);
	bool isGetReady_Pressed = false;

#pragma endregion


#pragma region Background 

	//---Background image in struct 

	struct BG_Imag
	{
		Texture bkImage[3];

	} BGImages;

	if (!BGImages.bkImage[0].loadFromFile("Flappy_bird Assets/background-day.png")
		|| !BGImages.bkImage[1].loadFromFile("Flappy_bird Assets/background-night.png")
		|| !BGImages.bkImage[2].loadFromFile("Flappy_bird Assets/background-day-blur.png")
		)
		return EXIT_FAILURE;

	Sprite bgi[3];
	bgi[0].setTexture(BGImages.bkImage[0]);
	bgi[1].setTexture(BGImages.bkImage[1]);
	bgi[2].setTexture(BGImages.bkImage[2]);

	//Random Generator (day - night)
	bool day;
	day = rand() % 2;

#pragma endregion
	// Here you could find a Struct -----


#pragma region Bird & Gravity

	struct birdimgs
	{
		Texture Bird[9];

	}BirdImages;

	if (
		//Red Bird
		!BirdImages.Bird[0].loadFromFile("Flappy_bird Assets/redbird-upflap.png")
		|| !BirdImages.Bird[1].loadFromFile("Flappy_bird Assets/redbird-midflap.png")
		|| !BirdImages.Bird[2].loadFromFile("Flappy_bird Assets/redbird-downflap.png")
		//Blue Bird
		|| !BirdImages.Bird[3].loadFromFile("Flappy_bird Assets/blue_bird-upflap.png")
		|| !BirdImages.Bird[4].loadFromFile("Flappy_bird Assets/blue_bird-midflap.png")
		|| !BirdImages.Bird[5].loadFromFile("Flappy_bird Assets/blue_bird-downflap.png")
		//Yellow Bird
		|| !BirdImages.Bird[6].loadFromFile("Flappy_bird Assets/yellow_bird-upflap.png")
		|| !BirdImages.Bird[7].loadFromFile("Flappy_bird Assets/yellow_bird-midflap.png")
		|| !BirdImages.Bird[8].loadFromFile("Flappy_bird Assets/yellow_bird-downflap.png")
		)
		return EXIT_FAILURE;

	RectangleShape bird(Vector2f(81, 62));
	bird.setOrigin(bird.getSize().x / 2, bird.getSize().y / 2);
	bird.setPosition(window.getSize().x / 2, window.getSize().y / 2);
	char colorChar;

	//Getting bird color
	get_BirdColor(colorChar);

	//Fly Speed 
	int birdColorIndex;
	float flySpeed;
	int counter;

	//Getting bird color's Numbers
	get_BirdColorNums(colorChar, birdColorIndex, flySpeed, counter);

	//---Gravity---
	const float Gravity = 0.89f;
	const float jumpSpeed = 10.0f;
	float rotationAngle = 0;
	Vector2f jumpVelocity(0, 0);

#pragma endregion
	// Here you could find a Struct -----


#pragma region Base

	//---Base---
	Texture Base;
	if (!Base.loadFromFile("Flappy_bird Assets/base.png"))
		return EXIT_FAILURE;

	RectangleShape base(Vector2f(1900, 112));
	base.setTexture(&Base);

	float groundHeight = window.getSize().y - base.getSize().y;
	base.setPosition(-3, groundHeight);

	Vector2f baseVelocity(-5, 0);

#pragma endregion

#pragma region Pipes

	Texture gPipe[2], rPipe[2];
	if (!gPipe[0].loadFromFile("Flappy_bird Assets/gTopPipe.png")
		|| !gPipe[1].loadFromFile("Flappy_bird Assets/gBottomPipe.png"))
		return EXIT_FAILURE;

	if (!rPipe[0].loadFromFile("Flappy_bird Assets/rTopPipe.png")
		|| !rPipe[1].loadFromFile("Flappy_bird Assets/rBottomPipe.png"))
		return EXIT_FAILURE;

	RectangleShape top_pipe(Vector2f(130, window.getSize().y));
	RectangleShape bottom_pipe(Vector2f(130, window.getSize().y));
	top_pipe.setOrigin(0, top_pipe.getSize().y);

	deque <RectangleShape> topPipe, bottomPipe;
	/*NEW*/int pipeIndex = 0;
	int sizeOf_PipesDeque = 0;

	const int topPipe_minYpos = 120, topPipe_maxYpos = 380; // Top Pipe minimum & bestScoremum positions on Y-axis 
	float topPipeYpos, bottomPipeYpos; // To save top & bottom pipe's positions 

	/*NEW*/const int numOfPipes = 10  ; 
	/*NEW*/int numOfPipesCounter = 0;
	int pipeSpawnTimer = 0; // Time between each pipe
	bool isCollided = false;
	bool isScored = false;

	const float distanceY_betweenPipes = 300; // Distance between Pipes on Y-axis
	const float distanceX_betweenPipes = 85;	// Distance between Pipes on X-axis

#pragma endregion

#pragma region Show score during the game

	int scoreCounter = 0;
	ostringstream ssScore;
	ssScore << scoreCounter;
	Text labelScore;
	labelScore.setCharacterSize(60);
	labelScore.setFont(gameFont);
	labelScore.setOutlineThickness(2.5);
	labelScore.setOutlineColor(Color::Black);
	labelScore.setString(ssScore.str());
	labelScore.setPosition(window.getSize().x / 2, 60);


#pragma endregion

#pragma region GamePause

	bool isPaused = false;
	Text Pause("Game Paused", gameFont, 120);
	Text resume("Press \"P\" to resume", gameFont, 40);

	//Pause word
	Pause.setPosition(280, 200);
	Pause.setOutlineThickness(4.0);
	Pause.setOutlineColor(Color::Black);

	//Resume word 
	resume.setPosition(430, 390);
	resume.setOutlineThickness(3.5);
	resume.setOutlineColor(Color::Black);

#pragma endregion 

#pragma region Game Over

	Texture game_over;
	if (!game_over.loadFromFile("Flappy_bird Assets/gameover.png"))
		return EXIT_FAILURE;

	RectangleShape gameOver(Vector2f(500, 115));
	gameOver.setTexture(&game_over);
	gameOver.setOrigin(gameOver.getSize().x / 2, gameOver.getSize().y / 2);
	gameOver.setPosition(window.getSize().x / 2, -200);
	bool isGameOver = false;

#pragma endregion

#pragma region Score Board 

	Texture score_board[5];
	if (!score_board[0].loadFromFile("Flappy_bird Assets/emptyboard.png")
		|| !score_board[1].loadFromFile("Flappy_bird Assets/bronzemedal.png")
		|| !score_board[2].loadFromFile("Flappy_bird Assets/silvermedal.png")
		|| !score_board[3].loadFromFile("Flappy_bird Assets/goldmedal.png")
		|| !score_board[4].loadFromFile("Flappy_bird Assets/platinummedal.png")
		)
		return EXIT_FAILURE;

	RectangleShape scoreBoard(Vector2f(400, 220));
	scoreBoard.setOrigin(scoreBoard.getSize().x / 2, scoreBoard.getSize().y / 2);
	scoreBoard.setPosition(window.getSize().x / 2, window.getSize().y + 200);

#pragma endregion

#pragma region New & High Score (Numbers)

	// Current Score (Number)
	Text currentScoreNum;
	currentScoreNum.setFont(gameFont);
	currentScoreNum.setCharacterSize(25);
	currentScoreNum.setPosition(770, 330);
	currentScoreNum.setPosition(770, scoreBoard.getPosition().y - 40);

	// High Score (Number)
	Text highScoreNum;
	highScoreNum.setFont(gameFont);
	highScoreNum.setPosition(770, scoreBoard.getPosition().y + 40);
	int bestScore = 0;

#pragma endregion

#pragma region Replay & Main Menu Buttons

	// Replay Button
	Texture replay_button;
	if (!replay_button.loadFromFile("Flappy_bird Assets/PlayButton.png"))
		return EXIT_FAILURE;

	RectangleShape replayButton(Vector2f(150, 90));
	replayButton.setTexture(&replay_button);
	replayButton.setPosition(-200, 510);

	// Main Menu Button
	Text mainMenuButton("Main\nMenu", gameFont, 47);
	mainMenuButton.setFillColor(Color::White);
	mainMenuButton.setOutlineThickness(2.5);
	mainMenuButton.setOutlineColor(Color::Black);
	mainMenuButton.setPosition(window.getSize().x + 10, 510);

#pragma endregion

#pragma region Congratulations

	Text congrats("Congratulations", gameFont, 90);
	congrats.setFillColor(sf::Color::White);
	congrats.setOutlineColor(sf::Color(127, 0, 255));
	congrats.setOutlineThickness(30);
	congrats.setPosition((window.getSize().x) - 975, -200);
	bool isWon = false;

#pragma endregion

	/*     END-TEXTURE & SPRITES     */

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		/*     UPDATE     */

#pragma region Update

		if (!isPaused && !isGameOver)
		{

#pragma region Bird

			if (isPlayButton_Pressed)
			{
				// Wings Motion
				if (flySpeed < birdColorIndex && !isCollided)
				{
					counter = flySpeed;
					bird.setTexture(&BirdImages.Bird[counter]);
					if (!isGetReady_Pressed)
						flySpeed += 0.15;
					else
						flySpeed += 0.35;
				}
				else
					get_BirdColorNums(colorChar, birdColorIndex, flySpeed, counter); //To reset the correct numbers for the selected color

				// Jumping, rotationAngle & Gravity
				if (isGetReady_Pressed && !isWon)
				{
					if (!isCollided)
					{
						//---Jumping & rotationAngle---
						if (Keyboard::isKeyPressed(Keyboard::Space) || Mouse::isButtonPressed(Mouse::Left))
						{
							fly_sound.play();

							// Jumping
							jumpVelocity.y = -jumpSpeed;

							// Rotation
							rotationAngle = -35;
						}
					}
					//---Gravity---
					if (bird.getPosition().y + bird.getSize().y / 2 <= groundHeight) //   /2 NEW
					{
						jumpVelocity.y += Gravity;
						rotationAngle += 2;
						if (rotationAngle > 90)
						{
							rotationAngle = 90;
						}
					}
					else //---Game Over---
					{
						bird.setPosition(bird.getPosition().x, groundHeight - bird.getSize().y / 2); //    /2 NEW
						die_sound.play();
						isGameOver = true;
						jumpVelocity.y = 0;
					}

					// Motion
					bird.move(jumpVelocity);
					bird.setRotation(rotationAngle);
				}
			}
#pragma endregion

#pragma region Base

			//Base 
			if (isPlayButton_Pressed && !isCollided)
			{
				base.move(baseVelocity); // Moves the base to the left 
				if (base.getPosition().x <= -625)
					base.setPosition(-3, groundHeight);
			}

#pragma endregion

#pragma region Pipes & Collision

			if (isGetReady_Pressed && !isCollided)
			{
				// Pushing Pipes
				topPipeYpos = get_topPipeYpos(topPipe_minYpos, topPipe_maxYpos); // Setting TOP pipe's position
				bottomPipeYpos = topPipeYpos + distanceY_betweenPipes; // Setting BOTTOM pipe's position

				if (numOfPipesCounter < numOfPipes)
				{
					if (pipeSpawnTimer < distanceX_betweenPipes) // Determine the distance between pipes on X-axis
						pipeSpawnTimer++;

					if (pipeSpawnTimer == distanceX_betweenPipes)
					{
						top_pipe.setPosition(window.getSize().x + 30, topPipeYpos); // Sets the Top Pipe position
						bottom_pipe.setPosition(window.getSize().x + 30, bottomPipeYpos); // Sets the Bottom Pipe position

						topPipe.push_back(RectangleShape(top_pipe)); // Add a pipe to the top_deque
						bottomPipe.push_back(RectangleShape(bottom_pipe)); // Add a pipe to the bottom_deque
						sizeOf_PipesDeque++;
						numOfPipesCounter++;
						pipeSpawnTimer = 0;
					}
				}
				// Moving Pipes
				for (int i = 0; i < sizeOf_PipesDeque; i++)
				{
					topPipe[i].move(baseVelocity); // Moves the TOP pipe to the left 
					bottomPipe[i].move(baseVelocity);	// Moves the BOTTOM pipe to the left 
				}

#pragma region Collisions & Scoring

				if (sizeOf_PipesDeque > 0 && scoreCounter != numOfPipes)
				{
					//---TOP Pipe Collision---
					if (bird.getGlobalBounds().intersects(topPipe[pipeIndex].getGlobalBounds()))
					{
						hit_sound.play();
						isCollided = true;
					}
					//---BOTTOM Pipe Collision---
					else if (bird.getGlobalBounds().intersects(bottomPipe[pipeIndex].getGlobalBounds()))
					{
						hit_sound.play();
						isCollided = true;
					}

					//---Scoring---
					if (bird.getPosition().x >= topPipe[pipeIndex].getPosition().x)
					{
						if (bird.getPosition().x - bird.getSize().x / 2 <= topPipe[pipeIndex].getPosition().x + topPipe[pipeIndex].getSize().x)
						{
							if (!isScored)
							{
								point_sound.play();
								isScored = true;
								scoreCounter++;
								previousScores << scoreCounter << endl;
							}
						}
					}
					if (bird.getPosition().x - bird.getSize().x / 2 >= topPipe[pipeIndex].getPosition().x + topPipe[pipeIndex].getSize().x)
					{
						pipeIndex++;
						isScored = false;
					}
				}

				//---Ground Collision---
				if (bird.getGlobalBounds().intersects(base.getGlobalBounds()))
				{
					hit_sound.play();
					isCollided = true;
				}

				//---Sky Collision---
				if (bird.getPosition().y <= -90)
				{
					hit_sound.play();
					isCollided = true;
				}
				// End Collision

#pragma endregion

			} // end if !isCollided

#pragma endregion

		}

#pragma region Game Pause & Return to Main Menu

		//Return to main menu before starting
		if (isPlayButton_Pressed && !isGetReady_Pressed)
		{
			if (Keyboard::isKeyPressed(Keyboard::Escape))
				isPlayButton_Pressed = false;
		}


		//Game Pause 
		if (isGetReady_Pressed && !isCollided /*NEW*/ && !isWon)
		{
			if (Keyboard::isKeyPressed(Keyboard::Escape))
				isPaused = true;

			if (Keyboard::isKeyPressed(Keyboard::P))
				isPaused = false;
		}

#pragma endregion

#pragma region Play Button

		if (!isPlayButton_Pressed && !isCredits_Pressed && !isHighScore_Pressed)
		{
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				// transform the mouse position from window coordinates to world coordinates
				Vector2f mouse = window.mapPixelToCoords(Mouse::getPosition(window));

				// retrieve the bounding box of the sprite
				FloatRect bounds = playButton.getGlobalBounds();

				// hit test
				if (bounds.contains(mouse))
				{
					isPlayButton_Pressed = true;
					swoosh_sound.play();
				}
			}
		}

#pragma endregion 

#pragma region Get Ready Button

		if (isPlayButton_Pressed)
		{
			if (Keyboard::isKeyPressed(Keyboard::Space))
				isGetReady_Pressed = true;
		}

#pragma endregion

#pragma region High Score Button

		if (!isPlayButton_Pressed && !isCredits_Pressed)
		{
			// Open High Score word
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				// transform the mouse position from window coordinates to world coordinates
				Vector2f mouse = window.mapPixelToCoords(Mouse::getPosition(window));

				// retrieve the bounding box of the sprite
				FloatRect bounds = highScore_word.getGlobalBounds();

				// hit test
				if (bounds.contains(mouse))
				{
					swoosh_sound.play();
					isHighScore_Pressed = true;
				}
			}

			// Return to Main Menu
			if (isHighScore_Pressed)
			{
				if (Keyboard::isKeyPressed(Keyboard::Escape))
				{
					swoosh_sound.play();
					isHighScore_Pressed = false;
				}
			}
		}

#pragma endregion

#pragma region Credits Button

		if (!isPlayButton_Pressed && !isHighScore_Pressed)
		{
			// Open credits_words
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				// transform the mouse position from window coordinates to world coordinates
				Vector2f mouse = window.mapPixelToCoords(Mouse::getPosition(window));

				// retrieve the bounding box of the sprite
				FloatRect bounds = credits_word.getGlobalBounds();

				// hit test
				if (bounds.contains(mouse))
				{
					swoosh_sound.play();
					isCredits_Pressed = true;
				}
			}

			// Return to Main Menu
			if (isCredits_Pressed)
			{
				if (Keyboard::isKeyPressed(Keyboard::Escape))
				{
					swoosh_sound.play();
					isCredits_Pressed = false;
				}
			}
		}

#pragma endregion

#pragma region Game Over

		// Motion
		if (isGameOver)
		{
			if (gameOver.getPosition().y <= 150)
				gameOver.move(0, 10);
		}

#pragma endregion

#pragma  region High Score

		ifstream HighScoref("previousScores.txt");
		if (HighScoref.is_open())
		{
			int n;
			int i = 0;
			while (HighScoref >> n)
			{

				if (i++ == 0)
					bestScore = n;


				if (n > bestScore)
					bestScore = n;
			}

		}
		else
			return EXIT_FAILURE;

#pragma endregion

#pragma region Score Board

		if (isGameOver || isWon)
		{
			// Moves the Score Board
			if (scoreBoard.getPosition().y >= 380)
				scoreBoard.move(0, -10);


			// Moves the Current Score
			if (currentScoreNum.getPosition().y >= 330)
				currentScoreNum.move(0, -10);


			// Moves the High Score
			if (highScoreNum.getPosition().y >= 410)
				highScoreNum.move(0, -10);
		}
#pragma endregion

#pragma region Replay & Main Menu Buttons

		if (isGameOver || isWon)
		{

#pragma region Motion

			// Motion of both buttons when (isGameOver)
			if (scoreBoard.getPosition().y <= 700)
			{
				if (replayButton.getPosition().x <= window.getSize().x / 2 - 150)
					replayButton.move(15, 0);

				if (mainMenuButton.getPosition().x >= window.getSize().x / 2 + 50)
					mainMenuButton.move(-15, 0);
			}

#pragma endregion

#pragma region Replay Button

			if (Mouse::isButtonPressed(Mouse::Left))
			{
				// transform the mouse position from window coordinates to world coordinates
				Vector2f mouse = window.mapPixelToCoords(Mouse::getPosition(window));

				// retrieve the bounding box of the sprite
				FloatRect bounds = replayButton.getGlobalBounds();

				// hit test
				if (bounds.contains(mouse) && replayButton.getPosition().x >= window.getSize().x / 2 - 150)
				{
					swoosh_sound.play();
					// Reset Day
					day = rand() % 2;

					// Reset Pipes
					sizeOf_PipesDeque = 0;
					pipeIndex = 0;
					pipeSpawnTimer = 0;
					numOfPipesCounter = 0;
					topPipe.clear();
					bottomPipe.clear();

					// Reset Bird
					bird.setPosition(window.getSize().x / 2, window.getSize().y / 2);
					bird.setRotation(0);

					get_BirdColor(colorChar);
					get_BirdColorNums(colorChar, birdColorIndex, flySpeed, counter);

					// Reset Game Over & Score Board
					gameOver.setPosition(window.getSize().x / 2, -200);
					scoreBoard.setPosition(window.getSize().x / 2, window.getSize().y + 200);

					// Reset Buttons except "Play Button"
					replayButton.setPosition(-200, 510);
					mainMenuButton.setPosition(window.getSize().x + 10, 510);
					isGetReady_Pressed = false;

					// Reset Collision & Won booleans
					isGameOver = false;
					isWon = false;
					isCollided = false;

					// Reset Score
					scoreCounter = 0;
					isScored = false;

					// Reset High & Current Scores' Positions
					highScoreNum.setPosition(770, scoreBoard.getPosition().y + 40);
					currentScoreNum.setPosition(770, scoreBoard.getPosition().y - 40);

					// Reset Congrats Pos
					congrats.setPosition((window.getSize().x) - 975, -200);
				}
			}

#pragma endregion

#pragma region Main Menu Button


			// Main Menu Button
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				// transform the mouse position from window coordinates to world coordinates
				Vector2f mouse = window.mapPixelToCoords(Mouse::getPosition(window));

				// retrieve the bounding box of the sprite
				FloatRect bounds = mainMenuButton.getGlobalBounds();

				// hit test
				if (bounds.contains(mouse) && mainMenuButton.getPosition().x <= window.getSize().x / 2 + 50)
				{
					swoosh_sound.play();

					// Reset Day
					day = rand() % 2;

					// Reset Pipes
					sizeOf_PipesDeque = 0;
					pipeIndex = 0;
					pipeSpawnTimer = 0;
					numOfPipesCounter = 0;
					topPipe.clear();
					bottomPipe.clear();

					// Reset Bird
					bird.setPosition(window.getSize().x / 2, window.getSize().y / 2);
					bird.setRotation(0);

					get_BirdColor(colorChar);
					get_BirdColorNums(colorChar, birdColorIndex, flySpeed, counter);

					// Reset Game Over & Score Board
					gameOver.setPosition(window.getSize().x / 2, -200);
					scoreBoard.setPosition(window.getSize().x / 2, window.getSize().y + 200);

					// Reset Buttons
					isPlayButton_Pressed = false;
					isGetReady_Pressed = false;
					replayButton.setPosition(-200, 510);
					mainMenuButton.setPosition(window.getSize().x + 10, 510);

					// Reset Collision & Won booleans
					isGameOver = false;
					isWon = false;
					isCollided = false;

					// Reset Score
					scoreCounter = 0;
					isScored = false;

					// Reset HighScore Pos & CurrentScore Pos
					highScoreNum.setPosition(770, scoreBoard.getPosition().y + 40);
					currentScoreNum.setPosition(770, scoreBoard.getPosition().y - 40);

					// Reset Congrats Pos
					congrats.setPosition((window.getSize().x) - 975, -200);
				}
			}
#pragma endregion

		}

#pragma endregion

#pragma region Win

		if (scoreCounter == numOfPipes)
		{
			if (topPipe.back().getPosition().x < -top_pipe.getSize().x)
			{
				isWon = true;
				if (congrats.getPosition().y <= 100)
					congrats.move(0, 10);
				bird.setPosition(window.getSize().x / 2, window.getSize().y / 2);
				bird.setRotation(0);
			}
		}

#pragma endregion


#pragma endregion

		/*     END-UPDATE     */

		window.clear();
		/*     DRAWING     */

#pragma region Draiwing

		//Background Image
		if (day)
			window.draw(bgi[0]);
		else
			window.draw(bgi[1]);

		//Pipes
		for (int i = 0; i < sizeOf_PipesDeque; i++)
		{
			if (day)
			{
				topPipe[i].setTexture(&gPipe[0]);
				bottomPipe[i].setTexture(&gPipe[1]);
			}
			else
			{
				topPipe[i].setTexture(&rPipe[0]);
				bottomPipe[i].setTexture(&rPipe[1]);
			}
			window.draw(topPipe[i]);
			window.draw(bottomPipe[i]);
		}

		//Base
		window.draw(base);

		//Bird
		window.draw(bird);

		//Game Paused
		if (isPaused && isGetReady_Pressed && !isGameOver)
		{
			window.draw(Pause);
			window.draw(resume);
		}

		//Current score
		if (isGetReady_Pressed && !isGameOver && !isWon)
		{
			ssScore.str(""); // Updates the Score Value
			ssScore << scoreCounter; // Sets the new Value 
			labelScore.setString(ssScore.str()); // Sets the value to a string 
			window.draw(labelScore);
		}

		//Game Over || Won
		/*NEW*/if (isGameOver || isWon)
		{
			//Game Over
			window.draw(gameOver);

			//Score Board
			if (scoreCounter < 10) // NO Medal
				scoreBoard.setTexture(&score_board[0]);

			else if (scoreCounter >= 10 && scoreCounter < 20) // Bronze Medal
				scoreBoard.setTexture(&score_board[1]);

			else if (scoreCounter >= 20 && scoreCounter < 30) // Silver Medal
				scoreBoard.setTexture(&score_board[2]);

			else if (scoreCounter >= 30 && scoreCounter < 40) // Gold Medal
				scoreBoard.setTexture(&score_board[3]);

			else // Platinum Medal
				scoreBoard.setTexture(&score_board[4]);

			window.draw(scoreBoard);

			//Replay & Main Menu Buttons
			window.draw(replayButton);
			window.draw(mainMenuButton);

			//High Score number
			highScoreNum.setCharacterSize(25);
			highScoreNum.setString(to_string(bestScore));
			highScoreNum.setOutlineThickness(2.0);
			highScoreNum.setOutlineColor(Color::Black);

			currentScoreNum.setString(to_string(scoreCounter));
			currentScoreNum.setOutlineThickness(2.0);
			currentScoreNum.setOutlineColor(Color::Black);
			window.draw(highScoreNum);
			window.draw(currentScoreNum);
		}

		//Blur Background Image
		if (!isPlayButton_Pressed)
			window.draw(bgi[2]);

		//Main Menu Texts
		if (!isPlayButton_Pressed)
		{
			window.draw(credits_word);
			window.draw(highScore_word);
			window.draw(flappyBird_word);
		}

		//Play Button
		if (!isPlayButton_Pressed)
		{
			window.draw(playButton);
		}

		//Credits Button
		if (isCredits_Pressed)
		{
			window.draw(credit_names_BGSprite);
			window.draw(credit_names);
			window.draw(madyBy);
			window.draw(return_to_mainMenu);
		}

		//HighScore Button
		if (isHighScore_Pressed)
		{
			window.draw(credit_names_BGSprite);
			highScoreNum.setPosition(605, 320);
			highScoreNum.setCharacterSize(120);
			highScoreNum.setOutlineThickness(3.5);
			highScoreNum.setOutlineColor(Color::Black);
			highScoreNum.setString(to_string(bestScore));
			window.draw(highScoreNum);
			window.draw(return_to_mainMenu);
			highScoreNum.setPosition(770, scoreBoard.getPosition().y + 40);
		}

		//Get Ready
		if (isPlayButton_Pressed && !isGetReady_Pressed)
		{
			window.draw(getReady);
		}

		// Congrats 
		if (isWon)
			window.draw(congrats);

#pragma endregion

		/*     END-DRAWING     */

		window.display();

	}

	return 0; // End of application
}

void get_BirdColor(char& colorChar)
{
	int random = rand() % 3; //DevSkim: ignore DS148264
	if (random == 0)
		colorChar = 'R';
	if (random == 1)
		colorChar = 'B';
	if (random == 2)
		colorChar = 'Y';
}
void get_BirdColorNums(char colorChar, int& birdColorIndex, float& flySpeed, int& counter)
{
	if (colorChar == 'R')
	{
		flySpeed = 0;
		counter = 0;
		birdColorIndex = 3;
	}
	else if (colorChar == 'B')
	{
		flySpeed = 3;
		counter = 3;
		birdColorIndex = 6;
	}
	else if (colorChar == 'Y')
	{
		flySpeed = 6;
		counter = 6;
		birdColorIndex = 9;
	}
}
int get_topPipeYpos(int topPipe_minYpos, int topPipe_maxYpos)
{
	int randomYpos;
	for (int i = topPipe_minYpos; i < topPipe_maxYpos; i++)
	{
		randomYpos = (rand() % (topPipe_maxYpos - topPipe_minYpos)) + topPipe_minYpos;
	}
	return randomYpos;
}
