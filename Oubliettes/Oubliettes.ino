// ###########################################################################
//         
//
// 		    Title: Oubliettes.ino
//         Author: Jean-Baptiste Chaffange Koch
//    Description: A falldown-like game for Gamebuino Meta.
//        Version: 1.0.0
//           Date: 22 Mar 2018
//        License: GPLv3 (see LICENSE)
//
//    Oubliettes Copyright (C) 2018 Jean-Baptiste Chaffange Koch
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    any later version.
//		
//	  Merci à Aurelien Rodot pour le programme "Physics" de la bibliothèque GB Meta
//
// 
//###########################################################################



// ###########################################################################
//    VARIABLES
// ###########################################################################

#include <Gamebuino-Meta.h>

String VERSION = "V1"; //version du jeu

//Paramètres physiques du jeu
byte tilesize = 4;
float scrollspeed;
float gravity = 0.7;
float friction = 0.85;
float movespeed = 0.3;
float jump = 2.5;
float rebound = 0.5;

//Variables d'état diverses
boolean debug = false; 		    //active le mode "debug"
byte mode=0;  				        //change l'aléatoire dans la génération des plateformes
byte gamestate = 1; 		      //0 : jeu en cours, 1 : menu principal, 2 : gameover, 3 : nouvelle vie, 4 : pause
boolean presstart = false; 	  //le joueur a appuyé sur Start (ou autre) ?
boolean highscore = false; 	  //le joueur a fait un meilleur score !
boolean bonusscore = false;   //le joueur active le bonus de score
boolean malusscore = false;   //le joueur active le malus de score
int framecount = 0; 		      //comptage de frame, utile dans certaines fonctions
int timerinit = 0; 			      //idem
boolean gblights = true;

//variables du joueur
float score = 0;		  	//score actuel
int lastscore = 0;			//score de la dernière partie
int bestscore = 0;			//meilleur score
int lives;					    //vies restantes
int boosttime = 100; 		//en frames donc 4s

//là je définit des struct, ça permet de dire, par exemple,que toutes que les "boites" ont une valeur w x h et y
//define de type Box pour les obstacles "durs"
typedef struct {
  byte w;					  //largeur
  byte x;					  //pos X
  byte h;					  //hauteur
  byte y; 					//pos Y
  byte type; 				//etat, type, etc
} Box;

//define de type MovingBox pour les obstacles qui "bougent"
typedef struct {
  byte w; 					//largeur
  float x;					//pos X
  float xv;					//vitesse X
  byte h;					  //hauteur
  float y; 					//pos Y
  float yv; 				//vitesse Y
  byte type; 				//etat, type, etc
} MovingBox;

//define de type MovingBoxP pour le joueur
typedef struct {
  byte w; 					//largeur
  float x; 					//pos X
  float xv; 				//vitesse X
  byte h; 					//hauteur
  float y; 					//pos Y
  float yv; 				//vitesse Y
} MovingBoxP;



// ###########################################################################
//    SETUP
// ###########################################################################

void setup() {

  gb.begin();
  //gb.display.init(160, 128, ColorMode::index); 
  bestscore = gb.save.get(0); //recupère l'ancien bestscore de la sauvegarde n° 0
}



// ###########################################################################
//    LOOP
// ###########################################################################

void loop() {
   
  while(!gb.update());			//indispensable
  
  gb.display.clear();			//efface l'écran
  gb.lights.clear();			//efface les lumières

  switch (gamestate) {			//sélection l'état du jeu en fonction de la variable gamestate, c'est comme un IF mais avec un choix !
    case 0: 						//Jeu en cours
      presstart = false; 				//bon, pour le coup, presstart est lié à MENU pour la pause (d'habitude il est lié à A)
      updatePlateforms(); 			//mise à jour des positions des plateformes (dans obstacles)
      updatePlayer(); 					//mise à jour du joueur (dans player)
      drawBack();               //dessin de l'arrière plan (dans interface)
      drawPlateforms(); 				//dessin des plateforme à l'écran (dans interface)
      drawPlayer(); 				  	//dessin du joueur à l'écran (dans player)
      drawInterface(); 					//dessin des éléments d'interfaces à l'écran (dans interface)
      if (debug) {drawBenchmark();} 	//pour avoir des stats sur le CPU et la ram en cas d'appui sur B et MENU (dans interface)
      if (presstart) {gamestate = 4;} 	//->pause
      break;							//sort du switch()
    case 1: 					//Menu principal
      presstart = false;				//
      lives = 3;						//3 vies
	    setMode();						//choix aléatoire du mode de génération des plateformes (dans obstacles)
      drawMain();           //ecran principal		//dessin de l'écran d'accueil	(dans interface)
      if (presstart) {gamestate = 3;}	//->nouvelle vie
      break;							  //sort du switch()
    case 2: 					//Menu gameover
      presstart = false;				//
      if (highscore) {drawHscore(); 	//Si le score est un high score, Gameover ne s'affiche pas (dans interface)
      } else {      				        	// sinon
      drawGameover();} 				      	// affichage de l'ecran gameover (dans interface)
      if (presstart) {				      	//En cas d'appui sur A
        lives = 3;						        //Réinitialisation puis -> nouvelle vie
        setMode();					        	//
        gamestate = 3;				        //
      }									              //
      break;							            //
    case 3: 					//Nouvelle vie
      presstart = false;				//
      initBorders();				    //Initialisation des bordures (dans obstacles)
      initPlateforms();					//Initialisation des plateformes (dans obstacles)
      drawBack();						    //dessin de l'arrière plan (dans interface)
      drawInterface();					//dessin des éléments d'interfaces à l'écran (dans interface)
      drawReady(); 						  //ecran "pret ?" (dans interface)
      if (presstart) {					//si appui sur A
        initPlayer(); 					//initialisation du joueur
        gamestate = 0;					//redémarrage du jeu
      }   								      //
      break;							      //
    case 4: 					//Pause
      presstart = false;				//dans cet état, je dessine tout, mais ne remet pas les positions du joueur et des plateformes à jour
      drawBack(); 						  //dessin de l'arrière plan (dans interface)
      drawPlateforms();					//dessin des plateforme à l'écran (dans interface)
      drawPlayer(); 					  //dessin du joueur à l'écran (dans player)
      drawInterface();					//dessin des éléments d'interfaces à l'écran (dans interface)
      drawPause(); 						  //dessin du menu pause
      if (presstart) {					//retour au jeu avec un appui sur un bouton
      gamestate = 0;					  //
      }									        //
      break;							      //
  }								//fin du switch
}						//fin du loop
