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

#include "MIL_AutomateMission_ABC_SurveillerItineraire.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_ABC_SurveillerItineraire::nDIAItineraireIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_SurveillerItineraire constructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_SurveillerItineraire::MIL_AutomateMission_ABC_SurveillerItineraire( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_SurveillerItineraire destructor
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_SurveillerItineraire::~MIL_AutomateMission_ABC_SurveillerItineraire()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_SurveillerItineraire::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ABC_SurveillerItineraire::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_SurveillerItineraire::Initialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC_SurveillerItineraire::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ABC_SurveillerItineraire& asnMission = *asnMsg.mission.u.mission_automate_abc_surveiller_itineraire;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire, itineraire_, GetVariable( nDIAItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_SurveillerItineraire::Terminate()
{
    NET_ASN_Tools::ResetPath( itineraire_, GetVariable( nDIAItineraireIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_SurveillerItineraire::Serialize
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_SurveillerItineraire::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ABC_SurveillerItineraire& asnMission = *new ASN1T_Mission_Automate_ABC_SurveillerItineraire();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_abc_surveiller_itineraire;
    asnMsg.mission.u.mission_automate_abc_surveiller_itineraire  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_SurveillerItineraire::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:2
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_SurveillerItineraire::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_abc_surveiller_itineraire );
    ASN1T_Mission_Automate_ABC_SurveillerItineraire& asnMission = *asnMsg.mission.u.mission_automate_abc_surveiller_itineraire;

    NET_ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
