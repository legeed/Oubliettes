#define NUM_PLATEFORMS 10 //10 plateforms : visibles + invisibles pour le scrolling 
MovingBox plateforms[NUM_PLATEFORMS]; //tableau avec les coordonnées des plateformes qui bougent dedans

#define NUM_BORDERS 4 //4 "bôites" fixes qui symbolisent les bords de la zone de jeu
Box borders[NUM_BORDERS];

#define NUM_BONUS 2 //2 "boîtes" bonus au dessus des plateformes elle apparaissent aléatoirement, 2 au maximum
MovingBox bonus[NUM_BONUS];

////////////////////////////////////////////////////////////////////////////////


  void setMode() { //règle les paramètres de mode 
  mode = random(0,3);
  switch (mode) {
    case 1: //mode 1
      pLWmin = 3; // 3
      pLWmax = 6; // 5
      pLXmin = 4; // 4
      pLXmax = 9; // 9
      pRWmin = 7; // 7
      pRWmax = 9; // 9
      pRXmin = 0; // 0
      pRXmax = 4; // 4
      break;
    case 0: //mode 0
      pLWmin = 3; // 3
      pLWmax = 5; // 4
      pLXmin = 4; // 4
      pLXmax = 8; // 8
      pRWmin = 7; // 7
      pRWmax = 9; // 9
      pRXmin = 1; // 1
      pRXmax = 4; // 4
      break;
    case 2: //mode 2
      pLWmin = 4; // 3
      pLWmax = 7; // 6
      pLXmin = 4; // 4
      pLXmax = 8; // 8
      pRWmin = 7; // 7
      pRWmax = 10; // 10
      pRXmin = 0; // 0
      pRXmax = 4; // 4
      break;
    }
  }

////////////////////////////////////////////////////////////////////////////////


  //fonction qui initialise mes plateformes (et les bonus) en début de partie
                        // je le fais avec des boucles, elles sont numérotées de 0 à 9
                        //0 1
                        //2 3
                        //4 5
                        //6 7
                        //8 9                        
 void initPlateforms(){
  //positions de départs, avec un tout petit peu d'aléatoire histoire de dire ...
  //je pourrais faire ça intelligemment avec des boucles for, ou pas :D

  scrollspeed = 1;
  
  for(byte p=0; p<NUM_PLATEFORMS; p++){ 
  plateforms[p].type = 0;
  plateforms[p].w = random(1,3)*tilesize;
  plateforms[p].xv = 0;
  plateforms[p].h = tilesize;
  plateforms[p].yv = scrollspeed;
  }

  plateforms[0].x = 4*tilesize;
  plateforms[0].y = 15*tilesize;
  plateforms[1].x = 11*tilesize;
  plateforms[1].y = plateforms[0].y;
   
  for(byte p=2; p<NUM_PLATEFORMS; p = p+2){ 
    plateforms[p].x = random(4,6)*tilesize;
    plateforms[p].y = plateforms[p-2].y - 3*tilesize;
  }

  for(byte p=3; p<NUM_PLATEFORMS; p = p+2 ){ 
    plateforms[p].x = random(11,13)*tilesize;
    plateforms[p].y = plateforms[p-2].y - 3*tilesize;
  }

  for(byte p=0; p<NUM_BONUS; p++){
  bonus[p].type = 0; //à 0 elles ne sont pas affichées, 1 c'est du score, 2 c'est du boost
  bonus[p].w = tilesize;
  bonus[p].xv = 0;
  bonus[p].h = tilesize;
  bonus[p].yv = -scrollspeed; 
  bonus[p].x = 0;
  bonus[p].y = 60; 
  }
  
}  


////////////////////////////////////////////////////////////////////////////////


//même genre mais pour les 4 "boites" qui entourent l'écran
void initBorders(){
  //top 
  borders[0].w = 20*tilesize;
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
}

////////////////////////////////////////////////////////////////////////////////


//scrolling Y des plateformes et génération de nouvelles en cas de sortie de l'écran
void updatePlateforms() {
    
    for (byte b=0; b<NUM_BONUS; b++){ //boites bonus
      if (bonus[b].y <= tilesize ) { //je les fais disparaitre si elles arrivent en haut, j'en générerai une nouvelle avec une plateforme
        bonus[b].type = 0;
      }
       bonus[b].y += bonus[b].yv;  //hop ça monte !
    }
  
    for(byte p=0; p<NUM_PLATEFORMS; p = p + 2){ //plateformes de gauche
    plateforms[p].yv = scrollspeed; //vitesse du scrolling
    plateforms[p].y = plateforms[p].y - plateforms[p].yv; //et hop ça monte !
      if ( plateforms[p].y <= tilesize) { //génération d'une nvle plateforme en bas quand elle arrive dans la lave en haut
        plateforms[p].y = 16*tilesize; //elle apparait "hors champ" en bas
        plateforms[p].w = random(pLWmin,pLWmax)*tilesize; //3 et 5 par défaut
        plateforms[p].x = random(pLXmin,pLXmax)*tilesize; //4 et 8 par défaut
        if( bonus[0].type == 0 ) {
          bonus[0].x = random(4,9)*tilesize;
          bonus[0].y = plateforms[p].y-tilesize;
          bonus[0].type = random(1,3);
        }
        
       }
    }
	
    for(byte p=1; p<NUM_PLATEFORMS; p = p + 2){ //plateformes de droite, en incluant un peu de math pour être sûr qu'il reste au moins un trou !
    plateforms[p].yv = scrollspeed; //vitesse du scrolling
    plateforms[p].y = plateforms[p].y - plateforms[p].yv; //et hop ça monte en suivant les copines de gauche !
      if ( plateforms[p].y <= tilesize) { //génération d'une nvle plateforme en bas quand une arrive dans la lave en haut
        plateforms[p].y = 16*tilesize; //elle apparait "hors champ" en bas
        plateforms[p].w = (random(pRWmin,pRWmax)-(plateforms[p-1].w/tilesize))*tilesize; //7 et 9 pour w min max
        plateforms[p].x = plateforms[p-1].x+plateforms[p-1].w+(random(pRXmin,pRXmax)*tilesize); //0 et 4 pour x min max
        if( bonus[1].type == 0 ) {
          bonus[1].x = random(10,14)*tilesize;;
          bonus[1].y = plateforms[p].y-tilesize;
          bonus[1].type = random(1,3);
        }
       }
     }

    
  }
  
////////////////////////////////////////////////////////////////////////////////



//dessin des plateforms
  void drawPlateforms(){
  
  for(byte d=0; d<NUM_PLATEFORMS; d++){ //au lieu d'utiliser un sprite, je les dessine directement avec des fonctions simples
  gb.display.setColor(PURPLE);
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
    
  for (byte d=0; d<NUM_BONUS; d++){ //dessin des bonus, ssi leur état est à 1 ou 2
     if ( bonus[d].type == 1) {
     gb.display.drawImage(bonus[d].x, bonus[d].y-2, potred);
     }
     if ( bonus[d].type == 2) {
     gb.display.drawImage(bonus[d].x, bonus[d].y-2, potgreen);
     }
    }
  }

////////////////////////////////////////////////////////////////////////////////

