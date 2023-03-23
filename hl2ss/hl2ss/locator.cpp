
#include "types.h"
#include "log.h"

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Numerics.h>
#include <winrt/Windows.Perception.h>
#include <winrt/Windows.Perception.Spatial.h>
#include <winrt/Windows.Perception.Spatial.Preview.h>
using namespace winrt::Windows::Perception::Spatial::Preview;

using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Windows::Perception;
using namespace winrt::Windows::Perception::Spatial;

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

static SpatialLocator g_locator = nullptr;
static SpatialLocatability g_locatability = SpatialLocatability::Unavailable;
static SpatialStationaryFrameOfReference g_referenceFrame = nullptr;
static SpatialCoordinateSystem g_referenceFrameCoordinateSystem = nullptr;
static SpatialLocatorAttachedFrameOfReference g_attachedReferenceFrame = nullptr;

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

// OK
static void Locator_OnLocatabilityChanged(winrt::Windows::Perception::Spatial::SpatialLocator const& locator, winrt::Windows::Foundation::IInspectable const&)
{
    g_locatability = locator.Locatability();
}

// OK
void Locator_Initialize(winrt::guid guid, winrt::Windows::Foundation::Numerics::float4x4 unityOriginToNodeTransform, bool useUnityOrigin)
{
    if (useUnityOrigin)
    {
        float3 position = float3(unityOriginToNodeTransform.m41, unityOriginToNodeTransform.m42, unityOriginToNodeTransform.m43);
        quaternion rotation = make_quaternion_from_rotation_matrix(unityOriginToNodeTransform);
        g_referenceFrameCoordinateSystem = SpatialGraphInteropPreview::CreateCoordinateSystemForNode(guid, position, rotation);
        ShowMessage("World origin: using unity");
    }
    else
    {
        g_locator = SpatialLocator::GetDefault();
        g_locator.LocatabilityChanged(Locator_OnLocatabilityChanged);
        g_locatability = g_locator.Locatability();
        g_referenceFrame = g_locator.CreateStationaryFrameOfReferenceAtCurrentLocation();
        g_attachedReferenceFrame = g_locator.CreateAttachedFrameOfReferenceAtCurrentHeading();
        ShowMessage("World origin: using WMR locator");
    }
}

// OK
float4x4 Locator_Locate(PerceptionTimestamp const& timestamp, SpatialLocator const& locator, SpatialCoordinateSystem const& world)
{
    auto location = locator.TryLocateAtTimestamp(timestamp, world);
    return location ? (make_float4x4_from_quaternion(location.Orientation()) * make_float4x4_translation(location.Position())) : float4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

// OK
float4x4 Locator_GetTransformTo(SpatialCoordinateSystem const& src, SpatialCoordinateSystem const& dst)
{
    auto location = src.TryGetTransformTo(dst);
    return location ? location.Value() : float4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

// OK
SpatialCoordinateSystem Locator_GetWorldCoordinateSystem(PerceptionTimestamp const& ts)
{
    return (g_referenceFrameCoordinateSystem != nullptr) ? g_referenceFrameCoordinateSystem : g_attachedReferenceFrame.GetStationaryCoordinateSystemAtTimestamp(ts);
}
