
MovingBox player; //créé mon objet qui représente le joueur

void initPlayer(){ //balance la balle au centre !
  player.w = tilesize;
  player.h = tilesize;
  player.x = tilesize*10;
  player.y = tilesize*4;
  }

void updatePlayer(){ //la balle bouge !

  //verticalement 
  
  player.yv -= scrollspeed; //défilement
  
  if (!playerLanding()){
  player.yv += gravity; //gravity
  }
  
  player.yv *= friction; //friction
  
  if(gb.buttons.timeHeld(BUTTON_UP) > 0 & gb.buttons.timeHeld(BUTTON_UP) < 5 && playerLanding()){ //higher jump by holding A down
    player.yv -= jump; //jump
  }
  
  player.y += player.yv; //on avance
  //check for collisions on y axis
  if(playerCollision()){
    player.y -= player.yv; //on recule en cas de collision
    player.yv *= -0.5;
  }

  
  //horizontalement
  player.xv *= friction; //friction
  if(gb.buttons.repeat(BUTTON_RIGHT, 1)){
    player.xv += movespeed;
  }
  if(gb.buttons.repeat(BUTTON_LEFT,1)){
    player.xv -= movespeed;
  }
  player.x += player.xv; //on avance
  if(playerCollision()){
    player.x -= player.xv; //on recule en cas de collision
    player.xv *= -0.5; //en sens inverse !
  }
  
}

void drawPlayer(){ //dessin de la balle
  gb.display.setColor(GREEN);
  gb.display.fillRect(player.x, player.y, player.w, player.h);
}


boolean playerCollision(){ //détecte si collision avec un mur ou une plateforme
  for(byte i=0; i<NUM_PLATEFORMS; i++){
    if(gb.collideRectRect(player.x, player.y, player.w, player.h, plateforms[i].x, plateforms[i].y, plateforms[i].w, plateforms[i].h)){
      return true;
    }
  }
  for(byte i=0; i<NUM_BORDERS; i++){
    if(gb.collideRectRect(player.x, player.y, player.w, player.h, borders[i].x, borders[i].y, borders[i].w, borders[i].h)){
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






