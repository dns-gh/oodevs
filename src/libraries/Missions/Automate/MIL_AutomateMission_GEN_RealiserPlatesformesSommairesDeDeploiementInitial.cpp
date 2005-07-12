// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:2 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::nDIAPlatesFormesIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial constructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial destructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::~MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAPlatesFormesIdx_ = DEC_Tools::InitializeDIAField( "platesFormes_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::Initialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial& asnMission = *asnMsg.mission.u.mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial;
    if( !NET_ASN_Tools::CopyGenObjectList( asnMission.plates_formes, GetVariable( nDIAPlatesFormesIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::Terminate()
{
    NET_ASN_Tools::ResetGenObjectList( GetVariable( nDIAPlatesFormesIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::Serialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial& asnMission = *new ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial;
    asnMsg.mission.u.mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial  = &asnMission;

    NET_ASN_Tools::CopyGenObjectList( GetVariable( nDIAPlatesFormesIdx_ ), asnMission.plates_formes );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_GEN_RealiserPlatesformesSommairesDeDeploiementInitial::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial );
    ASN1T_Mission_Automate_GEN_RealiserPlatesformesSommairesDeDeploiementInitial& asnMission = *asnMsg.mission.u.mission_automate_gen_realiser_plates_formes_sommaires_de_deploiement_initial;


    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
