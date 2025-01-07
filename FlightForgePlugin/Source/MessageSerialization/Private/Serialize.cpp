#include "Serialize.h"

// template void Serialization::SerializeRequest(Serializable::Common::NetworkRequest&, std::stringstream&);
//
// template void Serialization::SerializeRequest(Serializable::Drone::GetLocation::Request&, std::stringstream&);
//
// template void Serialization::SerializeRequest(Serializable::GameMode::GetDrones::Request&, std::stringstream&);
// template void Serialization::SerializeRequest(Serializable::GameMode::SpawnDrone::Request&, std::stringstream&);


IMPLEMENT_MODULE(Serialization, MessageSerialization)
