
#include <mfapi.h>
#include "ipc.h"
#include "plugin.h"

#include "../hl2ss/server.h"
#include "../hl2ss/locator.h"
#include "../hl2ss/research_mode.h"
#include "../hl2ss/spatial_input.h"
#include "../hl2ss/personal_video.h"
#include "../hl2ss/spatial_mapping.h"
#include "../hl2ss/scene_understanding.h"
#include "../hl2ss/stream_rm.h"
#include "../hl2ss/stream_mc.h"
#include "../hl2ss/stream_pv.h"
#include "../hl2ss/stream_si.h"
#include "../hl2ss/ipc_rc.h"
#include "../hl2ss/ipc_sm.h"
#include "../hl2ss/ipc_su.h"
#include "../hl2ss/timestamps.h"
#include "../hl2ss/log.h"
#include "../hl2ss/nfo.h"
#include "../hl2ss/types.h"

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------

UNITY_EXPORT
void SetWorldCoordinateSystem(GUID guid, float* unityOriginToNodeTransform, bool useUnityOrigin)
{
    winrt::Windows::Foundation::Numerics::float4x4 originToNodeTrasform;
    {
        originToNodeTrasform.m11 = unityOriginToNodeTransform[0]; originToNodeTrasform.m21 = unityOriginToNodeTransform[4]; originToNodeTrasform.m31 = unityOriginToNodeTransform[8]; originToNodeTrasform.m41 = unityOriginToNodeTransform[12];
        originToNodeTrasform.m12 = unityOriginToNodeTransform[1]; originToNodeTrasform.m22 = unityOriginToNodeTransform[5]; originToNodeTrasform.m32 = unityOriginToNodeTransform[9]; originToNodeTrasform.m42 = unityOriginToNodeTransform[13];
        originToNodeTrasform.m13 = unityOriginToNodeTransform[2]; originToNodeTrasform.m23 = unityOriginToNodeTransform[6]; originToNodeTrasform.m33 = unityOriginToNodeTransform[10]; originToNodeTrasform.m43 = unityOriginToNodeTransform[14];
        originToNodeTrasform.m14 = unityOriginToNodeTransform[3]; originToNodeTrasform.m24 = unityOriginToNodeTransform[7]; originToNodeTrasform.m34 = unityOriginToNodeTransform[11]; originToNodeTrasform.m44 = unityOriginToNodeTransform[15];
    }
    Locator_Initialize(guid, originToNodeTrasform, useUnityOrigin);
}

// OK
UNITY_EXPORT
void InitializeStreams(uint32_t enable)
{
    InitializeSockets();
    MFStartup(MF_VERSION);

    if (enable & HL2SS_ENABLE_RM) { ResearchMode_Initialize(); }
    if (enable & HL2SS_ENABLE_PV) { PersonalVideo_Initialize(); }
    if (enable & HL2SS_ENABLE_SI) { SpatialInput_Initialize(); }
    if (enable & HL2SS_ENABLE_SM) { SpatialMapping_Initialize(); }
    if (enable & HL2SS_ENABLE_SU) { SceneUnderstanding_Initialize(); }

    if (enable & HL2SS_ENABLE_RM) { RM_Initialize(); }
    if (enable & HL2SS_ENABLE_PV) { PV_Initialize(); }
    if (enable & HL2SS_ENABLE_MC) { MC_Initialize(); }
    if (enable & HL2SS_ENABLE_SI) { SI_Initialize(); }
    if (enable & HL2SS_ENABLE_RC) { RC_Initialize(); }
    if (enable & HL2SS_ENABLE_SM) { SM_Initialize(); }
    if (enable & HL2SS_ENABLE_SU) { SU_Initialize(); }

    MQ_Initialize();
}

// OK
UNITY_EXPORT
void DebugMessage(char const* str)
{
    ShowMessage("%s", str);
}

// OK
UNITY_EXPORT
void SI_Update()
{
    SI_NotifyNextFrame(QPCTimestampToPerceptionTimestamp(GetCurrentQPCTimestamp()));
}

// OK
UNITY_EXPORT
void GetLocalIPv4Address(wchar_t *buffer, int size)
{
    std::vector<wchar_t> address;
    GetLocalIPv4Address(address);
    wcscpy_s(buffer, size / sizeof(wchar_t), address.data());
}
