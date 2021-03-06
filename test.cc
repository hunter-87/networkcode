/*
 * C++ Conversion of NCUTILS https://code.google.com/p/ncutils/
 * Copyright (c) 2011-2012 Matteo Bruni <matteo.bruni at gmail.com>
 *
 * Original License header:
 *
 * Original Java implementation Copyright (c) 2010, EPFL - ARNI
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the EPFL nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "ncutils.h"

#include <stdio.h>
#include <iostream>
#include <bitset>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdlib>

#define TEST_LEN 10

void test_vectors();
void block_level_example();
void test_coding_vectors();

int main() {

	test_vectors();
	//test_coding_vectors();
	block_level_example();


	return 0;
}

void block_level_example(){

	/**
	 *
	 * This example shows how to use ncutils to build simulate
	 * random network coding network. Uncoded packets are first
	 * created then they are transformed to the corresponding coded
	 * packets. These are linearly combined (as a network would) in
	 * new coded packets. The resulting packets are finally decoded
	 * with the packet decoder.
	 *
	 * @author lokeller
	 */


	FiniteField* ff = FiniteField::getDefaultFiniteField();

	int blockNumber = 10;
	int payloadLen = 10;


	//int payloadLenCoeffs = 20;


	/* create the uncoded packets */
	//UncodedPacket[] inputPackets = new UncodedPacket[blockNumber];
	std::vector<UncodedPacket*> inputPackets;
	inputPackets.reserve(blockNumber);

	std::cout << " Input blocks "<< std::endl;;
	for ( int i = 0 ; i < blockNumber ; i++) {
		unsigned char* payload = new unsigned char[payloadLen];

		std::fill(payload, payload+payloadLen, 0xA0+i);
		inputPackets.push_back(new UncodedPacket(i, payload, payloadLen));
		std::cout<< "Uncodedpacket: " << i << ": "<< inputPackets[i]->toString()<< std::endl;

		// free memory since uncodedpacket() creates a copy
		delete [] payload;
	}


	/* prepare the input packets to be sent on the network */
	std::vector<CodedPacket*> codewords;
	codewords.reserve(blockNumber); //= new CodedPacket[blockNumber];

	std::cout << " Coded Packet: " <<std::endl;
	for ( int i = 0 ; i < blockNumber ; i++) {

		codewords.push_back(new CodedPacket( inputPackets[i], blockNumber, ff));
		delete inputPackets[i];

		std::cout<< "Packet: " << i << ": "<< codewords[i]->toString()<< std::endl;

	}


	/* create a set of linear combinations that simulate
	 * the output of the network
	 */

	std::vector<CodedPacket*> networkOutput;
	networkOutput.reserve(blockNumber);


	//int temp_time = time(NULL);
	srand(1322825377);//1322825321 //897563);//2131231);
	//std::cout<<"TEMP TIME: "<<temp_time<<std::endl;


	//Random r = new Random(2131231);
	std::cout << " Network Packet: " <<std::endl;
	for ( int i = 0 ; i < blockNumber ; i++) {

		networkOutput[i] = CodedPacket::createEmptyCodedPacketPtr(blockNumber, payloadLen, ff);

		for ( int j = 0 ; j < blockNumber ; j++) {
			int x = rand()%ff->getCardinality(); // r.nextInt(ff.getCardinality());
			CodedPacket* copy = codewords[j]->scalarMultiply(x);
			networkOutput[i]->addInPlace(copy);
			//networkOutput[i] = networkOutput[i]->add(copy);

			delete copy;
		}
		std::cout<< "Packet: " << i << ": "<< networkOutput[i]->toString()<< std::endl;

	}

	/* decode the received packets */
	PacketDecoder decoder(ff, blockNumber, payloadLen);

	std::cout << " Decoded Packet: " <<std::endl;

	std::vector<UncodedPacket*> decoded_packets;
	for ( int i = 0; i < blockNumber ; i++) {

		std::vector<UncodedPacket*> packets = decoder.addPacket(networkOutput[i]);
		//printUncodedPackets(packets, payloadLen);
		//std::cout<< packets.size()<<std::endl;
		std::cout<< "Recived packets: "<<i<< ". Decoded packets thanks this reception:" <<std::endl;
		for (unsigned int j=0; j<packets.size(); j++) {
			std::cout<< "	Packet: "<< packets[j]->toString()<< std::endl;

			// save decoded packets for later use
			decoded_packets.push_back(packets[j]->copy());

		}
		for (unsigned int j=0; j<packets.size(); j++) {
			// clear memory from temp pointers
			delete packets[j];

		}

	}

	std::cout << " We have decoded: "<< decoded_packets.size() << " packets"<<std::endl;


	// free all memory
	for ( int i = 0 ; i < blockNumber ; i++) {
//		delete inputPackets[i];
		delete codewords[i];
		delete networkOutput[i];
		delete decoded_packets[i];
	}
	delete ff;

}


void test_vectors(){
	/*
	 * This example show how to convert a payload in its finite field
	 * vector representation and how to obtain the payload from the ff vector
	 *
	 */

    unsigned char payload[TEST_LEN] = {0xa0, 0xb1, 0xc2,0x53,0x4,0x5,0x6, 0x7, 0x8, 0xac};

    //unsigned char payload[TEST_LEN] = {0xa0};

    printf("\n\n Hex representation \n <  ");
    for ( int i=0 ; i<TEST_LEN; i++){
    	if ( i == TEST_LEN-1 ){
        	printf(" %x >", payload[i]);
    	} else {
        	printf(" %x, ", payload[i]);
    	}

    }
    printf("\n\n");


    printf(" Bit representation \n <  ");
    for ( int i=0 ; i<TEST_LEN; i++){
    	if ( i == TEST_LEN - 1 ){
    		std::cout << std::bitset<8>(payload[i])<< " >" << std::endl;
    	} else {
    		std::cout << std::bitset<8>(payload[i])<< ", ";
    	}

    }
    printf("\n\n");

    printf("payload 10 bytes - ff Q=16, m=4 -> vector with 20 elements \n\n");

    FiniteField* field = new FiniteField(2,4);

    FiniteFieldVector* vector = field->byteToVector(payload, TEST_LEN );

    printf("creating a finitefield vector with %u elements \n", vector->getLength());


    printf(" vector elements: \n <  ");
    for (unsigned int i=0 ; i<vector->getLength(); i++){
    	if ( i == vector->getLength()-1 ){
    		std::cout << std::bitset<4>(vector->getCoordinate(i))<< " >" << std::endl;
    	} else {
    		std::cout << std::bitset<4>(vector->getCoordinate(i))<< ", ";
    	}

    }
    printf(" vector to string :\n" );

	std::cout << vector->toString()<< std::endl;

    printf("\n\n");


    printf("getting back the payload from the vector \n");

    unsigned char* back = field->vectorToBytes(vector) ;


    printf("\n\n Hex representation \n <  ");
    for ( int i=0 ; i<TEST_LEN; i++){
    	if ( i == TEST_LEN -1 ){
        	printf(" %x >", back[i]);
    	} else {
        	printf(" %x, ", back[i]);
    	}

    }
    printf("\n\n");


    printf(" Bit representation \n <  ");
    for ( int i=0 ; i<TEST_LEN; i++){
    	if ( i == TEST_LEN -1  ){
    		std::cout << std::bitset<8>(back[i])<< " >" << std::endl;
    	} else {
    		std::cout << std::bitset<8>(back[i])<< ", ";
    	}

    }
}


