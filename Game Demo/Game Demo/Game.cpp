/// <summary>
/// @author Harvey McNevin
/// @date February 2023
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ SCREEN_WIDTH, SCREEN_HEIGHT, 32U }, "SFML Game" },
	m_exitGame{false} //when true game will exit
{	
	
	setUpBackGround(); // load backrgound textures
	//setupFontAndText(); // load font 
	setupSprite(); // load texture


}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (sf::Event::KeyReleased == newEvent.type)
		{
				m_canWalk = false;
				m_playerDirection = Direction::None;
			
			
		}	
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
	if (sf::Keyboard::LShift == t_event.key.code)
	{
		if (!m_canJump) // stops player from speeding up in the air
		{
			m_canRun = !m_canRun;
		}
	}
	if (sf::Keyboard::Space == t_event.key.code)
	{
		m_canJump = true;

	}
	if (sf::Keyboard::K == t_event.key.code)
	{
		m_player = setPlayer();
	
	}
	
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();
	}

	checkDirection();
	checkBounds();
	checkCollision();
	whereAmI();
	animatePlanet();

	//if player is trying to walk and is not trying to jump
	if (m_canWalk && !m_canJump)
	{
		walkAnimation();
		move();
	}
		
	

	//will set idle animation to run whenever the player isnt moving
	if (!m_canWalk && !m_canJump)
	{
		idleAnimation();
	}

	//will run whenever the player is walkin, toggle run is on and the player is not currently jumping
	if (m_canWalk && m_canRun && !m_canJump)
	{
		runningAnimation();
		running();
	}

	//if player is trying to jump
	if (m_canJump)
	{
		jumping();
		jumpingAnimation();
	
	}
	
	if (m_canWalk && m_canJump)
	{
		move();
	}
	// if player is running and jumping, jump will keep speed from run 
	if (m_canRun && m_canJump)
	{
		running();
	}

	
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
		m_window.clear(sf::Color::White);
		m_window.draw(m_backGroundSprite);
		m_window.draw(m_playerSprite);
		m_window.draw(m_planetSprite);
		m_window.display();
}

float vectorLength(sf::Vector2f t_pos)
{
	return std::sqrtf(t_pos.x * t_pos.x + t_pos.y * t_pos.y);
}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{

	m_playerSprite.setTexture(m_playerIdleTexture[m_player]);
	m_playerSprite.setTextureRect(sf::IntRect{ 0,0,48,48 });
	m_playerSprite.setPosition(m_playerLocation);
	m_playerSprite.setScale(2.0f, 2.0f);
	m_playerSprite.setOrigin(24.0f, 24.0f); //half the sprites dimensions == middle of sprite

	setUpGraveRobber();
	setUpWoodCutter();
	setUpSteamMan();
	setUpColorMan();
	setUpPlanet();

}



/// <summary>
/// loads the players texture depending on what character they chose
/// </summary>
void Game::setUpGraveRobber()
{
	//GraveRobber

	if (!m_playerIdleTexture[0].loadFromFile("ASSETS\\IMAGES\\GraveRobber\\GraveRobber_idle.png"))
	{
		std::cout << "problem loading grave robber default sprite" << std::endl;
	}


	if (!m_playerWalkTexture[0].loadFromFile("ASSETS\\IMAGES\\GraveRobber\\GraveRobber_Walk.png"))
	{
		std::cout << "problem loading grave robber default texture" << std::endl;
	}

	if (!m_playerRunTexture[0].loadFromFile("ASSETS\\IMAGES\\GraveRobber\\GraveRobber_Run.png"))
	{
		std::cout << "problem loading grave robber run texture" << std::endl;
	}

	if (!m_playerJumpTexture[0].loadFromFile("ASSETS\\IMAGES\\GraveRobber\\GraveRobber_jump.png"))
	{
		std::cout << "problem loading grave robber jump texture" << std::endl;
	}



}
void Game::setUpWoodCutter()
{

	if (!m_playerIdleTexture[1].loadFromFile("ASSETS\\IMAGES\\WoodCutter\\WoodCutter_idle.png"))
	{
		std::cout << "problem loading woodcutter default sprite" << std::endl;
	}

	if (!m_playerWalkTexture[1].loadFromFile("ASSETS\\IMAGES\\WoodCutter\\Woodcutter_Walk.png"))
	{
		std::cout << "problem loading grave robber default texture" << std::endl;
	}

	if (!m_playerRunTexture[1].loadFromFile("ASSETS\\IMAGES\\WoodCutter\\Woodcutter_Run.png"))
	{
		std::cout << "problem loading woodcutter run texture" << std::endl;
	}

	if (!m_playerJumpTexture[1].loadFromFile("ASSETS\\IMAGES\\WoodCutter\\Woodcutter_jump.png"))
	{
		std::cout << "problem loading woodcutter jump texture" << std::endl;
	}



}
void Game::setUpSteamMan()
{

	if (!m_playerIdleTexture[2].loadFromFile("ASSETS\\IMAGES\\SteamMan\\SteamMan_idle.png"))
	{
		std::cout << "problem loading steamman default sprite" << std::endl;
	}

	if (!m_playerWalkTexture[2].loadFromFile("ASSETS\\IMAGES\\SteamMan\\SteamMan_Walk.png"))
	{
		std::cout << "problem loading steamman default texture" << std::endl;
	}

	if (!m_playerRunTexture[2].loadFromFile("ASSETS\\IMAGES\\SteamMan\\SteamMan_Run.png"))
	{
		std::cout << "problem loading steamman run texture" << std::endl;
	}

	if (!m_playerJumpTexture[2].loadFromFile("ASSETS\\IMAGES\\SteamMan\\SteamMan_jump.png"))
	{
		std::cout << "problem loading steamman jump texture" << std::endl;
	}



}

