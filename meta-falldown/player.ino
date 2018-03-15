
MovingBoxP player; //créé mon objet qui représente le joueur 


void initPlayer(){ //balance la balle au centre !
  player.w = tilesize;
  player.h = tilesize;
  player.x = tilesize*10;
  player.y = tilesize*4;
  boosttime = 125; //réinitialisation du boost
  bonusscore = false; // par défaut les bonus sont invalidés
  malusscore = false;
  playerdead = false;
  }



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
  }
  if (player.y >= 15*tilesize) {
    player.y -= player.yv; //on recule en cas de collision avec l'eau
    player.yv *= (-1)*(rebound/2); //on ralenti beaucoup et rebondi !
    malusscore = true; //rester dans l'eau diminue le score
  }
  if (player.y < 1*tilesize) { //le joueur a dépassé la limite et s'est fait coincé
    playerdead = true;
  }
  if ( (player.y < (4*tilesize)) && !playerdead ) { //le joueur est en haut de l'écran et son score profite du bonus
    bonusscore = true;
  }
  
  ////////horizontalement
  player.xv *= friction; //friction
  if (gb.buttons.repeat(BUTTON_A, 1) && boosttime > 0){ //Turbo !
    player.xv *= 1.25;
    boosttime = max(0,boosttime - 1); //consomme du boost
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
  }
  ////////detection de la perte de vie
  if (player.y <= tilesize) {
    //faire des trucs qui font perdre  
  }

  ////////calcul du score (à diviser par 25 au final, sinon ça dépasse trop) :p
  score++; 
  if (bonusscore) {
    score++;
  }
  if (malusscore) {
    score--;
  }
  lastscore = max(lastscore, score);
  
}

void drawPlayer(){ //dessin de la balle
  //dessine la "hitbox" de la balle 
  gb.display.setColor(GREEN);
  gb.display.fillRect(player.x, player.y, player.w, player.h);
}


boolean playerCollision(){ //détecte si collision une plateforme
  for(byte i=0; i<NUM_PLATEFORMS; i++){
    if(gb.collideRectRect(player.x, player.y, player.w, player.h, plateforms[i].x, plateforms[i].y, plateforms[i].w, plateforms[i].h)){
      return true;
    }
  }
  return false;
}

boolean playerLanding(){ //détecte si le joueur est SUR une plateforme
  for(byte i=0; i<NUM_PLATEFORMS; i++){
    if(gb.collideRectRect(player.x, player.y, player.w, player.h, plateforms[i].x+1, plateforms[i].y-1, plateforms[i].w-2, 1)){
    return true;
    }
  }
  return false;
 }

boolean playerBorder(){ //détecte si le joueur est contre une bordure  
  for(byte i=0; i<NUM_BORDERS; i++){
    if(gb.collideRectRect(player.x, player.y, player.w, player.h, borders[i].x, borders[i].y, borders[i].w, borders[i].h)){
      return true;
    }
  }
  return false;
}





