// ###########################################################################
//          Title: Oubliettes/interface.ino
//         Author: Jean-Baptiste Chaffange Koch
//    Description: Gestion et affichages des menus et de l'interface
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
//###########################################################################



// ###########################################################################
//    Ecran principal
// ###########################################################################
 void drawMain() {
  if( gb.buttons.pressed(BUTTON_A) ) { 		//avec un appui sur A je lance le jeu
    presstart = true;						//cette variable est reprise dans le LOOP et sert à indiquer que le joueur à appuyé sur A (ou autre)
    }
  gb.display.drawImage(0, 0, title);		//dessin de l'écran titre	
  gb.display.setColor(BLACK);
  gb.display.setCursor(0, 1);
  gb.display.print(VERSION); 
  gb.display.setColor(GRAY);
  gb.display.setCursor(0, 0);
  gb.display.print(VERSION); 
  if  (gb.buttons.repeat(BUTTON_MENU,1)) {	//avec un appui sur MENU j'affiche les commandes
   gb.display.clear();
   gb.display.drawImage(0, 0, howto);
    }
  if  (gb.buttons.repeat(BUTTON_B,1)) {		//avec un appui sur B j'affiche les scores
   gb.display.clear();
   gb.display.drawImage(0, 0, hscore);
   gb.display.setColor(WHITE);
   gb.display.setCursor(55, 51);
   gb.display.print(int(bestscore/25)); 
    }
  if (gb.buttons.repeat(BUTTON_B,1) && gb.buttons.repeat(BUTTON_MENU,1)) { //avec un appui sur B et Menu, je change l'état de debug et affiche le cpu/ram 
    debug = !debug;
  }
 }	//fin de drawMain()
 

 
// ###########################################################################
//    Interface en jeu
// ###########################################################################
  void drawInterface() {
  gb.display.drawImage(0, 0, front); 			//dessin de l'image de l'interface
  if (lives == 2 ) { 							//je cache les lumières en fonction des vies avec un petit rectangle noir par dessus
  gb.display.setColor(BLACK);					
  gb.display.fillRect(4,20,6,8);  
  }
  if (lives == 1 ) {
  gb.display.setColor(BLACK);
  gb.display.fillRect(4,12,6,16);  
  }
  gb.display.setColor(WHITE);							//affichage du score
  if (score >= bestscore) {gb.display.setColor(RED);}	//en rouge si highscore 
  gb.display.setCursor(66, 21);							//
  gb.display.print(int(score/25));						//pour le limiter en taille je le divise par 25 (uniquement pour l'affichage)
 }	//fin de drawInterface()

 
 
// ###########################################################################
//    Benchmark
// ###########################################################################
  void drawBenchmark() {				//affichage en sur-impression en haut à gauche
  uint16_t ram = gb.getFreeRam();
  gb.display.setColor(WHITE);
  gb.display.setCursor(1, 1);
  gb.display.print("RAM:");
  gb.display.println(ram);  
  uint8_t load = gb.getCpuLoad();
  gb.display.setCursor(1, 10);
  gb.display.print("CPU:");
  gb.display.print(load);
  gb.display.println("%");
  } //fin de drawBenchmark()

  
  
// ###########################################################################
//    Affichage du fond
// ###########################################################################
  void drawBack() {
  if (score > 15000) {
    gb.display.drawImage(0, 0, castle);  
  } else {
    gb.display.drawImage(0, 0, back); //fond
  }
  gb.display.setColor(YELLOW);  				//jauge de boost un carré coloré masquée 
  gb.display.fillRect(5,39,4,20);				//par un carré noir en fonction du boost restant
  gb.display.setColor(BLACK);					  //astuce : c'est affiché SOUS l'interface et la fiole est transparente dans l'image de l'interface.
  gb.display.fillRect(5,38,4,map(boosttime,0,100,20,0));
  } //fin de drawBack()


  
// ###########################################################################
//    Affichage du GameOver
// ########################################################################### 
void drawGameover() { 
  gb.lights.drawPixel(0, 0, RED);
  gb.display.drawImage(0, 0, gameover);
  gb.display.setColor(WHITE);
  gb.display.setCursor(33, 34);
  gb.display.print(int(lastscore/25));
  
  if( gb.buttons.pressed(BUTTON_A) ) { 										//avec un appui sur A je relance rapidement le jeu
    presstart = true;
    }
  if (gb.buttons.pressed(BUTTON_B) || gb.buttons.pressed(BUTTON_MENU)) {  	// B ou C retour au menu principal en changeant le gamestate
    gamestate = 1;
    }
} //fin de drawGameover()
  
  
  
// ###########################################################################
//    Affichage du menu Ready après la perte d'une vie
// ########################################################################### 
void drawReady() {
   gb.display.setColor(WHITE);
   gb.display.setCursor(27, 14);
   gb.display.print("Ready ?");
    if ( gb.buttons.pressed(BUTTON_A) || gb.buttons.pressed(BUTTON_B) ) { //avec un appui sur A ou B je relance rapidement le jeu
      presstart = true;
    }
    if ( gb.buttons.pressed(BUTTON_MENU)) { 								// C retourne au menu principal - exception a la methode habituelle
      gamestate = 1;
    }
} // fin de drawReady()



// ###########################################################################
//    Affichage du menu Pause
// ########################################################################### 
    void drawPause() {					//concrêtement, c'est un gros carré coloré avec PAUSE à l'intérieur
      gb.display.setColor(RED);
      gb.display.fillRect(16,15,45,17);
      gb.display.setColor(DARKGRAY);
      gb.display.drawRect(15,14,47,19);
      gb.display.setColor(BLACK);
      gb.display.fillRect(17,16,43,15);
      gb.display.setFontSize(2);
      gb.display.setColor(WHITE);
      gb.display.setCursor(20, 18);
      gb.display.print("Pause");
      gb.display.setFontSize(1);
      if ( gb.buttons.pressed(BUTTON_A) || gb.buttons.pressed(BUTTON_B) || gb.buttons.pressed(BUTTON_MENU)) { 
          presstart = true; //avec un appui sur un bouton je relance le jeu
      }
} // fin de drawPause()



// ###########################################################################
//    Affichage du menu HighScore
// ###########################################################################
 void drawHscore() {															// le menu est structuré comme le GameOver
   gb.lights.drawPixel(0, 0, YELLOW);
   gb.display.drawImage(0, 0, hscore);
   gb.display.setColor(WHITE);
   gb.display.setCursor(55, 51);
   gb.display.print(int(bestscore/25)); 
      if( gb.buttons.pressed(BUTTON_A) ) { 										//avec un appui sur A je relance rapidement le jeu
         presstart = true;
         highscore = false;
      }
      if (gb.buttons.pressed(BUTTON_B) || gb.buttons.pressed(BUTTON_MENU)) {  	// B ou C retourne au menu principal
         gamestate = 1;
         highscore = false;
      }
} // fin de drawHscore

