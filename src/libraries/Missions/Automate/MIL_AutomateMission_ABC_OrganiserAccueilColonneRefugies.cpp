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

#include "Missions_pch.h"

#include "MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies::nDIAZoneInstallationIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies::MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies::~MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAZoneInstallationIdx_ = DEC_Tools::InitializeDIAField( "zoneInstallation_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_ABC_OrganiserAccueilColonneRefugies& asnMission = *asnMsg.mission.u.mission_automate_abc_organiser_accueil_colonne_refugies;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.zone_installation, zoneInstallation_, GetVariable( nDIAZoneInstallationIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies::Terminate()
{
    NET_ASN_Tools::ResetPolygon( zoneInstallation_, GetVariable( nDIAZoneInstallationIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_ABC_OrganiserAccueilColonneRefugies& asnMission = *new ASN1T_Mission_Automate_ABC_OrganiserAccueilColonneRefugies();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_abc_organiser_accueil_colonne_refugies;
    asnMsg.mission.u.mission_automate_abc_organiser_accueil_colonne_refugies  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIAZoneInstallationIdx_ ), asnMission.zone_installation );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_AutomateMission_ABC_OrganiserAccueilColonneRefugies::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_abc_organiser_accueil_colonne_refugies );
    ASN1T_Mission_Automate_ABC_OrganiserAccueilColonneRefugies& asnMission = *asnMsg.mission.u.mission_automate_abc_organiser_accueil_colonne_refugies;

    NET_ASN_Tools::Delete( asnMission.zone_installation );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
