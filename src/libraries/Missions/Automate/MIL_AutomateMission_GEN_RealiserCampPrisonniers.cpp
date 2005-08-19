// *****************************************************************************
//
// $Created: 2005-08-19 - 11:30:55 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_GEN_RealiserCampPrisonniers.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_GEN_RealiserCampPrisonniers::nDIACampIdx_ = 0 ;
int MIL_AutomateMission_GEN_RealiserCampPrisonniers::nDIATc2Idx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserCampPrisonniers constructor
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_RealiserCampPrisonniers::MIL_AutomateMission_GEN_RealiserCampPrisonniers( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserCampPrisonniers destructor
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_RealiserCampPrisonniers::~MIL_AutomateMission_GEN_RealiserCampPrisonniers()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserCampPrisonniers::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_GEN_RealiserCampPrisonniers::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIACampIdx_ = DEC_Tools::InitializeDIAField( "camp_", diaType );
    nDIATc2Idx_ = DEC_Tools::InitializeDIAField( "tc2_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserCampPrisonniers::Initialize
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_GEN_RealiserCampPrisonniers::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers& asnMission = *asnMsg.mission.u.mission_automate_gen_realiser_camp_prisonniers;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.camp, camp_, GetVariable( nDIACampIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyAutomate( asnMission.tc2, GetVariable( nDIATc2Idx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserCampPrisonniers::Terminate()
{
    NET_ASN_Tools::ResetAutomate( GetVariable( nDIATc2Idx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserCampPrisonniers::Serialize
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserCampPrisonniers::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_gen_realiser_camp_prisonniers;
    asnMsg.mission.u.mission_automate_gen_realiser_camp_prisonniers  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIACampIdx_ ), asnMission.camp );
    NET_ASN_Tools::CopyAutomate( GetVariable( nDIATc2Idx_ ), asnMission.tc2 );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserCampPrisonniers::CleanAfterSerialization
// Created: 2005-08-19 - 11:30:55
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserCampPrisonniers::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_gen_realiser_camp_prisonniers );
    ASN1T_Mission_Automate_GEN_RealiserCampPrisonniers& asnMission = *asnMsg.mission.u.mission_automate_gen_realiser_camp_prisonniers;

    NET_ASN_Tools::Delete( asnMission.camp );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
