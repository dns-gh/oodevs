// *****************************************************************************
//
// $Created: 2005-08-17 - 16:30:43 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation::nDIATravauxIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation constructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation::MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation destructor
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation::~MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIATravauxIdx_ = DEC_Tools::InitializeDIAField( "travaux_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation::Initialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation& asnMission = *asnMsg.mission.u.mission_automate_gen_realiser_travaux_sommaires_au_profit_population;
    if( !NET_ASN_Tools::CopyGenObjectList( asnMission.travaux, GetVariable( nDIATravauxIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation::Terminate()
{
    NET_ASN_Tools::ResetGenObjectList( GetVariable( nDIATravauxIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation::Serialize
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_gen_realiser_travaux_sommaires_au_profit_population;
    asnMsg.mission.u.mission_automate_gen_realiser_travaux_sommaires_au_profit_population  = &asnMission;

    NET_ASN_Tools::CopyGenObjectList( GetVariable( nDIATravauxIdx_ ), asnMission.travaux );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation::CleanAfterSerialization
// Created: 2005-08-17 - 16:30:43
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserTravauxSommairesAuProfitPopulation::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_gen_realiser_travaux_sommaires_au_profit_population );
    ASN1T_Mission_Automate_GEN_RealiserTravauxSommairesAuProfitPopulation& asnMission = *asnMsg.mission.u.mission_automate_gen_realiser_travaux_sommaires_au_profit_population;

    NET_ASN_Tools::Delete( asnMission.travaux );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
