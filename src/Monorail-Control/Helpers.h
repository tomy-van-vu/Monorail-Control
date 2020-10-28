/*
 * Written by T4C2 - __author name here__
 * 
 * Functions to send fixed messages to a control box via bluetooth link
 */

#pragma once
// FUNCTIONS TO SEND STATE DATA
void send_stop() { quick_transmit(&bt_i, STOP); Serial.println("-- Message sent -- STOP");}

void send_go() { quick_transmit(&bt_i, START); Serial.println("-- Message sent -- START");}

void send_east() { quick_transmit(&bt_i, EAST); Serial.println("-- Message sent -- EAST");}

void send_west() { quick_transmit(&bt_i, WEST); Serial.println("-- Message sent -- WEST");}

void send_emergency() { quick_transmit(&bt_i, EMERGENCY); Serial.println("-- Message sent -- EMERGENCY");}

void send_not_emergency() { quick_transmit(&bt_i, NONE); Serial.println("-- Message sent -- NONE");}

void send_doors_open() { quick_transmit(&bt_i, OPEN); Serial.println("-- Message sent -- OPEN");}

void send_doors_closed() { quick_transmit(&bt_i, CLOSE); Serial.println("-- Message sent -- CLOSE");}
