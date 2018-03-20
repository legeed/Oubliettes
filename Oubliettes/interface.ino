
  void drawMain() {
  if( gb.buttons.pressed(BUTTON_A) ) { //avec un appui sur A je lance le jeu, sur B j'affiche les scores, sur menu les commandes
    presstart = true;
    }
  gb.display.drawImage(0, 0, title);
  gb.display.setColor(BLACK);
  gb.display.setCursor(0, 1);
  gb.display.print(VERSION); 
  gb.display.setColor(GRAY);
  gb.display.setCursor(0, 0);
  gb.display.print(VERSION); 
  if  (gb.buttons.repeat(BUTTON_MENU,1)) {
   gb.display.clear();
   gb.display.drawImage(0, 0, howto);
    }
  if  (gb.buttons.repeat(BUTTON_B,1)) {
   gb.display.clear();
   gb.display.drawImage(0, 0, hscore);
   gb.display.setColor(WHITE);
   gb.display.setCursor(55, 51);
   gb.display.print(int(bestscore/25)); 
    }
  if (gb.buttons.repeat(BUTTON_B,1) && gb.buttons.repeat(BUTTON_MENU,1)) {
    debug = true;
  }
  }
 

  ///////////////////////////////////////////////////
  
  //dessin de l'interface
  void drawInterface() {
  //dessin des bitmaps
  gb.display.drawImage(0, 0, front); //image de l'interface
  if (lives == 2 ) { //je cache les lumières en fonction des vies
  gb.display.setColor(BLACK);
  gb.display.fillRect(4,20,6,8);  
  }
  if (lives == 1 ) {
  gb.display.setColor(BLACK);
  gb.display.fillRect(4,12,6,16);  
  }
  //score
  gb.display.setColor(WHITE);
  if (score >= bestscore) {
  gb.display.setColor(RED);
  }
  gb.display.setCursor(66, 21);
  gb.display.print(int(score/25));
  }

  ///////////////////////////////////////////////////

  void drawBenchmark() {
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
  }

  ///////////////////////////////////////////////////


  void drawBack() {
  if (score > 6000) {
    gb.display.drawImage(0, 0, castle);  
  } else {
    gb.display.drawImage(0, 0, back); //fond
  }
  //jauge de boost un carré coloré masqué par un carré noir en fonction du boost restant
  gb.display.setColor(YELLOW); 
  gb.display.fillRect(5,39,4,20);
  gb.display.setColor(BLACK);
  gb.display.fillRect(5,38,4,map(boosttime,0,100,20,0));
  }

    ///////////////////////////////////////////////////

void drawGameover() { 
  gb.lights.drawPixel(0, 0, RED);
  gb.display.drawImage(0, 0, gameover);
  gb.display.setColor(WHITE);
  gb.display.setCursor(33, 34);
  gb.display.print(int(lastscore/25));
  
  if( gb.buttons.pressed(BUTTON_A) ) { //avec un appui sur A je relance rapidement le jeu
    presstart = true;
    }
  if (gb.buttons.pressed(BUTTON_B) || gb.buttons.pressed(BUTTON_MENU)) {  // B ou C retourne au menu principal
    gamestate = 1;
    }
  }
  
    ///////////////////////////////////////////////////


    void drawReady() {
      gb.display.setColor(WHITE);
      gb.display.setCursor(27, 14);
      gb.display.print("Ready ?");
      if ( gb.buttons.pressed(BUTTON_A) || gb.buttons.pressed(BUTTON_B) ) { //avec un appui sur A ou B je relance rapidement le jeu
          presstart = true;
      }
      if ( gb.buttons.pressed(BUTTON_MENU)) { // C retourne au menu principal - exception a la methode habituelle
         gamestate = 1;
      }
     }

     
    ///////////////////////////////////////////////////


    void drawPause() {
      
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
      if ( gb.buttons.pressed(BUTTON_A) || gb.buttons.pressed(BUTTON_B) || gb.buttons.pressed(BUTTON_MENU)) { //avec un appui sur A ou B je relance rapidement le jeu
          presstart = true;
      }
     }

     /////////////////////////////////////////////////

     void drawHscore() {
      gb.lights.drawPixel(0, 0, YELLOW);
      gb.display.drawImage(0, 0, hscore);
      gb.display.setColor(WHITE);
      gb.display.setCursor(55, 51);
      gb.display.print(int(bestscore/25)); 
      if( gb.buttons.pressed(BUTTON_A) ) { //avec un appui sur A je relance rapidement le jeu
         presstart = true;
         highscore = false;
      }
      if (gb.buttons.pressed(BUTTON_B) || gb.buttons.pressed(BUTTON_MENU)) {  // B ou C retourne au menu principal
         gamestate = 1;
         highscore = false;
      }
     }

