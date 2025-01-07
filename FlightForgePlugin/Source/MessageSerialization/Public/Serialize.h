#pragma once

#include "Modules/ModuleManager.h"
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>

class MESSAGESERIALIZATION_API Serialization : public IModuleInterface {

public:
  template <typename TRequest>
  static void SerializeRequest(TRequest& Request, std::stringstream& InputStream)
  {
    InputStream.seekg(0);
    cereal::BinaryInputArchive ia(InputStream);
    ia(Request);
  }

  template <typename TResponse>
  static void DeserializeResponse(TResponse& Response, std::stringstream& OutputStream)
  {
    cereal::BinaryOutputArchive oa(OutputStream);
    oa(Response);
  }
  
};