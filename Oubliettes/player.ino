
MovingBoxP player; //créé mon objet qui représente le joueur 

////////////////////////////////////////////////////////////////////////////////


void initPlayer(){ //balance la balle au centre !
  player.w = tilesize;
  player.h = tilesize;
  player.x = tilesize*10;
  player.y = tilesize*4;
  boosttime = 80; //réinitialisation du boost
  bonusscore = false; // par défaut les bonus sont invalidés
  malusscore = false;
  }


////////////////////////////////////////////////////////////////////////////////


void updatePlayer(){ //la balle bouge !

  bonusscore = false; // par défaut les bonus sont invalidés
  malusscore = false;
  
  ////////verticalement 
  player.y -= scrollspeed; 
  //on applique le scrolling avec un min pour ne pas dépasser la zone de jeu !
  if (!playerLanding()){ //si pas sur une plateforme, alors on tombe !
  player.yv += gravity; //gravity, mais sans george
  }
  player.yv *= friction; //friction
  if(gb.buttons.timeHeld(BUTTON_UP) > 0 & gb.buttons.timeHeld(BUTTON_UP) < 5 && playerLanding()){ 
    player.yv -= jump; //jump baby jump, plus on appuie longtemps plus on saute haut, il faut être sur une platef. aussi !
    boosttime = max(0,boosttime - 1); //sauter consomme du boost
  }
  player.y += player.yv; //on avance
  if(playerCollision()){
    player.y -= player.yv; //on recule en cas de collision avec une plateforme
    player.yv *= (-1)*rebound; //on ralenti et rebondi !
    //gb.sound.playTick();
  }
  if (player.y >= 15*tilesize) {
    player.y -= player.yv; //on recule en cas de collision avec l'eau
    player.yv *= (-1)*(rebound/2); //on ralenti beaucoup et rebondi !
    malusscore = true; //rester dans l'eau diminue le score
  }
 
 
  
  ////////horizontalement
  player.xv *= friction; //friction
  if (gb.buttons.repeat(BUTTON_A, 1) && boosttime > 0 && playerLanding() ){ //Turbo !
    player.xv *= 1.25;
    boosttime = max(0,boosttime - 2); //consomme du boost
  }
  if (gb.buttons.repeat(BUTTON_B, 1) && boosttime > 0 && playerLanding() ){ //Brake !
    player.xv *= 0.50;
    boosttime = max(0,boosttime - 2); //consomme du boost aussi :p
  }  
  if(gb.buttons.repeat(BUTTON_RIGHT, 1)){
    player.xv += movespeed; //bouge en cas d'appui
    }
  if(gb.buttons.repeat(BUTTON_LEFT,1)){
    player.xv -= movespeed;
  }
  player.x += player.xv; //on avance
  if(playerCollision() || playerBorder()){
    player.x -= player.xv; //on recule en cas de collision
    player.xv *= (-1)*rebound; //en sens inverse !
    //gb.sound.playTick();
  }
  
  ///////le joueur est en haut de l'écran et son score profite du bonus mais je l'alerte avec les led
  if ( (player.y < (4*tilesize)) && player.y > tilesize  ) { 
    bonusscore = true;
    //gb.lights.drawPixel(0, 0, RED);
  }

  ////////detection du ramassage d'un bonus - je teste la collision ici directement
  for(byte i=0; i<NUM_BONUS; i++){
    if(gb.collideRectRect(player.x, player.y, player.w, player.h, bonus[i].x, bonus[i].y, bonus[i].w, bonus[i].h)){
      switch (bonus[i].type) {
        case 1: //bonus type 1 c'est du score
          score += 250;
        break;
        case 2: //bonus type 2 c'est du boost
          boosttime = min(boosttime+10,100);
          score += 50;
        break;
      }
      //gb.lights.drawPixel(0, 0, WHITE);
      bonus[i].type = 0; //le bonus est ramassé !
    }
  }
  

  ////////calcul du score (à diviser par 25 au final, sinon ça dépasse trop) :p
  score++; 
  if (bonusscore) {
    score++;
  }
  if (malusscore) {
    score = max(score-25,1);
    //gb.lights.drawPixel(0, 0, BLUE);
  }
      
  ////////detection de la perte de vie
  if (player.y < tilesize) {
    gamestate = 3; //le jeu se en "nouvelle vie"
    lives = max(lives - 1,0);
    player.y = tilesize;
  }
  if (lives == 0) { //si le joueur n'a plus de vie j'arrête le jeu en gameover et sauve le score et le remet à 0
     gamestate = 2;
     lastscore = score;
    if (score > bestscore) {
        bestscore = score;
        gb.save.set(0,bestscore);
        highscore = true;
     }
     score = 0;
  }

  ////////detection la pause
  if (gb.buttons.pressed(BUTTON_MENU)) {
    presstart = true;
  }
  
  
}


////////////////////////////////////////////////////////////////////////////////

void drawPlayer(){ //dessin de la balle
  gb.display.setColor(RED);
  gb.display.fillCircle(player.x+2, player.y+2, 2);
  gb.display.setColor(ORANGE);
  gb.display.fillRect(player.x+1, player.y+1, 3, 3); 
  gb.display.setColor(YELLOW);
  gb.display.fillRect(player.x+1, player.y+1, 2, 2);
  
  /*dessine la "hitbox" de la balle 
  gb.display.setColor(GREEN);
  gb.display.fillRect(player.x, player.y, player.w, player.h);*/


  
}

////////////////////////////////////////////////////////////////////////////////

boolean playerCollision(){ //détecte si collision une plateforme
  for(byte i=0; i<NUM_PLATEFORMS; i++){
    if(gb.collideRectRect(player.x, player.y, player.w, player.h, plateforms[i].x, plateforms[i].y, plateforms[i].w, plateforms[i].h)){
      return true;
    }
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////

boolean playerLanding(){ //détecte si le joueur est SUR une plateforme
  for(byte i=0; i<NUM_PLATEFORMS; i++){
    if(gb.collideRectRect(player.x, player.y, player.w, player.h, plateforms[i].x+1, plateforms[i].y-1, plateforms[i].w-2, 1)){
    return true;
    }
  }
  return false;
 }
 
////////////////////////////////////////////////////////////////////////////////

boolean playerBorder(){ //détecte si le joueur est contre une bordure  
  for(byte i=0; i<NUM_BORDERS; i++){
    if(gb.collideRectRect(player.x, player.y, player.w, player.h, borders[i].x, borders[i].y, borders[i].w, borders[i].h)){
      return true;
    }
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////




