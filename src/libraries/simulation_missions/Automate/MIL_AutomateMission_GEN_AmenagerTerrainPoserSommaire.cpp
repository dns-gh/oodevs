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

#include "MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire.h"
#include "simulation_kernel/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire::nDIATerrainIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire::MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire::~MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIATerrainIdx_ = DEC_Tools::InitializeDIAField( "terrain_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire& asnMission = *asnMsg.mission.u.mission_automate_gen_amenager_terrain_poser_sommaire;
    if( !NET_ASN_Tools::CopyGenObject( asnMission.terrain, GetVariable( nDIATerrainIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire::Terminate()
{
    NET_ASN_Tools::ResetGenObject( GetVariable( nDIATerrainIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire& asnMission = *new ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_gen_amenager_terrain_poser_sommaire;
    asnMsg.mission.u.mission_automate_gen_amenager_terrain_poser_sommaire  = &asnMission;

    NET_ASN_Tools::CopyGenObject( GetVariable( nDIATerrainIdx_ ), asnMission.terrain );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_AmenagerTerrainPoserSommaire::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_gen_amenager_terrain_poser_sommaire );
    ASN1T_Mission_Automate_GEN_AmenagerTerrainPoserSommaire& asnMission = *asnMsg.mission.u.mission_automate_gen_amenager_terrain_poser_sommaire;

    NET_ASN_Tools::Delete( asnMission.terrain );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
