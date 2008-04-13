// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_AgentPion.cpp $
// $Author: Nld $
// $Modtime: 2/05/05 18:40 $
// $Revision: 42 $
// $Workfile: MIL_AgentPion.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentPion.h"

#include "Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Roles/Humans/PHY_RolePion_Humans.h"
#include "Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Roles/Network/NET_RolePion_Dotations.h"
#include "Roles/Decision/DEC_RolePion_Decision.h"
#include "Roles/Perception/PHY_RolePion_Perceiver.h"    
#include "Roles/Posture/PHY_RolePion_Posture.h"
#include "Roles/Location/PHY_RolePion_Location.h"
#include "Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Roles/NBC/PHY_RolePion_NBC.h"
#include "Roles/Communications/PHY_RolePion_Communications.h"
#include "Roles/HumanFactors/PHY_RolePion_HumanFactors.h"
#include "Roles/Transported/PHY_RolePion_Transported.h"
#include "Roles/Surrender/PHY_RolePion_Surrender.h"
#include "Roles/Refugee/PHY_RolePion_Refugee.h"
#include "Roles/Population/PHY_RolePion_Population.h"
#include "Roles/Logistic/Maintenance/PHY_RolePion_Maintenance.h"
#include "Roles/Logistic/Medical/PHY_RolePion_Medical.h"
#include "Roles/Logistic/Supply/PHY_RolePion_Supply.h"

#include "Actions/Loading/PHY_RoleAction_Loading.h"
#include "Actions/Objects/PHY_RoleAction_Objects.h"
#include "Actions/Moving/PHY_RoleAction_Moving.h"
#include "Actions/Flying/PHY_RoleAction_Flying.h"
#include "Actions/Firing/DirectFiring/PHY_RoleAction_DirectFiring.h"
#include "Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h"
#include "Actions/Transport/PHY_RoleAction_Transport.h"
#include "Actions/Emergency/PHY_RoleAction_FolkInfluence.h"

#include "Entities/Orders/MIL_Report.h"
#include "Entities/Agents/Units/PHY_UnitType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Army.h"

#include "Decision/DEC_ModelPion.h"

#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KS_Fire.h"

#include "Network/NET_AgentServer.h"
#include "Network/NET_Publisher_ABC.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_AsnException.h"

#include "Hla/HLA_UpdateFunctor.h"

#include "Tools/MIL_Tools.h"

#include "xeumeuleu/xml.h"

using namespace xml;

namespace
{
    template< typename R >
    void SaveRole( const MIL_AgentPion& pion, MIL_CheckPointOutArchive& file )
    {
        const R* const role = & pion.GetRole< R >();
        file << role;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPion::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPion::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pType_ );
    
    file << boost::serialization::base_object< MIL_Agent_ABC >( *this );
    file << boost::serialization::base_object< PHY_Actor     >( *this );
    unsigned type = pType_->GetID();
    file << type
         << bIsPC_
         << strName_
         << pAutomate_
      // << actions_ // actions non sauvegardées
         << pKnowledgeBlackBoard_;
         
    SaveRole< NET_RolePion_Dotations         >( *this, file );
    SaveRole< PHY_RolePion_Reinforcement     >( *this, file );
    SaveRole< PHY_RolePion_Posture           >( *this, file );
    SaveRole< PHY_RolePion_Location          >( *this, file );
    SaveRole< PHY_RolePion_Dotations         >( *this, file );
    SaveRole< PHY_RolePion_Humans            >( *this, file );
    SaveRole< PHY_RolePion_Composantes       >( *this, file );
    SaveRole< PHY_RolePion_Perceiver         >( *this, file );
    SaveRole< PHY_RolePion_NBC               >( *this, file );
    SaveRole< PHY_RolePion_Communications    >( *this, file );
    SaveRole< PHY_RolePion_HumanFactors      >( *this, file );
    SaveRole< PHY_RolePion_Transported       >( *this, file );
    SaveRole< PHY_RolePion_Maintenance       >( *this, file );
    SaveRole< PHY_RolePion_Medical           >( *this, file );
    SaveRole< PHY_RolePion_Supply            >( *this, file );
    SaveRole< PHY_RolePion_Surrender         >( *this, file );
    SaveRole< PHY_RolePion_Refugee           >( *this, file );
    SaveRole< PHY_RolePion_Population        >( *this, file );
    SaveRole< PHY_RoleAction_Loading         >( *this, file );
    SaveRole< PHY_RoleAction_Transport       >( *this, file );
    SaveRole< PHY_RoleAction_Moving          >( *this, file );
    SaveRole< PHY_RoleAction_Objects         >( *this, file );
    SaveRole< PHY_RoleAction_DirectFiring    >( *this, file );
    SaveRole< PHY_RoleAction_IndirectFiring  >( *this, file );
    SaveRole< DEC_RolePion_Decision          >( *this, file );
    SaveRole< PHY_RoleAction_InterfaceFlying >( *this, file );
    SaveRole< PHY_RoleAction_FolkInfluence   >( *this, file );    
}
