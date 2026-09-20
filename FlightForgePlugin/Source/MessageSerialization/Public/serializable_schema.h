#pragma once

// Self-description of the wire: walks every op's Request/Response through a
// recording cereal archive and builds the catalogue served by get_meta. Pure
// C++, mirrored verbatim into the connector next to serializable_shared.h.

#include <array>
#include <cstdint>
#include <cstdio>
#include <string>
#include <type_traits>
#include <vector>

#include <cereal/cereal.hpp>

#include "serializable_shared.h"

namespace Serializable
{
namespace Schema
{

/* Node //{ */

struct Node
{
  std::string       name;   // empty for sequence elements
  std::string       type;   // bool i8..u64 f32 f64 str struct vec array
  std::size_t       count = 0;  // array length
  std::vector<Node> children;   // struct fields, or the one element prototype of vec/array
};

//}

/* Archive //{ */

// records field names (from CEREAL_NVP) and types instead of bytes
class Archive : public cereal::OutputArchive<Archive>
{
public:
  Archive() : cereal::OutputArchive<Archive>(this) {
    Root.type = "struct";
    Stack.push_back(&Root);
  }

  void Begin(const char* Type, std::size_t Count = 0) {
    Node& N = Add(Type);
    N.count = Count;
    Stack.push_back(&N);
  }
  void End() {
    Stack.pop_back();
  }
  void Leaf(const char* Type) {
    Add(Type);
  }

  Node        Root;
  std::string Pending;

private:
  Node& Add(const char* Type) {
    auto& Kids = Stack.back()->children;
    Kids.push_back(Node{});
    Node& N = Kids.back();
    N.name  = Pending;
    N.type  = Type;
    Pending.clear();
    return N;
  }

