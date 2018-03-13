//dessin de l'interface
  void drawInterface() {
  gb.display.setColor(WHITE);
  gb.display.setCursor(62, 2);
  gb.display.print(playerLanding());
  gb.display.setCursor(62, 8);
  gb.display.print(playerCollision());
  }
