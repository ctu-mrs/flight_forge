// Automation tests for the pure math that only exists engine-side: frame
// conversions, spawn-config YAML round-trip, intrinsics, the distortion LUT
// inverse and noise determinism. Run from the editor's Automation tab or:
//   UnrealEditor-Cmd <project> -ExecCmds="Automation RunTests FlightForge" -unattended -nopause

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "FlightForgeCore/Sensors/Camera.h"
#include "FlightForgeCore/Sensors/FisheyeCamera.h"
#include "FlightForgeCore/Utils/SpawnConfig.h"

/* spawn transform inverse //{ */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeSpawnTransformInverseTest, "FlightForge.Math.SpawnTransformInverse",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FFlightForgeSpawnTransformInverseTest::RunTest(const FString& Parameters)
{
	const FVector WorldOrigin(1234.0, -567.0, 89.0);

	const FVector Positions[]    = {FVector::ZeroVector, FVector(5.0, -2.0, 1.5), FVector(-10.0, 10.0, 0.25)};
	const FVector Orientations[] = {FVector::ZeroVector, FVector(10.0, -20.0, 45.0), FVector(-5.0, 15.0, -120.0)};

	for (const FVector& Position : Positions)
	{
		for (const FVector& Orientation : Orientations)
		{
			FSpawnObjectEntry Entry;
			Entry.Position    = Position;
			Entry.Orientation = Orientation;
			Entry.Scale       = FVector(2.0, 0.5, 1.0);

			const FTransform Transform = USpawnConfigUtils::ToUnrealTransform(Entry, FVector::ZeroVector, WorldOrigin);

			FSpawnObjectEntry Recovered;
			USpawnConfigUtils::FromUnrealTransform(Transform, WorldOrigin, Recovered);

			TestTrue(TEXT("position recovered"), Recovered.Position.Equals(Entry.Position, 1.0e-4));
			TestTrue(TEXT("scale recovered"), Recovered.Scale.Equals(Entry.Scale, 1.0e-4));

			// orientations may come back as an equivalent rotation; compare quaternions
			const FTransform Again = USpawnConfigUtils::ToUnrealTransform(Recovered, FVector::ZeroVector, WorldOrigin);
			TestTrue(TEXT("orientation recovered"),
			         Again.GetRotation().AngularDistance(Transform.GetRotation()) < 1.0e-4);
		}
	}

	return true;
}

/* //} */

/* spawn yaml round-trip //{ */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeSpawnYamlRoundTripTest, "FlightForge.Math.SpawnYamlRoundTrip",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FFlightForgeSpawnYamlRoundTripTest::RunTest(const FString& Parameters)
{
	FSpawnConfig Config;
	Config.SearchPaths = {TEXT("/Game/Worlds")};

	FSpawnObjectEntry Entry;
	Entry.Asset       = TEXT("Cube");
	Entry.Position    = FVector(5.25, -2.5, 1.0);
	Entry.Orientation = FVector(10.0, 0.0, 45.0);
	Entry.Scale       = FVector(2.0, 2.0, 0.5);
	Entry.Stencil     = 203;
	Config.Objects.Add(Entry);

	const FString Yaml = USpawnConfigUtils::WriteSpawnConfig(Config);

	FSpawnConfig Parsed;
	FString      Error;
	TestTrue(TEXT("written yaml parses"), USpawnConfigUtils::ParseSpawnConfig(Yaml, Parsed, Error));
	TestEqual(TEXT("no parse error"), Error, FString());

	if (Parsed.Objects.Num() == 1)
	{
		const FSpawnObjectEntry& Out = Parsed.Objects[0];
		TestEqual(TEXT("asset"), Out.Asset, Entry.Asset);
		TestTrue(TEXT("position"), Out.Position.Equals(Entry.Position, 1.0e-6));
		TestTrue(TEXT("orientation"), Out.Orientation.Equals(Entry.Orientation, 1.0e-6));
		TestTrue(TEXT("scale"), Out.Scale.Equals(Entry.Scale, 1.0e-6));
		TestEqual(TEXT("stencil"), Out.Stencil, Entry.Stencil);
	}
	else
	{
		AddError(TEXT("expected exactly one parsed object"));
	}

	return true;
}

/* //} */