  std::vector<Node*> Stack;  // Stack[k] lives in Stack[k-1]->children, which only grows once k is popped
};

template <class T>
const char* TypeName() {
  if (std::is_same<T, bool>::value) return "bool";
  if (std::is_floating_point<T>::value) return sizeof(T) == 4 ? "f32" : "f64";
  switch (sizeof(T)) {
    case 1: return std::is_signed<T>::value ? "i8" : "u8";
    case 2: return std::is_signed<T>::value ? "i16" : "u16";
    case 4: return std::is_signed<T>::value ? "i32" : "u32";
    default: return std::is_signed<T>::value ? "i64" : "u64";
  }
}

template <class T>
typename std::enable_if<std::is_arithmetic<T>::value>::type save(Archive& ar, T const&) {
  ar.Leaf(TypeName<T>());
}

inline void save(Archive& ar, std::string const&) {
  ar.Leaf("str");
}

template <class T>
void save(Archive& ar, cereal::NameValuePair<T> const& nvp) {
  ar.Pending = nvp.name;
  ar(nvp.value);
}

template <class T, class A>
void save(Archive& ar, std::vector<T, A> const&) {
  ar.Begin("vec");
  T Proto{};
  ar(Proto);
  ar.End();
}

template <class T, std::size_t N>
void save(Archive& ar, std::array<T, N> const&) {
  ar.Begin("array", N);
  T Proto{};
  ar(Proto);
  ar.End();
}

template <class T>
typename std::enable_if<cereal::traits::has_member_serialize<T, Archive>::value>::type prologue(Archive& ar, T const&) {
  ar.Begin("struct");
}

template <class T>
typename std::enable_if<cereal::traits::has_member_serialize<T, Archive>::value>::type epilogue(Archive& ar, T const&) {
  ar.End();
}

template <class T>
Node Describe() {
  Archive ar;
  T       Value{};
  ar(Value);
  return ar.Root.children.at(0);
}

//}

/* rendering //{ */

// canonical form the hashes are taken over: {u16 type,bool status,vec<u8> image_}
inline std::string Signature(const Node& N) {
  if (N.type == "struct") {
    std::string S = "{";
    for (std::size_t i = 0; i < N.children.size(); ++i) {
      S += (i ? "," : "") + Signature(N.children[i]) + " " + N.children[i].name;
    }
    return S + "}";
  }
  if (N.type == "vec") return "vec<" + Signature(N.children.at(0)) + ">";
  if (N.type == "array") return "array<" + Signature(N.children.at(0)) + "," + std::to_string(N.count) + ">";
  return N.type;
}

inline std::string Json(const Node& N) {
  std::string S = "{";
  if (!N.name.empty()) S += "\"name\":\"" + N.name + "\",";
  S += "\"type\":\"" + N.type + "\"";
  if (N.type == "struct") {
    S += ",\"fields\":[";
    for (std::size_t i = 0; i < N.children.size(); ++i) S += (i ? "," : "") + Json(N.children[i]);
    S += "]";
  } else if (N.type == "vec" || N.type == "array") {
    if (N.type == "array") S += ",\"count\":" + std::to_string(N.count);
    S += ",\"elem\":" + Json(N.children.at(0));
  }
  return S + "}";
}

inline uint64_t Fnv1a(const std::string& S, uint64_t H = 14695981039346656037ull) {
  for (unsigned char C : S) {
    H ^= C;
    H *= 1099511628211ull;
  }
  return H;
}

inline std::string Hex(uint64_t V) {
  char Buf[17];
  std::snprintf(Buf, sizeof(Buf), "%016llx", static_cast<unsigned long long>(V));
  return Buf;
}

//}

/* catalogue //{ */

struct OpSchema
{
  Registry::Scope scope;
  unsigned short  id;
  std::string     name;
  Node            request;
  Node            response;
  Common::OpMeta  meta;  // the wire form: signatures + hash
};

inline const char* ScopeName(Registry::Scope S) {
  switch (S) {
    case Registry::Scope::Common: return "common";
    case Registry::Scope::Drone: return "drone";
    default: return "game_mode";
  }
}

inline std::vector<OpSchema> BuildCatalogue() {
  std::vector<OpSchema> Ops;
  Registry::ForEachOp([&](auto Op) {
    using T = decltype(Op);
    OpSchema S;
    S.scope          = Op.scope;
    S.id             = Op.id;
    S.name           = Op.name;
    S.request        = Describe<typename T::Request>();
    S.response       = Describe<typename T::Response>();
    S.meta.scope_    = static_cast<unsigned char>(Op.scope);
    S.meta.id_       = Op.id;
    S.meta.name_     = Op.name;
    S.meta.request_  = Signature(S.request);
    S.meta.response_ = Signature(S.response);
    S.meta.hash_     = Fnv1a(std::string(ScopeName(Op.scope)) + ":" + std::to_string(Op.id) + ":" + S.name + S.meta.request_ + "->" + S.meta.response_);
    Ops.push_back(std::move(S));
  });
  return Ops;
}

inline uint64_t CatalogueHash(const std::vector<Common::OpMeta>& Ops) {
  uint64_t H = 14695981039346656037ull;
  for (const auto& Op : Ops) H = Fnv1a(Hex(Op.hash_), H);
  return H;
}

inline std::string CatalogueJson(const std::vector<OpSchema>& Ops, uint64_t Hash) {
  std::string S = "{\"api\":[" + std::to_string(api_version_major) + "," + std::to_string(api_version_minor) + "],\"hash\":\"" + Hex(Hash) + "\",\"ops\":[";
  for (std::size_t i = 0; i < Ops.size(); ++i) {
    const auto& Op = Ops[i];
    S += std::string(i ? "," : "") + "{\"scope\":\"" + ScopeName(Op.scope) + "\",\"id\":" + std::to_string(Op.id) + ",\"name\":\"" + Op.name +
         "\",\"hash\":\"" + Hex(Op.meta.hash_) + "\",\"request\":" + Json(Op.request) + ",\"response\":" + Json(Op.response) + "}";
  }
  return S + "]}";
}

inline Common::GetMeta::Response BuildMetaResponse() {
  Common::GetMeta::Response R(true);
  R.api_version_major_ = api_version_major;
  R.api_version_minor_ = api_version_minor;
  auto Ops             = BuildCatalogue();
  for (const auto& Op : Ops) R.ops_.push_back(Op.meta);
  R.catalogue_hash_ = CatalogueHash(R.ops_);
  R.catalogue_json_ = CatalogueJson(Ops, R.catalogue_hash_);
  return R;
}

// client side: which of the ops this header knows (all, or only Names) the server lacks or serializes differently
inline std::vector<std::string> Incompatibilities(const std::vector<Common::OpMeta>& Remote, const std::vector<std::string>& Names = {}) {
  std::vector<std::string> Out;
  for (const auto& Local : BuildMetaResponse().ops_) {
    bool Wanted = Names.empty();
    for (const auto& N : Names) Wanted |= N == Local.name_;
    if (!Wanted) continue;
    const Common::OpMeta* Match = nullptr;
    for (const auto& R : Remote) {
      if (R.scope_ == Local.scope_ && R.id_ == Local.id_) Match = &R;
    }
    if (!Match) {
      Out.push_back(Local.name_ + ": missing on the server");
    } else if (Match->hash_ != Local.hash_) {
      Out.push_back(Local.name_ + ": schema differs, local " + Local.request_ + "->" + Local.response_ + " vs server " + Match->request_ + "->" + Match->response_);
    }
  }
  return Out;
}

//}

}  // namespace Schema
}  // namespace Serializable
