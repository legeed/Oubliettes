// ###########################################################################
//          Title: Oubliettes/obstacles.ino
//         Author: Jean-Baptiste Chaffange Koch
//    Description: Gestion des obstacles
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
//    VARIABLES
// ###########################################################################

#define NUM_PLATEFORMS 10 				      //10 plateforms 
MovingBox plateforms[NUM_PLATEFORMS]; 	//"tableau" avec les coordonnées des plateformes qui bougent dedans

#define NUM_BORDERS 4 					//4 "boîtes" fixes qui symbolisent les bords de la zone de jeu
Box borders[NUM_BORDERS];				//

#define NUM_BONUS 2 					    //2 "boîtes" bonus au dessus des plateformes
MovingBox bonus[NUM_BONUS];				//

byte pLWmin;
byte pLWmax; 
byte pLXmin; 
byte pLXmax; 
byte pRWmin;
byte pRWmax; 
byte pRXmin; 
byte pRXmax;



// ###########################################################################
//    Paramètres de génération des plateformes : dimensions et positions
// ###########################################################################
void setMode() { 
  mode = random(0,3);			//aléatoire sur le choix du mode
  switch (mode) {
    case 1: 					//mode 1
       pLWmin = 3; 					// 3
       pLWmax = 6; 					// 5
       pLXmin = 4; 					// 4
       pLXmax = 9; 					// 9
       pRWmin = 7; 					// 7
       pRWmax = 9; 					// 9
       pRXmin = 0; 					// 0
       pRXmax = 4;  				// 4
      break;
    case 0: 					//mode 0
       pLWmin = 3;  				// 3
       pLWmax = 5;  				// 4
       pLXmin = 4;  				// 4
       pLXmax = 8;  				// 8
       pRWmin = 7;  				// 7
       pRWmax = 9;  				// 9
       pRXmin = 1;  				// 1
       pRXmax = 4;  				// 4
      break;
    case 2: 					//mode 2
       pLWmin = 4;  				// 3
       pLWmax = 7;  				// 6
       pLXmin = 4;  				// 4
       pLXmax = 8;  				// 8
       pRWmin = 7;  				// 7
       pRWmax = 10;  				// 10
       pRXmin = 0;  				// 0
       pRXmax = 4;  				// 4
      break;
    }
  }		//fin de setMode()



// ###########################################################################
//    Génération initiale des plateformes et des bonus
// ###########################################################################                      
void initPlateforms(){
	scrollspeed = 1;								//vitesse de scrolling, fixe
	for(byte p=0; p<NUM_PLATEFORMS; p++){ 		//balayage des plateformes 0 à 9	
		plateforms[p].type = 0;
		plateforms[p].w = random(1,3)*tilesize;
		plateforms[p].xv = 0;
		plateforms[p].h = tilesize;
		plateforms[p].yv = scrollspeed;
	}
	plateforms[0].x = 4*tilesize;					//plateformes du premier rang
	plateforms[0].y = 15*tilesize;
	plateforms[1].x = 11*tilesize;
	plateforms[1].y = plateforms[0].y;
	for(byte p=2; p<NUM_PLATEFORMS; p = p+2){ 	//plateformes de gauche
		plateforms[p].x = random(4,6)*tilesize;
		plateforms[p].y = plateforms[p-2].y - 3*tilesize;
	}
	for(byte p=3; p<NUM_PLATEFORMS; p = p+2 ){ 	//plateformes de droite
		plateforms[p].x = random(11,13)*tilesize;
		plateforms[p].y = plateforms[p-2].y - 3*tilesize;
	}
	for(byte p=0; p<NUM_BONUS; p++){				//bonus
		bonus[p].type = 0; 
		bonus[p].w = tilesize;
		bonus[p].xv = 0;
		bonus[p].h = tilesize;
		bonus[p].yv = -scrollspeed; 
		bonus[p].x = 0;
		bonus[p].y = 60; 
	}
}		//fin de initPlateforms()



// ###########################################################################
//    Génération initiale des bordures
// ###########################################################################   
void initBorders(){
  //top 
  borders[0].w = 20*tilesize;		//pour rappel, tilesize = 4 pixels
  borders[0].h = 2*tilesize;
  borders[0].x = 0;
  borders[0].y = 0;
  //bottom
  borders[1].w = 20*tilesize;
  borders[1].h = 2*tilesize;
  borders[1].x = 0;
  borders[1].y = (15*tilesize);
  //left
  borders[2].w = 4*tilesize;
  borders[2].h = 22*tilesize;
  borders[2].x = 0;
  borders[2].y = 0;
  //right
  borders[3].w = 5*tilesize;
  borders[3].h = 22*tilesize;
  borders[3].x = (15*tilesize);
  borders[3].y = 0; 
}		//fin de initBorders()



