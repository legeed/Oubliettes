#define NUM_PLATEFORMS 10 //10 plateforms : 6 visibles + 4 invisibles pour le scrolling
MovingBox plateforms[NUM_PLATEFORMS]; //tableau avec les plateformes qui bougent dedans

#define NUM_BORDERS 4 //4 "bôites" fixes qui symbolise les bords de la zone de jeu
Box borders[NUM_BORDERS]; //tableau avec les plateformes qui bougent dedans



 //fonction qui initialise mes plateforme en début de partie
                        // je le fais avec des boucles, elles sont numérotées de 0 à 9
                        //0 1
                        //2 3
                        //4 5
                        //6 7
                        //8 9
                        
 void initPlateforms(){
  //je pourrais faire ça intelligemment avec des boucles for, ou pas :D
  
  for(byte p=0; p<NUM_PLATEFORMS; p++){ 
  plateforms[p].type = 0;
  plateforms[p].w = random(3,5)*tilesize;
  plateforms[p].xv = 0;
  plateforms[p].h = tilesize;
  plateforms[p].yv = scrollspeed;
  }

  plateforms[0].x = 4*tilesize;
  plateforms[0].y = 15*tilesize;
  plateforms[1].x = 11*tilesize;
  plateforms[1].y = plateforms[0].y;
   
  for(byte p=2; p<NUM_PLATEFORMS; p = p+2){ 
    plateforms[p].x = random(4,8)*tilesize;
    plateforms[p].y = plateforms[p-2].y - 3*tilesize;
  }

  for(byte p=3; p<NUM_PLATEFORMS; p = p+2 ){ 
    plateforms[p].x = random(11,14)*tilesize;
    plateforms[p].y = plateforms[p-2].y - 3*tilesize;
  }
}  


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

//scrolling Y des plateformes et génération de nouvelles en cas de sortie de l'écran
void updatePlateforms() {
    for(byte p=0; p<NUM_PLATEFORMS; p++){
    plateforms[p].yv = scrollspeed; //vitesse du scrolling
    plateforms[p].y = plateforms[p].y - plateforms[p].yv; //et hop ça monte !
      if ( plateforms[p].y <= tilesize) { //génération d'une nvle plateforme en bas quand elle arrive dans la lave en haut
        plateforms[p].y = 16*tilesize;
        plateforms[p].w = random(3,5)*tilesize;
        plateforms[p].x = random(4,11)*tilesize;
      }
     }
  }
  

//dessin des plateforms
  void drawPlateforms(){
  for(byte d=0; d<NUM_PLATEFORMS; d++){
  gb.display.setColor(BROWN);
  gb.display.fillRect(plateforms[d].x, plateforms[d].y, plateforms[d].w, plateforms[d].h);
    }
  for(byte d=0; d<NUM_PLATEFORMS; d++){
  gb.display.setColor(PINK);
  gb.display.drawRect(plateforms[d].x+1, plateforms[d].y-1, plateforms[d].w-2, 1);
    }
  }

//dessin des bordures
  void drawBorders(){
  gb.display.setColor(RED);
  gb.display.fillRect(borders[0].x, borders[0].y, borders[0].w, borders[0].h);
  gb.display.setColor(BLUE);
  gb.display.fillRect(borders[1].x, borders[1].y, borders[1].w, borders[1].h);
  gb.display.setColor(GRAY);
  gb.display.fillRect(borders[2].x, borders[2].y, borders[2].w, borders[2].h);
  gb.display.setColor(GRAY);
  gb.display.fillRect(borders[3].x, borders[3].y, borders[3].w, borders[3].h);

  }