void Game::setUpColorMan()
{
	if (!m_playerIdleTexture[3].loadFromFile("ASSETS\\IMAGES\\ColorMan\\ColorManIdle.png"))
	{
		std::cout << "problem loading colorGuy default sprite" << std::endl;
	}

	if (!m_playerWalkTexture[3].loadFromFile("ASSETS\\IMAGES\\ColorMan\\ColorManWalk.png"))
	{
		std::cout << "problem loading colorGuy default texture" << std::endl;
	}

	if (!m_playerRunTexture[3].loadFromFile("ASSETS\\IMAGES\\ColorMan\\ColorManRun.png"))
	{
		std::cout << "problem loading colorGuy run texture" << std::endl;
	}

	if (!m_playerJumpTexture[3].loadFromFile("ASSETS\\IMAGES\\ColorMan\\ColorManJump.png"))
	{
		std::cout << "problem loading colorGuy jump texture" << std::endl;
	}
		

}

void Game::setUpPlanet()
{
	for (int i = 0; i < NUMBER_OF_PLANETS; i++)
	{
		switch (i)
		{
		case 0:
			if (!m_planetTexture[i].loadFromFile("ASSETS\\IMAGES\\Planets\\earth.png"))
				{
					std::cout << "problem loading planet texture" << std::endl;
				}
		break;
		case 1:
			if (!m_planetTexture[i].loadFromFile("ASSETS\\IMAGES\\Planets\\desertPlanet.png"))
				{
					std::cout << "problem loading planet texture" << std::endl;
				}
		break;
		case 2:
			if (!m_planetTexture[i].loadFromFile("ASSETS\\IMAGES\\Planets\\islands.png"))
				{
					std::cout << "problem loading planet texture" << std::endl;
				}
		break;
		case 3:
			if (!m_planetTexture[i].loadFromFile("ASSETS\\IMAGES\\Planets\\star.png"))
				{
					std::cout << "problem loading planet texture" << std::endl;
				}
		break;
		case 4:
			if (!m_planetTexture[i].loadFromFile("ASSETS\\IMAGES\\Planets\\moons.png"))
				{
					std::cout << "problem loading planet texture" << std::endl;
				}
		break;
		}
	}
	m_planetSprite.setTexture(m_planetTexture[m_planet]);
	m_planetSprite.setScale(1.3f, 1.3f);
	m_planetSprite.setTextureRect(sf::IntRect( 0.0f, 0.0f, 100.0f, 100.0f ));
	m_planetSprite.setPosition(200.0f, 200.0f);
}

int Game::setPlayer()
{
	srand(time(nullptr));
	int num = (rand() % NUMBER_OF_PLAYERS);
	
	while(num == m_player)
	{
		num = (rand() % NUMBER_OF_PLAYERS);
	}
	
	//smaller player sprite is assigned number 3, need to have the origin brought higher
	if (num == m_colorMan)
	{
		m_playerSprite.setOrigin(24.0f, 16.0f);
	}
	else
	{
		m_playerSprite.setOrigin(24.0f, 24.0f);
	}
	//std::cout << num << " ";
	return num;
	
}

