// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentPerceptionDataRecognition.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 17:58 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_AgentPerceptionDataRecognition.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_AgentPerceptionDataRecognition.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "MIL_AgentServer.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_AgentPerceptionDataRecognition, "DEC_Knowledge_AgentPerceptionDataRecognition" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition constructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerceptionDataRecognition::DEC_Knowledge_AgentPerceptionDataRecognition()
    : nTimeLastUpdate_       ( 0 )
    , rOperationalState_     ( 1. )
    , rMajorOperationalState_( 1. )
    , pArmy_                 ( 0 )
    , bIsPC_                 ( false )
    , pAgentType_            ( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition destructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentPerceptionDataRecognition::~DEC_Knowledge_AgentPerceptionDataRecognition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerceptionDataRecognition::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> nTimeLastUpdate_
         >> rOperationalState_
         >> rMajorOperationalState_
         >> const_cast< MIL_Army*& >( pArmy_ )
         >> bIsPC_
         >> composantes_;
    
    uint nID;
    file >> nID;
    pAgentType_ = MIL_AgentTypePion::Find( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerceptionDataRecognition::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    unsigned agentType = ( pAgentType_ ? pAgentType_->GetID() : (uint)-1 );
    file << nTimeLastUpdate_
         << rOperationalState_
         << rMajorOperationalState_
         << pArmy_
         << bIsPC_
         << composantes_
         << agentType;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::Update
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentPerceptionDataRecognition::Update( const MIL_Agent_ABC& agentPerceived, const PHY_PerceptionLevel& perceptionLevel )
{
    if( perceptionLevel < PHY_PerceptionLevel::recognized_ )
        return;

    const uint nCurrentTimeStep = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( nTimeLastUpdate_ >= nCurrentTimeStep )
        return;
    nTimeLastUpdate_ = nCurrentTimeStep;

    composantes_.clear();
    agentPerceived.GetRole< PHY_RoleInterface_Composantes >().BuildKnowledgeComposantes( composantes_ );

    const PHY_RoleInterface_Composantes& roleComposantes = agentPerceived.GetRole< PHY_RoleInterface_Composantes >();
    rOperationalState_      =  roleComposantes.GetOperationalState     ();
    rMajorOperationalState_ =  roleComposantes.GetMajorOperationalState();
    pArmy_                  = dynamic_cast< const MIL_Army* >( &agentPerceived.GetArmy() );
    bIsPC_                  =  agentPerceived.IsPC();
    pAgentType_             = &agentPerceived.GetType();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::GetArmy
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
const MIL_Army_ABC* DEC_Knowledge_AgentPerceptionDataRecognition::GetArmy() const
{
    return pArmy_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::GetTimeLastUpdate
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
uint DEC_Knowledge_AgentPerceptionDataRecognition::GetTimeLastUpdate() const
{
    return nTimeLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::IsPC
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentPerceptionDataRecognition::IsPC() const
{
    return bIsPC_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::GetAgentType
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
const MIL_AgentType_ABC* DEC_Knowledge_AgentPerceptionDataRecognition::GetAgentType() const
{
    return pAgentType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::GetOperationalState
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_AgentPerceptionDataRecognition::GetOperationalState() const
{
    return rOperationalState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::GetMajorOperationalState
// Created: NLD 2005-11-30
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_AgentPerceptionDataRecognition::GetMajorOperationalState() const
{
    return rMajorOperationalState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentPerceptionDataRecognition::GetComposantes
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
const T_KnowledgeComposanteVector& DEC_Knowledge_AgentPerceptionDataRecognition::GetComposantes() const
{
    return composantes_;
}
