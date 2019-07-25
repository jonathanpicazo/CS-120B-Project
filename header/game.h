#ifndef GAME_H
#define GAME_H
#include <avr/io.h>

unsigned char input;
unsigned short hold_countp1 = 0;
unsigned short hold_countp2 = 0;
unsigned short phys_hold = 0;
unsigned short trackball = 0;
char connect = -1;
char bounce = -1;
unsigned char s = 0;
unsigned char score_p1 = -1; // due to random score bug ;P
unsigned char score_p2 = 0;
unsigned char p1_appl = 0; //decides who can score
unsigned char p2_appl = 0;
unsigned char paddle_p1[6] = {0x07, 0x0E, 0x1C, 0x38, 0x70, 0xE0};
unsigned char paddle_p2[6] = {0x07, 0x0E, 0x1C, 0x38, 0x70, 0xE0};
unsigned char phys_row[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
unsigned char phys_column[8] = {0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};
unsigned char cur_p1 = 2;
unsigned char cur_p2 = 2;
unsigned char column_p1 = 0x7F;
unsigned char column_p2 = 0xFE;
unsigned char i = 3;
unsigned char j = 3;

void resetValues() {
	hold_countp1 = 0;
	hold_countp2 = 0;
	phys_hold = 0;
	trackball = 0;
	connect = -1;
	bounce = 1;
	s = 0;
	score_p1 = -1;
	score_p2 = 0;
	p1_appl = 0;
	p2_appl = 0;
	cur_p1 = 2;
	cur_p2 = 2;
	i = 3;
	j = 3;
}

void startGame(unsigned char passed_input) {
	input = passed_input;
	updatePlayer1();
	updatePlayer2();
	updatePong();
	update();
	if (checkScore() != -1) {
		PORTC = 0x00;
		resetValues();
		return;
	}
}

enum player1s {init_p1, pad1, right1, left1, holddown1} state_p1;
void updatePlayer1() {	
	switch(state_p1) {
		case init_p1:
			state_p1 = pad1;
			break;
		case pad1:
			if (input & 0x01) {
				state_p1 = right1;
			}
			else if(input & 0x02) {
				state_p1 = left1;
			}
			else {
				state_p1 = pad1;
			}
			break;
		case right1:
			state_p1 = holddown1;
			break;
		case left1:
			state_p1 = holddown1;
			break;
		case holddown1:
			if (hold_countp1 == 2500) {
				state_p1 = pad1;
			}
			else {
				state_p1 = holddown1;
			}
			break;
	}	
	switch (state_p1) {
		case init_p1:
			break;
		case pad1:
			hold_countp1 = 0;
			break;
		case right1:
			if (cur_p1 < 5) {
				++cur_p1;
			}
			break;
		case left1:
			if (cur_p1 > 0) {
				--cur_p1;
			}
			break;
		case holddown1:
			hold_countp1++;
			break;
	}
}

enum player2s {init_p2, pad2, right2, left2, holddown2} state_p2;
void updatePlayer2() {
	switch(state_p2) {
		case init_p2:
			state_p2 = pad2;
			break;
		case pad2:
			if (input & 0x04) {
				state_p2 = right2;
			}
			else if (input & 0x08) {
				state_p2 = left2;
			}
			break;
		case right2:
			state_p2 = holddown2;
			break;
		case left2:
			state_p2 = holddown2;
			break;
		case holddown2:
			if (hold_countp2 == 2500) {
				state_p2 = pad2;
			}
			else {
				state_p2 = holddown2;
			}
			break;
	}
	switch(state_p2) {
		case init_p2:
			hold_countp2 = 0;
			break;
		case pad2:
			hold_countp2 = 0;
			break;
		case right2:
			if (cur_p2 < 5) {
				++cur_p2;
			}
			break;
		case left2:
			if (cur_p2 > 0) {
				--cur_p2;
			}
			break;
		case holddown2:
			hold_countp2++;
			break;
	}
}

enum pongpPhysics {init_pong, phys_wait, pong_update, contain} pong_state;
void updatePong() {
	switch (pong_state) {
		case init_pong:
			pong_state = phys_wait;
			break;
		case phys_wait:
			if (trackball != 10000) {
				pong_state = phys_wait;
			}
			else if (trackball == 10000) {
				trackball = 0;
				pong_state = pong_update;
			}	
			break;
		case pong_update:
			trackball = 0;
			if (j == 0) {
				p1_appl = 1;
				pong_state = init_pong;
			}
			else if (j == 7) {
				p2_appl = 1;
				pong_state = init_pong;
			}
			else {
				pong_state = contain;
			}
			break;
		case contain:
			if (phys_hold != (2000)) {
				pong_state = contain;
			}
			else {
				phys_hold = 0;
				if (p2_appl) {
					pong_state = init_pong;
				}
				else if (p1_appl) {
					pong_state = init_pong;
				}
				else {
					pong_state = pong_update;
				}
			}
			break;
	}	
	switch (pong_state) {
		case init_pong:
			if (!p2_appl && !p1_appl) {
				connect = -1;
				bounce = 0;
				i = 4;
				j = 4;
				cur_p1 = 3;
				cur_p2 = 3;
				p2_appl = 0;
				p1_appl = 0;
			}
			else if(p2_appl) {
				connect = 1;
				bounce = 0;
				i = 4;
				j = 2;
				cur_p1 = 3;
				cur_p2 = 3;
				++score_p2;
			}
			else if (p1_appl) {
				connect = -1;
				bounce = 0;
				i = 4;
				j = 5;
				cur_p1 = 3;
				cur_p2 = 3;
				++score_p1;
			}
			break;
		case phys_wait:
			p2_appl = 0;
			p1_appl = 0;
			++trackball;
			break;
		case pong_update:
			trackball = 0;
			i += bounce;
			j += connect;
			
			if (i > 6 || i < 1) {
				bounce = bounce * -1;
			}
			if (j < 2) {	
				if (i == 0) {
					if(cur_p1 == 0) { connect = 1; bounce = 1; }
				}
				else if (i == 1) {
					if (cur_p1 == 0) { connect = 1; bounce = 0; }
					else if (cur_p1 == 1) { connect = 1; bounce = -1; }
				}
				else if (i == 2) {
					if (cur_p1 == 0) { connect = 1; bounce = 1; }
					else if (cur_p1 == 1) { connect = 1; bounce = 0; }
					else if (cur_p1 == 2) { connect = 1; bounce = -1; }
				}
				else if (i == 3) {
					if (cur_p1 == 1) { connect = 1; bounce = 1; }
					else if (cur_p1 == 2) { connect = 1; bounce = 0; }
					else if (cur_p1 == 3) { connect = 1; bounce = -1; }
				}
				else if (i == 4) {
					if (cur_p1 == 2) { connect = 1; bounce = 1; }
					else if (cur_p1 == 3) { connect = 1; bounce = 0; }
					else if (cur_p1 == 4) { connect = 1; bounce = -1; }
				}
				else if (i == 5) {
					if (cur_p1 == 3) { connect = 1; bounce = 1; }
					else if (cur_p1 == 4) { connect = 1; bounce = 0; }
					else if(cur_p1 == 5) { connect = 1; bounce = -1; }
				}
				else if (i == 6) {
					if (cur_p1 == 4) { connect = 1; bounce = 0; }
					else if (cur_p1 == 5) { connect = 1; bounce = -1; }
				}
				else if (i == 7) {
					if (cur_p1 == 5) { connect = 1; bounce = -1; }
				}
				else {
					p1_appl = 1;
				}
			}
			else if (j > 5) {
				if (i == 0) {
					if (cur_p2 == 0) { connect = -1; bounce = -1; }
				}
				else if (i == 1) {
					if (cur_p2 == 0) { connect = -1; bounce = 0; }
					else if (cur_p2 == 1) { connect = -1; bounce = -1; }
				}
				else if (i == 2) {
					if (cur_p2 == 0) { connect = -1; bounce = 1; }
					else if (cur_p2 == 1) { connect = -1; bounce = 0; }
					else if (cur_p2 == 2) { connect = -1; bounce = -1; }
				}
				else if (i == 3) {
					if (cur_p2 == 1) { connect = -1; bounce = 1; }
					else if (cur_p2 == 2) { connect = -1; bounce = 0; }
					else if (cur_p2 == 3) { connect = -1; bounce = -1; }
				}
				else if (i == 4) {
					if (cur_p2 == 2) { connect = -1; bounce = 1; }
					else if (cur_p2 == 3) { connect = -1; bounce = 0;}
					else if(cur_p2 == 4) { connect = -1; bounce = -1; }
				}
				else if (i == 5) {
					if (cur_p2 == 3) { connect = -1; bounce = 1; }
					else if (cur_p2 == 4) { connect = -1; bounce = 0; }
					else if (cur_p2 == 5) { connect = -1; bounce = -1; }
				}
				else if (i == 6) {
					if (cur_p2 == 4) { connect = -1; bounce = 0; }
					else if (cur_p2 == 5) { connect = -1; bounce = -1; }
				}
				else if (i == 7) {
					if (cur_p2 == 5) { connect = -1; bounce = -1; }
				}
			}
			break;
		case contain:
			++phys_hold;
			break;
	}
}
enum board {wait_init_pong_game, updateP1, updateP2, updateBall} board_state;
void update() {
	switch (board_state) {
		case wait_init_pong_game:
			board_state = updateP1;
			break;
		case updateP1:
			board_state = updateP2;
			break;
		case updateP2:
			board_state = updateBall;
			break;
		case updateBall:
				board_state = updateP1;
			break;
	}
	switch (board_state) {
		case wait_init_pong_game:
			break;
		case updateP1:
			PORTB = paddle_p1[cur_p1];
			PORTA = column_p1;
			break;
		case updateP2:
			PORTB = paddle_p2[cur_p2];
			PORTA = column_p2;
			break;
		case updateBall:
			PORTB = phys_row[i];
			PORTA = phys_column[j];
			break;	
	}
}

int checkScore() {
	if (score_p2 == 1) {
		PORTC |= 0x01; 
	}
	if (score_p2 == 2) {
		PORTC |= 0x04;
	}
	if (score_p2 == 3) {
		PORTC |= 0x07;
		score_p2 = 0;
		score_p1 = 0;
		return 1;
	}
	if (score_p1 == 1) {
		PORTC |= 0x10;
	}
	if (score_p1 == 2) {
		PORTC |= 0x30;
	}
	if (score_p1 == 3) {
		//reset score
		PORTC |= 0x07;
		score_p2 = 0;
		score_p1 = 0;
		return 1;
	}
	return -1;
}
#endif