int Game::setPlanet()
{	
	srand(time(nullptr));
	int num = (rand() % NUMBER_OF_PLANETS);

	int localWidth = SCREEN_WIDTH - 100;
	int localHeight = SCREEN_HEIGHT -300;


	while (num == m_planet)
	{
		num = (rand() % NUMBER_OF_PLANETS);
	}

	if (num == m_blackHole)
	{
		m_planetSize = 200;
		m_planetSprite.setOrigin(50.0f, 50.0f);
		
	}
	else
	{
		m_planetSize = 100;
		m_planetSprite.setOrigin(0.0f, 0.0f);
	}

	// random planet position within the screen space
	sf::Vector2f position{sf::Vector2f(rand() % localWidth,rand() % localHeight + 200)};
	m_planetSprite.setPosition(position);

	return num;
}

void Game::checkCollision()
{
	sf::FloatRect planet = m_planetSprite.getGlobalBounds();
	sf::FloatRect player = m_playerSprite.getGlobalBounds();
		if (player.intersects(planet))
		{
			std::cout << "boom";
		}
}

void Game::animatePlanet()
{
	float increment{ 0.175f }; // alter to speed up animation
	int oldFrame = m_pFrameValue;
	m_pFrameValue += increment;
	m_planetSprite.setTexture(m_planetTexture[m_planet]);
	m_pCurrentFrame = (int)m_pFrameValue;
	m_pCurrentFrame %= 35;
	if (oldFrame != m_pCurrentFrame)
	{
		m_planetSprite.setTextureRect(sf::IntRect{ m_pCurrentFrame * m_planetSize,0,m_planetSize,m_planetSize });
	}

}

void Game::idleAnimation()
{
	float increment{ 0.10f }; // adjust to change speed of animation

	m_playerSprite.setTexture(m_playerIdleTexture[m_player]);
	int oldFrame = m_frameValue;

	m_frameValue += increment;
	m_currentFrame = (int)m_frameValue;
	if (m_player == m_colorMan)
	{
		m_currentFrame %= 10;// modulus by the number of seperate sprites in the sprite sheet
	}
	else
	{
		m_currentFrame %= 4;// modulus by the number of seperate sprites in the sprite sheet
	}
	if (oldFrame != m_currentFrame)
	{
		m_playerSprite.setTextureRect(sf::IntRect{ m_currentFrame * 48, 0, 48, 48 });
	}


}

/// <summary>
/// sets up background texture and assigns to sprite depending on location
/// </summary>
void Game::setUpBackGround()
{
	for (int i = 0; i < NUMBER_OF_BACKGROUNDS;i++)
	{
		if (!m_backGroundTexture[0].loadFromFile("ASSETS\\IMAGES\\cloudsBackground.png"))
		{
			std::cout << "Problem loading default background texture" << std::endl;
		}

		if (!m_backGroundTexture[1].loadFromFile("ASSETS\\IMAGES\\DesertDay.png"))
		{
			std::cout << "Problem loading desert day texture" << std::endl;
		}

		if (!m_backGroundTexture[2].loadFromFile("ASSETS\\IMAGES\\finalDay.png"))
		{
			std::cout << "Problem loading forest day texture" << std::endl;
		}
		if (!m_backGroundTexture[3].loadFromFile("ASSETS\\IMAGES\\DesertNight.png"))
		{
			std::cout << "Problem loading desert night texture" << std::endl;
		}
		if (!m_backGroundTexture[4].loadFromFile("ASSETS\\IMAGES\\finalNight.png"))
		{
			std::cout << "Problem loading forest night texture" << std::endl;
		}
	}
	
	m_backGroundSprite.setScale(3.0f, 2.4f);
}

/// <summary>
/// checks what direction the player is heading
/// </summary>
void Game::checkDirection()
{
	m_playerDirection = Direction::None;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_playerDirection = Direction::Left;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_playerDirection = Direction::Right;
	}
	if (m_playerDirection != Direction::None)
	{
		m_canWalk = true;
	}
}
/// <summary>
/// animates the walking sprite sheet
/// </summary>
void Game::walkAnimation()
{	
	float increment{ 0.18 }; // adjust to change speed of animation
	m_playerSprite.setTexture(m_playerWalkTexture[m_player]);
	int oldFrame = m_frameValue;

	
	m_frameValue += increment;
	m_currentFrame = (int)m_frameValue;
	if (m_player == m_colorMan)
	{
		m_currentFrame %= 8;
	}
	else
	{
		m_currentFrame %= 6;// modulus by the number of seperate sprites in the sprite sheet
	}
	if (oldFrame != m_currentFrame)
	{
		m_playerSprite.setTextureRect(sf::IntRect{ m_currentFrame * 48, 0, 48, 48 });
	}

}