// ###########################################################################
//    Scrolling initiale des plateformes et génération de celles-ci
// ###########################################################################   
void updatePlateforms() {
	for (byte b=0; b<NUM_BONUS; b++){ 						          //gestion des boites bonus
      if (bonus[b].y <= tilesize ) {bonus[b].type = 0;} 	//disparitions si elles arrivent en haut
    bonus[b].y += bonus[b].yv;  							            //deplacement des bonus, pour rappel yv = scrollspeed
    }
	for(byte p=0; p<NUM_PLATEFORMS; p = p + 2){ 			      //gestion des plateformes de gauche
    plateforms[p].yv = scrollspeed; 						          //vitesse du scrolling
    plateforms[p].y = plateforms[p].y - plateforms[p].yv; //ça monte !
      if ( plateforms[p].y <= tilesize) { 					      //si une plateforme arrive en haut de l'écran
        plateforms[p].y = 16*tilesize; 						        //elle disparait puis ré-apparait "hors champ" en bas
        plateforms[p].w = random(pLWmin,pLWmax)*tilesize; //
        plateforms[p].x = random(pLXmin,pLXmax)*tilesize; //
        if( bonus[0].type == 0 ) {							          //si il n'y a pas de bonus à gauche, j'en génère un nouveau
          bonus[0].x = random(4,9)*tilesize;				      //
          bonus[0].y = plateforms[p].y-tilesize;			    //
          bonus[0].type = random(1,3);						         //
        }
      }
    }
	for(byte p=1; p<NUM_PLATEFORMS; p = p + 2){ 			      //plateformes de droite, idem que celles de gauches
    plateforms[p].yv = scrollspeed; 						      
    plateforms[p].y = plateforms[p].y - plateforms[p].yv; 	
      if ( plateforms[p].y <= tilesize) { 					
        plateforms[p].y = 16*tilesize; 						
        plateforms[p].w = (random(pRWmin,pRWmax)-(plateforms[p-1].w/tilesize))*tilesize; 			//la formule tient compte de la position de la plateforme de gauche
        plateforms[p].x = plateforms[p-1].x+plateforms[p-1].w+(random(pRXmin,pRXmax)*tilesize); 	
        if( bonus[1].type == 0 ) {							
          bonus[1].x = random(10,14)*tilesize;;				
          bonus[1].y = plateforms[p].y-tilesize;			
          bonus[1].type = random(1,3);						
        }
      }
    }
}	//fin de updatePlateforms()
  
  
  
// ###########################################################################
//    Dessin des plateformes et des bonus
// ###########################################################################  
void drawPlateforms(){							          //en me basant sur les coordonnées des boites des plateformes je dessine les "vraies" plateformes
  for(byte d=0; d<NUM_PLATEFORMS; d++){ 		  //au lieu d'utiliser un sprite, je les dessine directement avec des fonctions simples vu qu'il n'y a pas d'animation
	gb.display.setColor(PURPLE);				        //elles sont légèrement plus grandes que leurs hitbox réelles
	gb.display.drawFastHLine(plateforms[d].x+1, plateforms[d].y,max(2,plateforms[d].w-2));
	gb.display.setColor(BEIGE);
	gb.display.drawFastHLine(plateforms[d].x+1, plateforms[d].y+1,max(2,plateforms[d].w-2));
	gb.display.setColor(GRAY);
	gb.display.drawFastHLine(plateforms[d].x+1, plateforms[d].y+2,max(2,plateforms[d].w-2));
	gb.display.drawFastHLine(plateforms[d].x+1, plateforms[d].y+3,max(2,plateforms[d].w-2));
	gb.display.setColor(DARKGRAY);
	gb.display.drawFastHLine(plateforms[d].x+1, plateforms[d].y+4,max(2,plateforms[d].w-2));
	gb.display.drawFastVLine(plateforms[d].x,plateforms[d].y+1,3);
	gb.display.drawFastVLine(plateforms[d].x+plateforms[d].w-1,plateforms[d].y+1,3);
  }
  for (byte d=0; d<NUM_BONUS; d++){					 									                          	//dessin des bonus, leur état (1 ou 2) définit le type
     if ( bonus[d].type == 1) {gb.display.drawImage(bonus[d].x, bonus[d].y-2, potred);} 	//la fonction drawImage anime toute seule mon image le cas échéant
     if ( bonus[d].type == 2) {gb.display.drawImage(bonus[d].x, bonus[d].y-2, potgreen);}	//
    }
}	//fin de drawPlateforms
