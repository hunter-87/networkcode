
#include <string>

// TODO: DISTRUTTORI VARI!!!!

int ipow(int base, int exp);

class FiniteFieldVector;

class FiniteField {

	public:
		/**
		 * Associates each field element to its inverse
		 */
		int *inverse;

		/**
		 * Associate two field elements to their sum
		 */
		int **sum;

		/**
		 * Associate two field elements to their division
		 */
		int **div;

		/**
		 * Associate two field elements to their substraction
		 */
		int **sub;

		/**
		 * Associate two field elements to their multiplication
		 */
		int **mul;


		FiniteField(int q=2, int m=4);
		FiniteField(int total_size);
		// TODO
		~FiniteField();

//		FiniteFieldVector byteToVector(unsigned char *bytes, int bytes_lenght);
//		FiniteFieldVector byteToVector(unsigned char *bytes, int bytes_lenght, int coordinates);
//		FiniteFieldVector byteToVector(unsigned char *bytes, int offset, int bytes_lenght, int coordinates);

		FiniteFieldVector* byteToVector(unsigned char *bytes, int bytes_lenght, int offset = 0);
		int readBits(unsigned char* data, int data_lenght, int offset, int field, int fieldSize);

		unsigned char* vectorToBytes(FiniteFieldVector* vector);
		void vectorToBytes (FiniteFieldVector* vector, unsigned char* output, int start);
		void writeBits(unsigned char* data, int offset, int field, int value, int fieldSize);

		int bytesLength(int coordinates_count);
		int bitsPerCoordinate();
		int coordinatesCount(int bytes_length);
		int getCardinality();

	private:
		/**
		 * Total number of elements in the field
		 */
		int Q;

};


class FiniteFieldVector {

	public:

		FiniteFieldVector(int len, FiniteField* ff);
		FiniteFieldVector(int coords[], int len, FiniteField* field);
		// TODO
		~FiniteFieldVector();

		int getLength();
		FiniteField* getFiniteField();

		void setCoordinate(int index, int value);
		int getCoordinate(int index);
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
		int length;
		FiniteField* ff ;

};


class UncodedPacket {

	public:

		static UncodedPacket wrap(int id, unsigned char *payload, int length);

		UncodedPacket(int id, unsigned char *payload, int length);
		UncodedPacket(int id, FiniteFieldVector* vector);
		// TODO
		~UncodedPacket();

		int getId();
		unsigned char* getPayload();
		int getPayloadLength();
		UncodedPacket* copy();

		//TODO operator:=
		int compareTo(UncodedPacket o);

	private:

		int id;
		unsigned char* payload;
		int payload_length;
		UncodedPacket(int id);



};

class CodedPacket {

	public:

		CodedPacket(FiniteFieldVector* codingVector, FiniteFieldVector* payloadVector);
		CodedPacket(UncodedPacket* packet, int maxPackets, FiniteField* ff);
		CodedPacket(int maxPackets, int payloadByteLen, FiniteField* ff);
		CodedPacket(int maxPackets, unsigned char* data, int offset, int length, FiniteField* ff);

		static CodedPacket* createEmptyCodedPacket(int max_packets, int payload_byte_lenght, FiniteField* ff);

		// TODO
		~CodedPacket();

		FiniteFieldVector* getCodingVector();
		FiniteFieldVector* getPayloadVector();
		FiniteField* getFiniteField();

		void setCoordinate(int index, int value);
		int getCoordinate(int index);

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






