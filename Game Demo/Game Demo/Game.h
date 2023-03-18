/// <summary>
/// author Harvey McNevin February 2023
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cstdlib>




float vectorLength(sf::Vector2f t_pos);

enum class Direction
{
	None,
	Left,
	Right,
};

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	void setupSprite();
	
	void setUpGraveRobber();
	void setUpWoodCutter();
	void setUpSteamMan();
	void setUpColorMan();
	void setUpPlanet();
	int setPlayer();
	int setPlanet();

	void checkCollision();
	void animatePlanet();
	void idleAnimation();
	void setUpBackGround();
	void checkDirection();
	void walkAnimation();
	void move();
	void runningAnimation();
	void running();
	void jumpingAnimation();
	void jumping();
	void checkBounds();
	void whereAmI();
	

	
	
	
	
	
	const unsigned int SCREEN_WIDTH = 1100U;
	const unsigned int SCREEN_HEIGHT = 700U;
	const unsigned int NUMBER_OF_BACKGROUNDS{ 5 };
	const unsigned int NUMBER_OF_PLAYERS{ 4 };
	const unsigned int NUMBER_OF_PLANETS{ 5 };
	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message

	
	sf::Texture
		m_backGroundTexture[5], // texture used for background
		m_playerIdleTexture[4], // texture for idle sprite
		m_playerWalkTexture[4], // texture for walking spritesheet
		m_playerRunTexture[4], // texture for running spritesheet
		m_playerJumpTexture[4], // texture for jumping spritesheet
		m_planetTexture[5]; // texture for planet

	sf::Sprite m_backGroundSprite; // sprite used for background
	sf::Sprite m_playerSprite; // sprite for player
	sf::Sprite m_planetSprite; // sprite for planet
	sf::Font m_gameFont; // font for messages





	Direction m_playerDirection{ Direction::None };	// none by default so he doesn't move before we say so 

	
	sf::Vector2f m_playerVelocity{0.0f,-25.0f};
	sf::Vector2f m_playerLocation{ 1000.0f,SCREEN_HEIGHT - 48.0f };//sets location of sprite to bottom of screen
	
	
	float m_playerSpeed{ 2.345 }; // how fast the player moves
	//determines how fast the animation runs
	float m_frameValue{0};

	int m_currentFrame{0};
	int m_player = rand() % 3; // keeps track of which player is active
	int m_planet = rand() % 5; // keeps track of which planet is active
	int m_colorMan{ 3 };
	int m_blackHole{ 3 };
	float m_planetRadius;
	float m_playerRadius;


	
	float m_pFrameValue{ 0.0f };
	int m_pCurrentFrame{ 0 };
	int m_planetSize{ 100 };
	// determines which character will be assigned all of the initalization functions
	bool
		m_exitGame, // control exiting game
		m_canJump{ false },
		m_canWalk{ false }, //checking if walking animation should start
		m_canRun{ false }, //checking if running should be allowed
		m_canChange{ false }; // checking if players character is allowed to change

	int m_onScreen{ 1 }; // which screen is the player on
	sf::Vector2f m_gravity{0.0f, 0.8f }; //change this to alter the lenght of time of the jump




};

#endif // !GAME_HPP

