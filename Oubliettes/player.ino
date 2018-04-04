// ###########################################################################
//          Title: Oubliettes/obstacles.ino
//         Author: Jean-Baptiste Chaffange Koch
//    Description: Gestion du joueur
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
//    VARIABLE
// ###########################################################################
MovingBoxP player; //création de  l'objet qui représente le joueur 



// ###########################################################################
//    Initialisation du joueur
// ###########################################################################
void initPlayer(){ 			//Balle au centre !
  player.w = tilesize;		
  player.h = tilesize;		
  player.x = tilesize*10;	
  player.y = tilesize*4;	
  boosttime = 80; 			//réinitialisation du boost
  bonusscore = false; 	// par défaut les bonus sont invalidés
  malusscore = false;		//
}



// ###########################################################################
//    Mise à jour - position, score, vies, etc.
// ###########################################################################
void updatePlayer(){					      //les fonctions de collisions sont décrites tout en bas, j'utilise des ratio pour "imiter" la physique de la balle, pas des formules de math compliquées !
  bonusscore = false; 					    // par défaut les bonus sont invalidés
  malusscore = false;					      //
//-------------------------------------------------------------
//Mouvement verticaux	    		     	   
//-------------------------------------------------------------
  player.y -= scrollspeed; 								//application du scrolling sur la position du joueur - par défaut il monte.
  if (!playerLanding()){player.yv = player.yv + gravity;} //si le joueur n'est pas sur une plateforme, alors il tombe. sa vitesse verticale est augmentée, la gravité "compense" l'application du scrollspeed
  player.yv *= friction; 								      //friction, pour eviter les rebond et bug
  if(gb.buttons.timeHeld(BUTTON_UP) > 0 & gb.buttons.timeHeld(BUTTON_UP) < 5 && playerLanding()) {//Saut, conditioné par l'appui sur Haut et le fait d'être sur une plateforme
    player.yv -= jump; 									//la vitesse verticale est modifiée, le joueur remontera brutalement
    boosttime = max(0,boosttime - 1); 					//sauter consomme du boost
  }
  player.y = player.y + player.yv; 						//application de la vitesse verticale
  if(playerCollision()){								      //test de collision avec une plateforme
    player.y = player.y - player.yv; 				  //en cas de collision j'annule le déplacement 
    player.yv *= (-1)*rebound; 							  //je "ralenti" le joueur et inverse son sens de déplacement
    //gb.lights.drawPixel(0, 2, WHITE);
    //gb.lights.drawPixel(1, 2, WHITE);
  }
  if (player.y >= 15*tilesize) {						//test de collision avec le slime
    player.y -= player.yv; 								  //en cas de collision j'annule le déplacement
    player.yv *= (-1)*(rebound/2); 					//je "ralenti" beaucoup le joueur et inverse son sens de déplacement
    malusscore = true; 									    //rester dans le slime diminue le score
  }
//-------------------------------------------------------------
//  Mouvement horizontaux
//-------------------------------------------------------------
  player.xv *= friction; 														//friction pour ralentir le joueur
  if (gb.buttons.repeat(BUTTON_A, 1) && boosttime > 0 && playerLanding() ){ 	//Turbo ! conditoné sur l'appui d'un bouton et la présence sur une plateforme
    player.xv *= 1.30;															//multiplie la vitesse horizontale
    boosttime = max(0,boosttime - 2); 							//consomme du boost
  }
  if (gb.buttons.repeat(BUTTON_B, 1) && boosttime > 0 && playerLanding() ){ 	//Frein ! conditoné sur l'appui d'un bouton et la présence sur une plateforme
    player.xv *= 0.60;															//divise la vitesse horizontale
    boosttime = max(0,boosttime - 2); 							//consomme du boost
  }  
  if(gb.buttons.repeat(BUTTON_RIGHT, 1)){player.xv += movespeed;}				//Appui droite, modifie la vitesse horizontale
  if(gb.buttons.repeat(BUTTON_LEFT,1)){player.xv -= movespeed;}					//Appui gauche, modifie la vitesse horizontale
  player.x += player.xv; 														//applique la vitesse horizontale
  if(playerCollision() || playerBorder()){					//test de collision avec une bordure
    player.x -= player.xv; 													//en cas de collision j'annule le déplacement
    player.xv *= (-1)*rebound; 											//je "ralenti" le joueur et inverse son sens de déplacement
  }
//-------------------------------------------------------------
//  Bonus score
//-------------------------------------------------------------
  if ( (player.y < (4*tilesize)) && player.y > tilesize  ) { //le joueur est en "haut" de l'écran, il profite d'un bonus au score
    bonusscore = true;
  }
//-------------------------------------------------------------
//  Ramassage d'un bonus
//-------------------------------------------------------------
 for(byte i=0; i<NUM_BONUS; i++){				//test la collision entre une boite "bonus" et la boite "player"
    if(gb.collideRectRect(player.x, player.y, player.w, player.h, bonus[i].x, bonus[i].y, bonus[i].w, bonus[i].h)){
      switch (bonus[i].type) {					//si il y a collision, je détermine le type du bonus et applique les conséquences au score et/ou au boosttime
        case 1: 									      //bonus type 1 c'est du score
          score += 250;
          gb.lights.fill(BROWN);
        break;
        case 2: 									    //bonus type 2 c'est du boost (et un peu de score)
          boosttime = min(boosttime+10,100);
          score += 50;
          gb.lights.fill(BROWN);
        break;
      }
      bonus[i].type = 0; 							//le bonus est ramassé, son état passe à 0, il ne sera plus affiché et régénéré à la prochain apparition de plateforme
    }
 }
//-------------------------------------------------------------
//  Calcul du score
//-------------------------------------------------------------
  score++; 											              //à chaque frame, le score prends +1 (Pour l'affichage, toutefois, je le divise par 25 sinon c'est trop grand)
  if (bonusscore) {
    score++;                                  //si bonusscore est activé, il prends +1 supplémentaire
    gb.lights.drawPixel(0, 0, RED);
    gb.lights.drawPixel(1, 0, RED);}						      
  if (malusscore) {                           //si malusscore est activé, il diminue
    score = max(score-25,1);
    gb.lights.drawPixel(0, 4, GREEN);
    gb.lights.drawPixel(1, 4, GREEN);}	
//-------------------------------------------------------------
// Gestion des vies, du score et du gameover    
//-------------------------------------------------------------
  if (player.y < tilesize) {						//le joueur arrive en haut de l'écran
    gamestate = 3; 									  //le mode du jeu passe en "nouvelle vie"
    lives = max(lives - 1,0);						//le joueur perds une vie
    player.y = tilesize;							//le Y du joueur est fixé provisoirement 
  }
  if (lives == 0) { 								  //si le joueur n'a plus de vie 
     gamestate = 2;									  //le mode du jeu passe en "gameover"
     lastscore = score;								//lastscore sauvegarde le score de la partie venant de se terminer
    if (score > bestscore) {					//si le score est plus grand que le highscore
        bestscore = score;						//il est sauvegardé
        gb.save.set(0,bestscore);			//dans la save 0
        highscore = true;							
     }												
     score = 0;										    //la partie en cours est terminée, le score est réinitialisé
  }													
//-------------------------------------------------------------
//  Pause !									   
//-------------------------------------------------------------
  if (gb.buttons.pressed(BUTTON_MENU)) { 			//le bouton MENU est pressé
    presstart = true;								
  }												
    
}	//fin de updatePlayer()



