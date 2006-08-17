// *****************************************************************************
//
// $Created: 
// $Archive: /MVW_v10/Build/SDK/AGR/src/AGR_MissionPion_Skeleton.cpp $
// $Author: Nld $
// $Modtime: 20/10/04 15:41 $
// $Revision: 6 $
// $Workfile: AGR_MissionPion_Skeleton.cpp $
//
// *****************************************************************************

#include "simulation_missions_pch.h"
#include "MIL_PionMission_INF_ControlerSecteur.h"

#include "simulation_kernel/Entities/Orders/Pion/MIL_PionOrderManager.h"
#include "simulation_kernel/Entities/Orders/Pion/MIL_PionMissionType.h"
#include "simulation_kernel/Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "simulation_kernel/Knowledge/MIL_KnowledgeGroup.h"
#include "simulation_kernel/Network/NET_ASN_Tools.h"
#include "simulation_kernel/Decision/DEC_Tools.h"

int MIL_PionMission_INF_ControlerSecteur::nDIASecteurIdx_ = 0 ;
int MIL_PionMission_INF_ControlerSecteur::nDIAPreparerTerrainIdx_ = 0 ;


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ControlerSecteur::InitializeDIA
// Created: 
//-----------------------------------------------------------------------------
// static
void MIL_PionMission_INF_ControlerSecteur::InitializeDIA( const MIL_PionMissionType& type )
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( type.GetDIATypeName() );
    nDIASecteurIdx_ = DEC_Tools::InitializeDIAField( "secteur_", diaType );
    nDIAPreparerTerrainIdx_ = DEC_Tools::InitializeDIAField( "preparerTerrain_", diaType );

}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ControlerSecteur constructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_ControlerSecteur::MIL_PionMission_INF_ControlerSecteur( MIL_AgentPion& pion, const MIL_PionMissionType& type )
: MIL_PionMission_ABC( pion, type )
{
    // NOTHING    
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ControlerSecteur destructor
// Created: 
//-----------------------------------------------------------------------------
MIL_PionMission_INF_ControlerSecteur::~MIL_PionMission_INF_ControlerSecteur()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ControlerSecteur::Initialize
// Created: 
//-----------------------------------------------------------------------------
ASN1T_EnumOrderErrorCode MIL_PionMission_INF_ControlerSecteur::Initialize( const ASN1T_MsgPionOrder& asnMsg )
{
    ASN1T_EnumOrderErrorCode nCode = MIL_PionMission_ABC::Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
        return nCode;        

    const ASN1T_Mission_Pion_INF_ControlerSecteur& asnMission = *asnMsg.mission.u.mission_pion_inf_controler_secteur;
    if( !NET_ASN_Tools::CopyPolygon( asnMission.secteur, GetVariable( nDIASecteurIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;
    if( !NET_ASN_Tools::CopyBool( asnMission.preparer_terrain, GetVariable( nDIAPreparerTerrainIdx_ ) ) )
        return EnumOrderErrorCode::error_invalid_mission_parameters;

    return EnumOrderErrorCode::no_error;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ControlerSecteur::Initialize
// Created: 
//-----------------------------------------------------------------------------
bool MIL_PionMission_INF_ControlerSecteur::Initialize( const MIL_AutomateMission_ABC& parentMission )
{
    if( ! MIL_PionMission_ABC::Initialize( parentMission ) )
        return false;

        
    return true;    
}

// ------------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ControlerSecteur::Initialize
// Created: 
// -----------------------------------------------------------------------------
bool MIL_PionMission_INF_ControlerSecteur::Initialize( MIL_PionMission_ABC& missionTmp )
{
    if( !MIL_PionMission_ABC::Initialize( missionTmp ) )
        return false;
    MIL_PionMission_INF_ControlerSecteur& mission = static_cast< MIL_PionMission_INF_ControlerSecteur& >( missionTmp );

    NET_ASN_Tools::CopyPolygon( mission.GetVariable( nDIASecteurIdx_ ), GetVariable( nDIASecteurIdx_ ) );
    NET_ASN_Tools::CopyBool( mission.GetVariable( nDIAPreparerTerrainIdx_ ), GetVariable( nDIAPreparerTerrainIdx_ ) );

    return true;
}                                                                    

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ControlerSecteur::Terminate
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_ControlerSecteur::Terminate()
{
    
    MIL_PionMission_ABC::Terminate();    
}

//=============================================================================
// NETWORK
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ControlerSecteur::Serialize
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_ControlerSecteur::Serialize( ASN1T_MsgPionOrder& asnMsg )
{
    MIL_PionMission_ABC::Serialize( asnMsg );
    
    ASN1T_Mission_Pion_INF_ControlerSecteur& asnMission = *new ASN1T_Mission_Pion_INF_ControlerSecteur();
    asnMsg.mission.t                           = T_Mission_Pion_mission_pion_inf_controler_secteur;
    asnMsg.mission.u.mission_pion_inf_controler_secteur  = &asnMission;

    NET_ASN_Tools::CopyPolygon( GetVariable( nDIASecteurIdx_ ), asnMission.secteur );
    NET_ASN_Tools::CopyBool( GetVariable( nDIAPreparerTerrainIdx_ ), asnMission.preparer_terrain );

}

//-----------------------------------------------------------------------------
// Name: MIL_PionMission_INF_ControlerSecteur::CleanAfterSerialization
// Created: 
//-----------------------------------------------------------------------------
void MIL_PionMission_INF_ControlerSecteur::CleanAfterSerialization( ASN1T_MsgPionOrder& asnMsg )
{
    assert( asnMsg.mission.t == T_Mission_Pion_mission_pion_inf_controler_secteur );
    ASN1T_Mission_Pion_INF_ControlerSecteur& asnMission = *asnMsg.mission.u.mission_pion_inf_controler_secteur;

    NET_ASN_Tools::Delete( asnMission.secteur );

    delete &asnMission;

    MIL_PionMission_ABC::CleanAfterSerialization( asnMsg );
}
