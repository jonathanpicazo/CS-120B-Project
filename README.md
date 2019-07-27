# CS-120B-Project
120B Final Project for Summer 2019 Session A

Name: Jonathan Picazo

SID: 862024831

Pong is a table tennis game, that has two paddles on each side hitting a ball.
The first player to get 3 wins, which means hitting it onto the opponent's side successfully, wins.

game.h file in /header includes all of Pong's SM and Game Logic/Physics
## Functions Used
updateP1(): changes the position of Player 1's paddle using input from PIND

updateP2(): changes the position of Player 2's paddle using input from PIND

updatePong(): takes care of the ball bouncing and physics

update(): Updates the 8x8 matrix using inputs from the player, uses previous functions and displays them
