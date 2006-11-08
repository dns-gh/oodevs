// *****************************************************************************
//
// $Created: $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"

#include "MIL_AutomateMission_ReleverSurPlace.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_ReleverSurPlace::nDIACompagnieAReleverIdx_ = 0 ;
int MIL_AutomateMission_ReleverSurPlace::nDIAZoneInstallationIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ReleverSurPlace constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ReleverSurPlace::MIL_AutomateMission_ReleverSurPlace( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ReleverSurPlace destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ReleverSurPlace::~MIL_AutomateMission_ReleverSurPlace()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ReleverSurPlace::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ReleverSurPlace::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIACompagnieAReleverIdx_ = DEC_Tools::InitializeDIAField( "compagnieARelever_", diaType );
    nDIAZoneInstallationIdx_ = DEC_Tools::InitializeDIAField( "zoneInstallation_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ReleverSurPlace::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ReleverSurPlace::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ReleverSurPlace& asnMission = *asnMsg.mission.u.mission_automate_relever_sur_place;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.compagnie_a_relever, GetVariable( nDIACompagnieAReleverIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_installation, GetVariable( nDIAZoneInstallationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ReleverSurPlace::Terminate()
{
    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ReleverSurPlace::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ReleverSurPlace::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ReleverSurPlace& asnMission = *new ASN1T_Mission_Automate_ReleverSurPlace();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_relever_sur_place;
    asnMsg.mission.u.mission_automate_relever_sur_place  = &asnMission;

    NET_ASN_Tools::CopyAutomate( GetVariable( nDIACompagnieAReleverIdx_ ), asnMission.compagnie_a_relever );
    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneInstallationIdx_ ), asnMission.zone_installation );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ReleverSurPlace::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ReleverSurPlace::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_relever_sur_place );
    ASN1T_Mission_Automate_ReleverSurPlace& asnMission = *asnMsg.mission.u.mission_automate_relever_sur_place;

    NET_ASN_Tools::Delete( asnMission.zone_installation );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
