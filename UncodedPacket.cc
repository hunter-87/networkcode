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
#include <memory.h>
#include <sstream>


#include <stdio.h>


// TODO: non sembra mai usato
///**
// * Constructs a new UncodedPacket with the specified id and payload
// * The uncoded packet contains a reference to the payload therefore
// * any change to the array payload will be reflected in the contents
// * of the uncoded packet.
// *
// * @param id the id of the packet
// * @param payload the payload
// */
//public static UncodedPacket wrap(int id, byte[] payload) {
//	UncodedPacket pkt = new UncodedPacket(id);
//	pkt.payload = payload;
//	return pkt;
//}



UncodedPacket::UncodedPacket(int id) {
	this->id = id;
	this->payload_length = 0;
}

/**
 * Constructs a new UncodedPacket with the specified id and payload
 *
 * @param id the id of the packet
 * @param payload the payload
 */
UncodedPacket::UncodedPacket(int id, unsigned char* payload_vector, unsigned int length) {
	this->id = id;
	this->payload = new unsigned char[length];
	memcpy(this->payload, payload_vector, length);
	this->payload_length = length;
}

// TODO: serve vectortobytes
/**
 * Creates an uncoded packet with the specified Id and as payload the
 * byte array representation ( see FiniteField.vectorToBytes ) of a
 *  given vector
 *
 * @param id the id of the packet
 * @param payload a vector that will be the payload
 */
UncodedPacket::UncodedPacket(int id, FiniteFieldVector* vector) {

	this->id = id;
	this->payload = vector->getFiniteField()->vectorToBytes(vector);
    this->payload_length = vector->getFiniteField()->bytesLength(vector->getLength());
    		//ff_coordinates_to_bytes(payload->ff, payload->length);
}

UncodedPacket::~UncodedPacket(){

	//std::cout << "Distruttore UncodedPacket " <<std::endl;
//	if (payload_length == 0){
//		delete payload;
//	} else {
	delete [] payload;
	//}

}


/**
 * Returns the id of the packet
 * @return the id of the packet
 */
int UncodedPacket::getId() {
	return this->id;
}



/**
 * Returns the payload of the packet
 *
 * @return the byte array that contains the payload of the packet
 */
unsigned char* UncodedPacket::getPayload() {
	return payload;
}

void UncodedPacket::setPayload(unsigned char* newpayload, int newlenght ) {
	if (this->payload_length != 0){
		delete [] payload;
	}
	this->payload = new unsigned char[newlenght];
	memcpy(payload, newpayload, newlenght);
	this->payload_length = newlenght;

}


unsigned int UncodedPacket::getPayloadLength(){
	return payload_length;
}

UncodedPacket* UncodedPacket::copy() {

	UncodedPacket* copy = new UncodedPacket(id);//, new unsigned char[payload_length], payload_length);
	//memcpy(copy->payload, payload, payload_length);
	copy->setPayload(payload, payload_length);

	return copy;
}



// pay attention to buffer size
std::string UncodedPacket::toString() {

    char tmp[10];
    char buffer[500];
    buffer[0] = 0;
    unsigned int len = 500;

    snprintf(buffer, len, "ID: %d - " , this->id);

    //printf("paylen %u \n", payload_length );
    for (unsigned int i = 0 ; i < this->payload_length ; i++) {
        snprintf(tmp, 10, "%02hhx ", this->payload[i]);

        if ( strlen(buffer) + strlen(tmp) < len) {
            strcat(buffer, tmp);
        }
    }


    std::string ret(buffer);

	return ret;
}


//
//	@Override
//	public int hashCode() {
//		final int prime = 31;
//		int result = 1;
//		result = prime * result + id;
//		result = prime * result + Arrays.hashCode(payload);
//		return result;
//	}
//
//	@Override
//	public boolean equals(Object obj) {
//		if (this == obj)
//			return true;
//		if (obj == null)
//			return false;
//		if (!(obj instanceof UncodedPacket))
//			return false;
//		UncodedPacket other = (UncodedPacket) obj;
//		if (id != other.id)
//			return false;
//		if (!Arrays.equals(payload, other.payload))
//			return false;
//		return true;
//	}
//
//    public int compareTo(UncodedPacket o) {
//
//        if (o.getId() == this.getId()) return 0;
//        else if ( o.id > this.id) return -1;
//        else return 1;
//
//    }
//
//
//}
