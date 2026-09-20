// Automation tests for the op registry: every message id the enums declare is
// in the catalogue, ids are unique per scope and the schema records field names.
//   UnrealEditor-Cmd <project> -ExecCmds="Automation RunTests FlightForge.Api" -unattended -nopause

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "MessageSerialization/Public/Serialize.h"
#include "MessageSerialization/Public/serializable_schema.h"

/* catalogue //{ */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeApiCatalogueTest, "FlightForge.Api.Catalogue",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FFlightForgeApiCatalogueTest::RunTest(const FString& Parameters)
{
	using namespace Serializable;

	auto Meta = Schema::BuildMetaResponse();
	TestEqual(TEXT("api major"), Meta.api_version_major_, api_version_major);
	TestEqual(TEXT("api minor"), Meta.api_version_minor_, api_version_minor);
	TestEqual(TEXT("hash is deterministic"), Schema::BuildMetaResponse().catalogue_hash_, Meta.catalogue_hash_);

	TSet<uint32> Seen;
	for (const auto& Op : Meta.ops_)
	{
		const uint32 Key = (static_cast<uint32>(Op.scope_) << 16) | Op.id_;
		TestFalse(*FString::Printf(TEXT("duplicate id %d in scope %d"), Op.id_, Op.scope_), Seen.Contains(Key));
		Seen.Add(Key);
		TestTrue(*FString::Printf(TEXT("%hs has a request"), Op.name_.c_str()), Op.request_.size() > 2);
		TestTrue(*FString::Printf(TEXT("%hs has a response"), Op.name_.c_str()), Op.response_.size() > 2);
	}

	// the last enumerator of each MessageType; bump when adding an op so the range check covers it
	for (int Id = Drone::MessageType::get_location; Id <= Drone::MessageType::set_propeller_speeds; ++Id)
	{
		TestTrue(*FString::Printf(TEXT("drone op %d is registered"), Id), Seen.Contains((static_cast<uint32>(Registry::Scope::Drone) << 16) | Id));
	}
	for (int Id = GameMode::MessageType::get_drones; Id <= GameMode::MessageType::step; ++Id)
	{
		TestTrue(*FString::Printf(TEXT("game mode op %d is registered"), Id), Seen.Contains((static_cast<uint32>(Registry::Scope::GameMode) << 16) | Id));
	}

	const auto Rgb = Schema::Describe<Drone::GetRgbCameraData::Response>();
	TestEqual(TEXT("rgb response signature"), FString(Schema::Signature(Rgb).c_str()), FString(TEXT("{u16 type,bool status,u32 seq,vec<u8> image_,f64 stamp_}")));

	// TcpServer::Respond patches seq in at byte offset 3 of every response
	TestEqual(TEXT("response header layout"), FString(Schema::Signature(Schema::Describe<Common::NetworkResponse>()).c_str()), FString(TEXT("{u16 type,bool status,u32 seq}")));

	TestTrue(TEXT("no incompatibilities against itself"), Schema::Incompatibilities(Meta.ops_).empty());
	TestTrue(TEXT("json carries field names"), Meta.catalogue_json_.find("\"name\":\"sensor_id_\"") != std::string::npos);

	std::stringstream Stream;
	Serialization::DeserializeResponse(Meta, Stream);
	Common::GetMeta::Response Back;
	Serialization::SerializeRequest(Back, Stream);
	TestEqual(TEXT("meta round-trips"), Back.catalogue_hash_, Meta.catalogue_hash_);
	TestEqual(TEXT("meta round-trips ops"), Back.ops_.size(), Meta.ops_.size());

	return true;
}

//}

#endif
