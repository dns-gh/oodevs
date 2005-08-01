// *****************************************************************************
//
// $Created: 2005-08-01 - 11:23:53 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_FaireMouvement.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_FaireMouvement::nDIAItineraireIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FaireMouvement constructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_FaireMouvement::MIL_AutomateMission_FaireMouvement( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FaireMouvement destructor
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
MIL_AutomateMission_FaireMouvement::~MIL_AutomateMission_FaireMouvement()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FaireMouvement::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_FaireMouvement::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAItineraireIdx_ = DEC_Tools::InitializeDIAField( "itineraire_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FaireMouvement::Initialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_FaireMouvement::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_FaireMouvement& asnMission = *asnMsg.mission.u.mission_automate_faire_mouvement;
    if( !NET_ASN_Tools::CopyPath( asnMission.itineraire, itineraire_, GetVariable( nDIAItineraireIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_FaireMouvement::Terminate()
{
    NET_ASN_Tools::ResetPath( itineraire_, GetVariable( nDIAItineraireIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FaireMouvement::Serialize
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_FaireMouvement::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_FaireMouvement& asnMission = *new ASN1T_Mission_Automate_FaireMouvement();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_faire_mouvement;
    asnMsg.mission.u.mission_automate_faire_mouvement  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAItineraireIdx_ ), asnMission.itineraire );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_FaireMouvement::CleanAfterSerialization
// Created: 2005-08-01 - 11:23:53
//-----------------------------------------------------------------------------
void MIL_AutomateMission_FaireMouvement::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_faire_mouvement );
    ASN1T_Mission_Automate_FaireMouvement& asnMission = *asnMsg.mission.u.mission_automate_faire_mouvement;

    NET_ASN_Tools::Delete( asnMission.itineraire );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
