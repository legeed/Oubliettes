  void mainMenu(){
  gb.display.setColor(RED);
  gb.display.fillRect(10, 20, 60, 24);
    if(gb.buttons.repeat(BUTTON_A, 1)){
    presstart = true; //press start !
    }
  gb.display.setFontSize(1);
  gb.display.setColor(WHITE);
  gb.display.setCursor(14, 24);
  gb.display.print("META-FALLDOWN");
  gb.display.setFontSize(1);
  gb.display.setCursor(22, 34);
  gb.display.print("TOP");
  gb.display.setCursor(40, 34);
  gb.display.print(int(lastscore/25));
  gb.display.setCursor(8, 48);
  gb.display.print("PRESS A TO START");
  
  }
  
  void setMode() { //règle les paramètres de mode et début de partie et initialise le score
  score = 0;
  mode = random(0,3);
  switch (mode) {
    case 1: //mode 1
      lives = 3;
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
      lives = 3;
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
      lives = 3;
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

  
  //dessin de l'interface
  void drawInterface() {
  gb.display.setColor(WHITE);
  gb.display.setCursor(61, 2);
  gb.display.print(int(score/25));
  gb.display.setCursor(61, 8);
  gb.display.print(int(lastscore/25));
  gb.display.setCursor(61, 14);
  gb.display.print(lives);
  gb.display.setCursor(61, 20);
  gb.display.print(boosttime);
  
  }


