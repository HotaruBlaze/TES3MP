#include <components/openmw-mp/NetworkMessages.hpp>
#include "PacketObjectPlace.hpp"

using namespace mwmp;

PacketObjectPlace::PacketObjectPlace(RakNet::RakPeerInterface *peer) : ObjectPacket(peer)
{
    packetID = ID_OBJECT_PLACE;
    hasCellData = true;
}

void PacketObjectPlace::Object(WorldObject &worldObject, bool send)
{
    ObjectPacket::Object(worldObject, send);
    RW(worldObject.count, send);
    RW(worldObject.charge, send);
    RW(worldObject.enchantmentCharge, send);
    RW(worldObject.goldValue, send);
    RW(worldObject.position, send);
    RW(worldObject.droppedByPlayer, send);
}