void Game::move()
{
	sf::Vector2f playerVelocity = (sf::Vector2f{ 0.0f,0.0f });

	m_playerSpeed = 2.345f; //without this, the run function will permenantly alter the players speed

	switch (m_playerDirection)
	{
	case Direction::None:
		break;
	case Direction::Left:
		playerVelocity.x -= m_playerSpeed;
		m_playerSprite.setScale(-2.0f, 2.0f);
		break;
	case Direction::Right:
		playerVelocity.x = m_playerSpeed;
		m_playerSprite.setScale(2.0f, 2.0f);
		break;
	default:
		break;
	}
	m_playerLocation += playerVelocity;
	m_playerSprite.setPosition(m_playerLocation);

}
/// <summary>
/// animates the running sprite sheet
/// </summary>
void Game::runningAnimation()
{

	float runIncrement{ 0.18f };

	m_playerSprite.setTexture(m_playerRunTexture[m_player]);
	int oldFrame = m_frameValue;
	m_frameValue += runIncrement;
	m_currentFrame = (int)m_frameValue;
	if (m_player == m_colorMan)
	{
		m_currentFrame %= 8;
	}
	else
	{
		m_currentFrame %= 6;// modulus by the number of seperate sprites in the sprite sheet
	}
	if (oldFrame != m_currentFrame)
	{
		m_playerSprite.setTextureRect(sf::IntRect{ m_currentFrame * 48, 0, 48, 48 });
	}


}

void Game::running()
{
	sf::Vector2f playerVelocity = (sf::Vector2f{ 0.0f,0.0f });
	m_playerSpeed = 7.5f;
	switch (m_playerDirection)
	{
	case Direction::None:
		break;

	case Direction::Left:
		playerVelocity.x -= m_playerSpeed;
		m_playerSprite.setScale(-2.0f, 2.0f);
		break;
	case Direction::Right:
		playerVelocity.x = m_playerSpeed;
		m_playerSprite.setScale(2.0f, 2.0f);
		break;
	default:
		break;
	}
	m_playerLocation += playerVelocity;
	m_playerSprite.setPosition(m_playerLocation);
}
/// <summary>
/// animates the jumping spritesheet
/// </summary>
void Game::jumpingAnimation()
{
	
	float jumpIncrement{ 0.15f }; // change to increase animation speed

	m_playerSprite.setTexture(m_playerJumpTexture[m_player]);

	int oldFrame = m_frameValue;
	m_frameValue += jumpIncrement;
	m_currentFrame = (int)m_frameValue;
	m_currentFrame %= 6;// modulus by the number of seperate sprites in the sprite sheet
	if (oldFrame != m_currentFrame)
	{
		m_playerSprite.setTextureRect(sf::IntRect{ m_currentFrame * 48, 0, 48, 48 });
		
	}
}

void Game::jumping()
{		
	
	m_playerLocation += m_playerVelocity;
	m_playerSprite.setPosition(m_playerLocation);
	m_playerVelocity += m_gravity;


	
	//boundary checking
	if (m_playerLocation.y > SCREEN_HEIGHT - 48.0f)
	{
		m_playerLocation.y = SCREEN_HEIGHT - 48.0f;
		m_playerVelocity = sf::Vector2f(0.0f, -25.0f);
		m_canJump = false;
	}
	
}

void Game::checkBounds()
{
	if (m_playerLocation.x > SCREEN_WIDTH)
	{
		m_planet = setPlanet(); // changes planet when character moves to another screen
		m_onScreen ++;
		m_playerLocation.x = 0.0f;	
	}

	if (m_playerLocation.x < 0.0f)
	{
    	m_onScreen--;
		m_playerLocation.x = SCREEN_WIDTH;
		m_planet = setPlanet(); // changes planet when character moves to another screen
	}

	switch (m_onScreen)
	{
	case 1:
		m_backGroundSprite.setTexture(m_backGroundTexture[0]);
		break;
	case 2:
		m_backGroundSprite.setTexture(m_backGroundTexture[1]);
		break;
	case 3:
		m_backGroundSprite.setTexture(m_backGroundTexture[2]);
		break;
	case 4:
		m_backGroundSprite.setTexture(m_backGroundTexture[3]);
		break;
	case 5:
		m_backGroundSprite.setTexture(m_backGroundTexture[4]);
		break;
	}	
}



void Game::whereAmI()
{

	if (m_onScreen > NUMBER_OF_BACKGROUNDS)
	{
		m_onScreen = 1;
	}
	if (m_onScreen < 1)
	{
		m_onScreen = NUMBER_OF_BACKGROUNDS;
	}
}