/* intrinsics //{ */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeIntrinsicsTest, "FlightForge.Math.Intrinsics",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FFlightForgeIntrinsicsTest::RunTest(const FString& Parameters)
{
	// 90 deg horizontal FOV at 640 wide: fx = 320 / tan(45 deg) = 320
	const FCameraIntrinsics Derived = UCamera::EffectiveIntrinsics(FCameraIntrinsics{}, 90.0, 640, 480);
	TestTrue(TEXT("fx from fov"), FMath::IsNearlyEqual(Derived.Fx, 320.0, 1.0e-6));
	TestTrue(TEXT("fy equals fx"), FMath::IsNearlyEqual(Derived.Fy, Derived.Fx, 1.0e-9));
	TestTrue(TEXT("centered principal point"), FMath::IsNearlyEqual(Derived.Cx, 320.0, 1.0e-9) && FMath::IsNearlyEqual(Derived.Cy, 240.0, 1.0e-9));

	// custom values pass through untouched
	FCameraIntrinsics Custom;
	Custom.UseCustom = true;
	Custom.Fx        = 500.0;
	Custom.Fy        = 505.0;
	Custom.Cx        = 300.0;
	Custom.Cy        = 250.0;
	const FCameraIntrinsics Passed = UCamera::EffectiveIntrinsics(Custom, 90.0, 640, 480);
	TestTrue(TEXT("custom passthrough"), Passed.Fx == 500.0 && Passed.Fy == 505.0 && Passed.Cx == 300.0 && Passed.Cy == 250.0);

	// centered custom matrix must match the engine's reversed-Z perspective matrix
	const FMatrix Ours   = UCamera::IntrinsicsProjectionMatrix(Derived, 640, 480);
	const FMatrix Engine = FReversedZPerspectiveMatrix(FMath::DegreesToRadians(45.0f), 640.0f, 480.0f, GNearClippingPlane);
	TestTrue(TEXT("centered matrix matches FReversedZPerspectiveMatrix"), Ours.Equals(Engine, 1.0e-4));

	return true;
}

/* //} */

/* distortion LUT //{ */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeDistortionLutTest, "FlightForge.Math.DistortionLut",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FFlightForgeDistortionLutTest::RunTest(const FString& Parameters)
{
	const int32             Width  = 64;
	const int32             Height = 48;
	const FCameraIntrinsics K      = UCamera::EffectiveIntrinsics(FCameraIntrinsics{}, 90.0, Width, Height);

	// zero coefficients: the LUT must be the identity (up to the pad shift)
	{
		FCameraDistortion Zero;
		Zero.Enable = true;

		const auto LUT = UCamera::BuildDistortionLUT(Zero, K, Width, Height);
		TestTrue(TEXT("zero-coefficient LUT built"), LUT.IsValid());

		if (LUT.IsValid())
		{
			bool bIdentity = true;
			for (int32 V = 0; V < Height && bIdentity; ++V)
			{
				for (int32 U = 0; U < Width; ++U)
				{
					const FVector2f Source = LUT->Source[V * Width + U];
					if (!FMath::IsNearlyEqual(Source.X - LUT->Pad.X, (float)U, 1.0e-3f) || !FMath::IsNearlyEqual(Source.Y - LUT->Pad.Y, (float)V, 1.0e-3f))
					{
						bIdentity = false;
						break;
					}
				}
			}
			TestTrue(TEXT("zero-coefficient LUT is the identity"), bIdentity);
		}
	}

	// real coefficients: pushing the LUT's pinhole coordinate through the FORWARD
	// model must land back on the output pixel (validates the iterative inverse)
	{
		FCameraDistortion Distortion;
		Distortion.Enable = true;
		Distortion.K1     = -0.28;
		Distortion.K2     = 0.07;
		Distortion.P1     = 0.001;
		Distortion.P2     = -0.0005;

		const auto LUT = UCamera::BuildDistortionLUT(Distortion, K, Width, Height);
		TestTrue(TEXT("LUT built"), LUT.IsValid());

		if (LUT.IsValid())
		{
			double MaxError = 0.0;

			for (int32 V = 0; V < Height; ++V)
			{
				for (int32 U = 0; U < Width; ++U)
				{
					const FVector2f Source = LUT->Source[V * Width + U];

					const double Xu = (Source.X + 0.5 - LUT->Pad.X - K.Cx) / K.Fx;
					const double Yu = (Source.Y + 0.5 - LUT->Pad.Y - K.Cy) / K.Fy;

					const double R2     = Xu * Xu + Yu * Yu;
					const double Radial = 1.0 + R2 * (Distortion.K1 + R2 * (Distortion.K2 + R2 * Distortion.K3));
					const double Xd     = Xu * Radial + 2.0 * Distortion.P1 * Xu * Yu + Distortion.P2 * (R2 + 2.0 * Xu * Xu);
					const double Yd     = Yu * Radial + Distortion.P1 * (R2 + 2.0 * Yu * Yu) + 2.0 * Distortion.P2 * Xu * Yu;

					const double BackU = Xd * K.Fx + K.Cx - 0.5;
					const double BackV = Yd * K.Fy + K.Cy - 0.5;

					MaxError = FMath::Max(MaxError, FMath::Max(FMath::Abs(BackU - U), FMath::Abs(BackV - V)));
				}
			}

			TestTrue(FString::Printf(TEXT("forward model recovers every pixel (max error %f px)"), MaxError), MaxError < 1.0e-2);
		}
	}

	return true;
}

