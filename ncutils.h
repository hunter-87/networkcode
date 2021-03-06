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

#ifndef __NCUTILS_omnet_h__

#define __NCUTILS_omnet_h__

#include <string>
#include <map>
#include <vector>

#include <iostream>


int ipow(int base, int exp);

class FiniteFieldVector;

class FiniteField {

	public:

		FiniteField(int q, int m);
		FiniteField(int total_size);
		~FiniteField();
		static FiniteField* getDefaultFiniteField();


		int sum(int a, int b);
		int sub(int a, int b);
		int div(int a, int b);
		int mul(int a, int b);



//		FiniteFieldVector byteToVector(unsigned char *bytes, int bytes_lenght);
//		FiniteFieldVector byteToVector(unsigned char *bytes, int bytes_lenght, int coordinates);
//		FiniteFieldVector byteToVector(unsigned char *bytes, int offset, int bytes_lenght, int coordinates);

		FiniteFieldVector* byteToVector(unsigned char *bytes, int bytes_lenght, int offset = 0);

		int readBits(unsigned char* data, int data_lenght, int offset, int field, int fieldSize);

		unsigned char* vectorToBytes(FiniteFieldVector* vector);
		void vectorToBytes (FiniteFieldVector* vector, unsigned char* output, int start);
		void writeBits(unsigned char* data, int offset, int field, int value, int fieldSize);

		unsigned int bytesLength(int coordinates_count);
		unsigned int bitsPerCoordinate();
		unsigned int coordinatesCount(int bytes_length);
		int getCardinality();

	private:
		/**
		 * Total number of elements in the field
		 */
		int Q;
		/**
		 * Associates each field element to its inverse
		 */
		int *inverse;

		/**
		 * Associate two field elements to their sum
		 */
		int **_sum;

		/**
		 * Associate two field elements to their division
		 */
		int **_div;

		/**
		 * Associate two field elements to their substraction
		 */
		int **_sub;

		/**
		 * Associate two field elements to their multiplication
		 */
		int **_mul;


};


class FiniteFieldVector {

	public:

		FiniteFieldVector(unsigned int len, FiniteField* ff);
		FiniteFieldVector(int coords[], unsigned int len, FiniteField* field);
		~FiniteFieldVector();


		unsigned int getLength();
		FiniteField* getFiniteField();

		void setCoordinate(unsigned int index, int value);
		int getCoordinate(unsigned int index);
		void setToZero();

		FiniteFieldVector* copy();

		FiniteFieldVector* add(FiniteFieldVector* vector);
		void addInPlace(FiniteFieldVector* vector);

		FiniteFieldVector* scalarMultiply(int c);
		void scalarMultiplyInPlace(int c);

		FiniteFieldVector* multiplyAndAdd(int c, FiniteFieldVector* other);
		void multiplyAndAddInPlace(int c, FiniteFieldVector* other);

		std::string toString();


	private:
		int *coordinates;
		unsigned int length;
		FiniteField* ff ;


};


class UncodedPacket {

	public:

		static UncodedPacket wrap(int id, unsigned char *payload, int length);

		UncodedPacket(int id, unsigned char *payload_vector, unsigned int length);
		UncodedPacket(int id, FiniteFieldVector* vector);
		~UncodedPacket();

		int getId();
		unsigned char* getPayload();
		void setPayload(unsigned char* newpayload, int newlenght );

		unsigned int getPayloadLength();
		UncodedPacket* copy();

		//TODO operator:=
		int compareTo(UncodedPacket o);

		std::string toString();

	private:

		int id;
		unsigned char* payload;
		unsigned int payload_length;
		UncodedPacket(int id);




};

class CodedPacket {

	public:

		CodedPacket(FiniteFieldVector* codingVector, FiniteFieldVector* payloadVector);
		CodedPacket(UncodedPacket* packet,unsigned int maxPackets, FiniteField* ff);
		//CodedPacket(int maxPackets, int payloadByteLen, FiniteField* ff);
		CodedPacket(const CodedPacket& p);

		CodedPacket(int maxPackets, unsigned char* data, int offset, int length, FiniteField* ff);
		~CodedPacket();


		static CodedPacket* createEmptyCodedPacketPtr(int max_packets, int payload_byte_lenght, FiniteField* ff);

		FiniteFieldVector* getCodingVector();
		FiniteFieldVector* getPayloadVector();
		FiniteField* getFiniteField();

		void setCoordinate(unsigned int index, int value);
		int getCoordinate(unsigned int index);

		//TODO: add a copy constructor?
		CodedPacket* copy();
		void setToZero();

		CodedPacket* add(CodedPacket* vector);
		void  addInPlace(CodedPacket* vector);

		CodedPacket* scalarMultiply(int c);
		void scalarMultiplyInPlace(int c);

		CodedPacket* multiplyAndAdd(int c, CodedPacket* vector);
		void  multiplyAndAddInPlace(int c, CodedPacket* vector);

		unsigned char* toByteArray();
		std::string toString();

	private:
		FiniteFieldVector* coding_vector;
		FiniteFieldVector* payload_vector;

};

/**
 *
 * This exception is raised by CodingVectorDecoder when
 * a vector is detected to be linerly dependant of the vectors
 * previously added.
 *
 * @author lokeller
 */
class LinearDependantException {
	public:
		LinearDependantException(){};
};
//LinearDependantException::LinearDependantException(){}

class CodingVectorDecoder {

	public:
		CodingVectorDecoder(unsigned int maxPackets, FiniteField* ff);
		~CodingVectorDecoder();
		CodingVectorDecoder(const CodingVectorDecoder& other);



		CodingVectorDecoder* copy();

		unsigned int getMaxPackets();
		int getSubspaceSize();

		std::map<int, FiniteFieldVector*> addVector(FiniteFieldVector* v) throw(LinearDependantException);


	private:

		void initVars();

		/* the matrix used for gaussian jordan elimination, the first half of the
		 * columns store the matrix being inverted the second half the inverted
		 * matrix.
		 */
		int** decodeMatrix;

		/** stores the position of the pivot of each line */
		int* pivotPos;

		/** stores for each column if it is a pivot column for a line or not*/
		bool* isPivot;

		/** stores for each column if it has already been decoded or not*/
		bool* decoded;

		/** stores the number of non-zero lines in the decode matrix ( the number
		 * of packets that have been received */
		unsigned int packetCount;

		/*
		 * maxPackets the length of the vectors have to be decoded
		 */
		unsigned int decodeMatrixLenght;

		/** the finite field that is used in this decoder */
		FiniteField* ff;


};



///**
// * This class supports decoding of coded packets. It uses a coding vector
// * decoder to compute how to reconstruct uncoded packets and then linearly
// * combine the received packets payloads to reconstruct the uncoded payloads.
// *
// * @author lokeller
// */



class PacketDecoder {

	public:
		PacketDecoder(FiniteField* field, int maxPackets, int payloadBytesLength);
		PacketDecoder(const PacketDecoder& p);

		~PacketDecoder();

		PacketDecoder& operator= (const PacketDecoder & p);

		int getSubspaceSize();
		unsigned int getMaxPackets();
		std::vector<UncodedPacket*> addPacket(CodedPacket* p);

	private:
		FiniteFieldVector* decodePayload(FiniteFieldVector* encoding);

		std::vector<CodedPacket*> packets;// = new Vector<CodedPacket>();

		CodingVectorDecoder* codingVectorDecoder;
		unsigned int payloadCoordinatesCount;
		FiniteField* ff;


};

#endif