// ###########################################################################
//    Dessin de la balle/joueur
// ###########################################################################
void drawPlayer(){  			// ce n'est pas un sprite, mais un dessin direct à l'aide de fonctions simple
  gb.display.setColor(RED);
  gb.display.fillCircle(player.x+2, player.y+2, 2);
  gb.display.setColor(ORANGE);
  gb.display.fillRect(player.x+1, player.y+1, 3, 3); 
  gb.display.setColor(YELLOW);
  gb.display.fillRect(player.x+1, player.y+1, 2, 2);
  /*dessine la "hitbox" de la balle 
  gb.display.setColor(GREEN);
  gb.display.fillRect(player.x, player.y, player.w, player.h);*/
  //notez que le hitbox de la balle est un carré de 4x4pixel, son "dessin" est donc plus grand que sa hitbox réelle !
} //fin de drawPlayer()



// ###########################################################################
//    Détection de collision avec une plateforme
// ###########################################################################
boolean playerCollision(){ 				//test de la superposition des coordonnées des boites des plateformes avec la boite du joueur
  for(byte i=0; i<NUM_PLATEFORMS; i++){
    if(gb.collideRectRect(player.x, player.y, player.w, player.h, plateforms[i].x, plateforms[i].y, plateforms[i].w, plateforms[i].h)){
      return true;						//en cas de test positif, la fonction renvoie TRUE
    }									
  }										
  return false;							//ou FALSE
}	//fin de playerCollision()



// ###########################################################################
//    Détection de collision sur une plateforme
// ###########################################################################
boolean playerLanding(){ 				//test de la superposition des coordonnées de boites fictives situées sur les plateformes, avec la boite du joueur
  for(byte i=0; i<NUM_PLATEFORMS; i++){
    if(gb.collideRectRect(player.x, player.y, player.w, player.h, plateforms[i].x+1, plateforms[i].y-1, plateforms[i].w-2, 1)){
    return true;						//en cas de test positif, la fonction renvoie true
    }									  
  }										  
  return false;						//ou False
 }	//fin de playerLanding()
 
 
// ###########################################################################
//    Détection de collision avec une bordure
// ###########################################################################
boolean playerBorder(){ 				// le principe est toujours le même  
  for(byte i=0; i<NUM_BORDERS; i++){	//
    if(gb.collideRectRect(player.x, player.y, player.w, player.h, borders[i].x, borders[i].y, borders[i].w, borders[i].h)){
      return true;						
    }									
  }										
  return false;							
}	//fin de playerBorder()
