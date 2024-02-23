#include "Game.h"
#include "Actor.h"
#include "PlayerInput.h"
#include "Player.h"
#include "Enemy.h"

Game game; //global variable, static memory allocated.

Game::Game() {
	actors[0] = new Player(Vector(100.f, 100.f)); //Heap allocated actor. Player is inheriting from Actor
	player = actors[0]; //set player to newly allocated actor.
	actors[1] = new Enemy(Vector(600.f, 250.f));
}

void Game::Update() {//update whole game before drawing something, common way of doing things.
	UpdateInput(player->position, speed); //player->position == (*player).position
	for (int i = 0; i < maxActors; i++) {
		if (actors[i] != nullptr) { //Check if not nullpointer, since those are illegal to reference.
			actors[i]->Update(); //since this is marked virtual, it wont just call update on the actor, but also check if the actor has overriden the function(as would be the case with an inherited class).
		}
	}
}

void Game::Render() {
	//draw all actors
	for (int i = 0; i < maxActors; i++) {
		if (actors[i] != nullptr) { //Check if not nullpointer, since those are illegal to reference.
			actors[i]->Draw(); //((*actors[i]).draw()). 
		}
	}
}