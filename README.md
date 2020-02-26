# LED Pong

Pong is a table tennis game, that has two paddles on each side hitting a ball.
The first player to get 3 wins, which means hitting it onto the opponent's side successfully, wins.

game.h file in /header includes all of Pong's SM and Game Logic/Physics
## Functions Used
startGame(): simple function that executes all functions needed to run Pong

updateP1(): changes the position of Player 1's paddle using input from PIND

updateP2(): changes the position of Player 2's paddle using input from PIND

updatePong(): takes care of the ball bouncing and physics

update(): Updates the 8x8 matrix using inputs from the player, uses previous functions and displays them

resetValues(): starts over the game if a Player wins

checkScore(): updates the Player score and checks to see if a Player has won
