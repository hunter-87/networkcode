








//package ch.epfl.arni.ncutils;
//
//import java.util.Arrays;
//
///**
// *
// * This class represents an uncoded packet. An uncoded packet
// * has an unique ID and a payload represented by a byte array.
// *
// * @author lokeller
// */
//public class UncodedPacket implements Comparable<UncodedPacket> {
//
//    private int id;
//    private byte[] payload;
//
//    /**
//     * Constructs a new UncodedPacket with the specified id and payload
//     * The uncoded packet contains a reference to the payload therefore
//     * any change to the array payload will be reflected in the contents
//     * of the uncoded packet.
//     *
//     * @param id the id of the packet
//     * @param payload the payload
//     */
//    public static UncodedPacket wrap(int id, byte[] payload) {
//        UncodedPacket pkt = new UncodedPacket(id);
//        pkt.payload = payload;
//        return pkt;
//    }
//
//    private UncodedPacket(int id) {
//        this.id = id;
//    }
//
//    /**
//     * Constructs a new UncodedPacket with the specified id and payload
//     *
//     * @param id the id of the packet
//     * @param payload the payload
//     */
//    public UncodedPacket(int id, byte[] payload) {
//        this.id = id;
//        this.payload = Arrays.copyOf(payload, payload.length);
//    }
//
//    /**
//     * Creates an uncoded packet with the specified Id and as payload the
//     * byte array representation ( see FiniteField.vectorToBytes ) of a
//     *  given vector
//     *
//     * @param id the id of the packet
//     * @param payload a vector that will be the payload
//     */
//    public UncodedPacket(int id, FiniteFieldVector payload) {
//
//        this.id = id;
//        this.payload = payload.getFiniteField().vectorToBytes(payload);
//
//    }
//
//    /**
//     * Returns the id of the packet
//     * @return the id of the packet
//     */
//    public int getId() {
//        return id;
//    }
//
//    /**
//     * Returns the payload of the packet
//     *
//     * @return the byte array that contains the payload of the packet
//     */
//    public byte[] getPayload() {
//        return payload;
//    }
//
//    @Override
//    public String toString() {
//        String ret = "Id: " + id + " Payload: ";
//        for (int k = 0; k < payload.length; k++) {
//            ret += String.format("%02X ", payload[k]);
//        }
//
//        return ret;
//    }
//
//	public UncodedPacket copy() {
//		UncodedPacket copy = new UncodedPacket(this.id, new byte[payload.length]);
//
//		System.arraycopy(payload, 0, copy.payload, 0, payload.length);
//
//		return copy;
//	}
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