/* //} */

/* noise determinism //{ */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeNoiseDeterminismTest, "FlightForge.Math.NoiseDeterminism",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FFlightForgeNoiseDeterminismTest::RunTest(const FString& Parameters)
{
	const FIntPoint Size(32, 24);

	TArray<FColor> Base;
	Base.Init(FColor(120, 90, 200, 255), Size.X * Size.Y);

	FCameraNoise Noise;
	Noise.Enable    = true;
	Noise.ShotScale = 3.0e-4;
	Noise.ReadSigma = 4.0e-3;
	Noise.RowSigma  = 1.0e-3;

	TArray<FColor> A = Base;
	TArray<FColor> B = Base;
	TArray<FColor> C = Base;

	UCamera::ApplyNoise(A, Size, Noise, 12345);
	UCamera::ApplyNoise(B, Size, Noise, 12345);
	UCamera::ApplyNoise(C, Size, Noise, 54321);

	TestTrue(TEXT("same seed is bit-identical"), A == B);
	TestTrue(TEXT("different seed differs"), !(A == C));
	TestTrue(TEXT("noise actually perturbs"), !(A == Base));

	return true;
}

/* //} */

/* camera info //{ */

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeCameraInfoTest, "FlightForge.Math.CameraInfo",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FFlightForgeCameraInfoTest::RunTest(const FString& Parameters)
{
	// the fisheye focal puts theta = fov/2 exactly on the image circle for every lens model
	const double FovDeg  = 180.0;
	const double HalfFov = FMath::DegreesToRadians(FovDeg) * 0.5;
	const int32  W = 640, H = 480;
	const double Radius = 0.5 * H;

	const double Fe = UFisheyeCamera::FocalPx(EFisheyeLensModel::EQUIDISTANT, FovDeg, W, H, false);
	TestEqual(TEXT("equidistant edge"), Fe * HalfFov, Radius, 1.0e-9);

	const double Fs = UFisheyeCamera::FocalPx(EFisheyeLensModel::EQUISOLID, FovDeg, W, H, false);
	TestEqual(TEXT("equisolid edge"), 2.0 * Fs * FMath::Sin(HalfFov * 0.5), Radius, 1.0e-9);

	const double Fg = UFisheyeCamera::FocalPx(EFisheyeLensModel::STEREOGRAPHIC, FovDeg, W, H, false);
	TestEqual(TEXT("stereographic edge"), 2.0 * Fg * FMath::Tan(HalfFov * 0.5), Radius, 1.0e-9);

	const double Ff = UFisheyeCamera::FocalPx(EFisheyeLensModel::EQUIDISTANT, FovDeg, W, H, true);
	TestEqual(TEXT("full-frame edge is the diagonal"), Ff * HalfFov, 0.5 * FMath::Sqrt((double)(W * W + H * H)), 1.0e-9);

	// pinhole: FOV -> fx, OpenCV pixel convention, D in plumb_bob order
	FCameraDistortion D;
	D.Enable = true;
	D.K1 = 0.1; D.K2 = 0.02; D.K3 = 0.003; D.P1 = 0.0004; D.P2 = 0.00005;
	const auto Info = UCamera::PinholeInfo(3, SensorType::RGB_CAMERA, FCameraIntrinsics{}, D, 90.0, W, H, FVector(10.0, 0.0, -5.0), FRotator(-30.0, 0.0, 0.0));
	TestEqual(TEXT("fx from fov"), Info.fx_, (W * 0.5) / FMath::Tan(FMath::DegreesToRadians(45.0)), 1.0e-9);
	TestEqual(TEXT("fov round-trips"), Info.fov_deg_, 90.0, 1.0e-9);
	TestEqual(TEXT("cx is the image centre in OpenCV convention"), Info.cx_, W * 0.5 - 0.5, 1.0e-9);
	TestEqual(TEXT("cy likewise"), Info.cy_, H * 0.5 - 0.5, 1.0e-9);
	TestEqual(TEXT("D size"), (int32)Info.d_.size(), 5);
	TestTrue(TEXT("D is k1 k2 p1 p2 k3"), Info.d_[0] == 0.1 && Info.d_[2] == 0.0004 && Info.d_[3] == 0.00005 && Info.d_[4] == 0.003);
	TestEqual(TEXT("projection"), FString(Info.projection_.c_str()), FString(TEXT("pinhole")));
	TestEqual(TEXT("distortion model"), FString(Info.distortion_model_.c_str()), FString(TEXT("plumb_bob")));
	TestEqual(TEXT("pitch"), Info.rotation_pitch_, -30.0, 1.0e-9);

	// custom K: reported as-is (wire convention), fov is what fx implies, D present but zero
	FCameraIntrinsics K;
	K.UseCustom = true;
	K.Fx = 500.0; K.Fy = 510.0; K.Cx = 300.5; K.Cy = 250.5;
	const auto Custom = UCamera::PinholeInfo(3, SensorType::RGB_CAMERA, K, FCameraDistortion{}, 90.0, W, H, FVector::ZeroVector, FRotator::ZeroRotator);
	TestEqual(TEXT("custom fy"), Custom.fy_, 510.0, 1.0e-9);
	TestEqual(TEXT("custom cx back in wire convention"), Custom.cx_, 300.0, 1.0e-9);
	TestEqual(TEXT("custom fov"), Custom.fov_deg_, FMath::RadiansToDegrees(2.0 * FMath::Atan(320.0 / 500.0)), 1.0e-9);
	TestEqual(TEXT("zero D without distortion"), (int32)Custom.d_.size(), 5);
	TestTrue(TEXT("zero D values"), Custom.d_[0] == 0.0 && Custom.d_[4] == 0.0);

	return true;
}

