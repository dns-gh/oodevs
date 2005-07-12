// *****************************************************************************
//
// $Created: 2005-6-28 - 14:3:3 $
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionAutomate_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:36 $
// $Revision: 7 $
// $Workfile: AGR_MissionAutomate_Skeleton.cpp $
//
// *****************************************************************************

#include "Missions_pch.h"

#include "MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur.h"
#include "MIL/Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "MIL/Entities/Automates/MIL_Automate.h"
#include "MIL/Knowledge/MIL_KnowledgeGroup.h"
#include "MIL/Network/NET_ASN_Tools.h"
#include "MIL/Decision/DEC_Tools.h"

int MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur::nDIAFicelleDeVoleIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur constructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur::MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur( MIL_Automate& automate, const MIL_AutomateMissionType& type )
    : MIL_AutomateMission_ABC( automate, type )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur destructor
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur::~MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur()
{
    // NOTHING
}

//=============================================================================
// INIT
//=============================================================================
   
//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur::InitializeDIA
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
// static
void MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur::InitializeDIA( const MIL_AutomateMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIAFicelleDeVoleIdx_ = DEC_Tools::InitializeDIAField( "ficelleDeVole_", diaType );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur::Initialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur::Initialize( const ASN1T_MsgAutomateOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_AutomateMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur& asnMission = *asnMsg.mission.u.mission_automate_rens_roim_sdti_renseigner_sur;
    if( !NET_ASN_Tools::CopyPath( asnMission.ficelle_de_vole, ficelleDeVole_, GetVariable( nDIAFicelleDeVoleIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_ABC::Terminate
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur::Terminate()
{
    NET_ASN_Tools::ResetPath( ficelleDeVole_, GetVariable( nDIAFicelleDeVoleIdx_ ) );

    MIL_AutomateMission_ABC::Terminate();
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur::Serialize
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur::Serialize( ASN1T_MsgAutomateOrder& asnMsg )
{
    MIL_AutomateMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur& asnMission = *new ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur();
    asnMsg.mission.t                           = T_Mission_Automate_mission_automate_rens_roim_sdti_renseigner_sur;
    asnMsg.mission.u.mission_automate_rens_roim_sdti_renseigner_sur  = &asnMission;

    NET_ASN_Tools::CopyPath( GetVariable( nDIAFicelleDeVoleIdx_ ), asnMission.ficelle_de_vole );

}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur::CleanAfterSerialization
// Created: 2005-6-28 - 14:3:3
//-----------------------------------------------------------------------------
void MIL_AutomateMission_RENS_ROIM_SDTI_RenseignerSur::CleanAfterSerialization( ASN1T_MsgAutomateOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Automate_mission_automate_rens_roim_sdti_renseigner_sur );
    ASN1T_Mission_Automate_RENS_ROIM_SDTI_RenseignerSur& asnMission = *asnMsg.mission.u.mission_automate_rens_roim_sdti_renseigner_sur;

    NET_ASN_Tools::Delete( asnMission.ficelle_de_vole );

    delete &asnMission;

    MIL_AutomateMission_ABC::CleanAfterSerialization( asnMsg );
}