//}

/* fisheye calibration vs the LUT model //{ */

// Projects a ray through a reported CameraInfo the way OpenCV's fisheye::projectPoints does
// (theta-polynomial on the equidistant model, OpenCV pixel convention).
static FVector2D ProjectKannalaBrandt(const Serializable::Drone::CameraInfo& Info, const double Theta, const double Phi)
{
	const double T2 = Theta * Theta;
	const double Td = Theta * (1.0 + Info.d_[0] * T2 + Info.d_[1] * T2 * T2 + Info.d_[2] * T2 * T2 * T2 + Info.d_[3] * T2 * T2 * T2 * T2);
	return FVector2D(Info.cx_ + Info.fx_ * Td * FMath::Cos(Phi), Info.cy_ + Info.fy_ * Td * FMath::Sin(Phi));
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFlightForgeFisheyeCalibrationTest, "FlightForge.Math.FisheyeCalibration",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FFlightForgeFisheyeCalibrationTest::RunTest(const FString& Parameters)
{
	struct FCase
	{
		const TCHAR*         Name;
		FFisheyeCameraConfig Config;
		double               MaxErrorPx;  // 0 for models KB carries exactly, series truncation otherwise
	};

	FFisheyeCameraConfig Base;
	Base.FOVAngle  = 190.0;
	Base.Width     = 800;
	Base.Height    = 600;
	Base.LensModel = EFisheyeLensModel::EQUIDISTANT;

	TArray<FCase> Cases;
	Cases.Add({TEXT("equidistant"), Base, 1.0e-6});

	FCase Solid{TEXT("equisolid"), Base, 0.05};
	Solid.Config.LensModel = EFisheyeLensModel::EQUISOLID;
	Cases.Add(Solid);

	FCase Stereo{TEXT("stereographic"), Base, 0.5};
	Stereo.Config.LensModel = EFisheyeLensModel::STEREOGRAPHIC;
	Cases.Add(Stereo);

	FCase FullFrame{TEXT("equidistant full-frame"), Base, 1.0e-6};
	FullFrame.Config.FullFrame = true;
	Cases.Add(FullFrame);

	// a Kalibr-style calibration: fx != fy, off-centre principal point, real-looking coefficients
	FCase Kb{TEXT("kannala-brandt custom"), Base, 1.0e-6};
	Kb.Config.LensModel            = EFisheyeLensModel::KANNALA_BRANDT;
	Kb.Config.Intrinsics.UseCustom = true;
	Kb.Config.Intrinsics.Fx        = 254.3;
	Kb.Config.Intrinsics.Fy        = 252.9;
	Kb.Config.Intrinsics.Cx        = 405.7 + WirePixelShift;  // engine convention, as SetConfig stores it
	Kb.Config.Intrinsics.Cy        = 291.2 + WirePixelShift;
	Kb.Config.K1                   = -0.012;
	Kb.Config.K2                   = 0.021;
	Kb.Config.K3                   = -0.017;
	Kb.Config.K4                   = 0.004;
	Cases.Add(Kb);

	FCase CustomEqui{TEXT("equidistant custom K"), Base, 1.0e-6};
	CustomEqui.Config.Intrinsics = Kb.Config.Intrinsics;
	Cases.Add(CustomEqui);

	for (const FCase& Case : Cases)
	{
		const FFisheyeCameraConfig&           C    = Case.Config;
		const FCameraIntrinsics               K    = UFisheyeCamera::EffectiveFisheyeIntrinsics(C);
		const Serializable::Drone::CameraInfo Info = UFisheyeCamera::FisheyeInfo(C);

		TestEqual(*FString::Printf(TEXT("%s: four KB coefficients"), Case.Name), (int32)Info.d_.size(), 4);
		TestEqual(*FString::Printf(TEXT("%s: distortion model"), Case.Name), FString(Info.distortion_model_.c_str()), FString(TEXT("equidistant")));
		if (C.Intrinsics.UseCustom)
		{
			TestEqual(*FString::Printf(TEXT("%s: custom cx round-trips to wire convention"), Case.Name), Info.cx_, 405.7, 1.0e-9);
		}

		double MaxError = 0.0;
		int32  Rendered = 0;
		for (int32 py = 0; py < C.Height; py += 7)
		{
			for (int32 px = 0; px < C.Width; px += 7)
			{
				double Theta, Phi;
				if (!UFisheyeCamera::PixelToAngles(C, K, px + 0.5, py + 0.5, Theta, Phi))
				{
					continue;
				}
				++Rendered;
				const FVector2D Back = ProjectKannalaBrandt(Info, Theta, Phi);
				MaxError            = FMath::Max(MaxError, FVector2D::Distance(Back, FVector2D(px, py)));
			}
		}

		AddInfo(FString::Printf(TEXT("%s: %d pixels, max reprojection error through the reported calibration %.3g px"), Case.Name, Rendered, MaxError));
		TestTrue(*FString::Printf(TEXT("%s: renders pixels"), Case.Name), Rendered > 1000);
		TestTrue(*FString::Printf(TEXT("%s: reported calibration matches the LUT model (%.3g px)"), Case.Name, MaxError), MaxError <= Case.MaxErrorPx);
	}

	// the rendered angle is capped by fov even with custom intrinsics
	double Theta, Phi;
	const FCameraIntrinsics KbK = UFisheyeCamera::EffectiveFisheyeIntrinsics(Kb.Config);
	TestFalse(TEXT("beyond fov/2 is not rendered"), UFisheyeCamera::PixelToAngles(Kb.Config, KbK, KbK.Cx + 2000.0, KbK.Cy, Theta, Phi));

	return true;
}

//}

#endif  // WITH_DEV_AUTOMATION_TESTS
