// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_AgentDataRecognition.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 17:55 $
// $Revision: 6 $
// $Workfile: DEC_Knowledge_AgentDataRecognition.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_AgentDataRecognition.h"

#include "DEC_Knowledge_AgentPerceptionDataRecognition.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/Units/Categories/PHY_NatureLevel.h"
#include "Entities/Agents/Units/Categories/PHY_NatureAtlas.h"
#include "Entities/Agents/Units/Categories/PHY_Protection.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposanteType_ABC.h"
#include "Entities/Agents/MIL_AgentTypePion.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_AgentDataRecognition, "DEC_Knowledge_AgentDataRecognition" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition constructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentDataRecognition::DEC_Knowledge_AgentDataRecognition()
    : nTimeLastUpdate_          ( 0 )
    , pArmy_                    ( 0 )
    , bIsPC_                    ( false )
    , rOperationalState_        ( std::numeric_limits< MT_Float >::max() )
    , rMajorOperationalState_   ( std::numeric_limits< MT_Float >::max() ) 
    , pAgentType_               ( 0 )
    , bOperationalStateChanged_ ( false )
    , bAgentTypeUpdated_        ( false )
{
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition destructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentDataRecognition::~DEC_Knowledge_AgentDataRecognition()
{

}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataRecognition::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> nTimeLastUpdate_
         >> rOperationalState_
         >> rMajorOperationalState_
         >> composantes_
         >> const_cast< MIL_Army*& >( pArmy_ )
         >> bIsPC_;
         
    uint nID;
    file >> nID;
    pAgentType_ = MIL_AgentTypePion::Find( nID );
    
    file >> bOperationalStateChanged_
         >> bAgentTypeUpdated_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataRecognition::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    unsigned agentType = ( pAgentType_ ? pAgentType_->GetID() : (uint)-1 );
    file << nTimeLastUpdate_
         << rOperationalState_
         << rMajorOperationalState_
         << composantes_
         << pArmy_
         << bIsPC_
         << agentType
         << bOperationalStateChanged_
         << bAgentTypeUpdated_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::Prepare
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataRecognition::Prepare()
{
    bOperationalStateChanged_ = false;
    bAgentTypeUpdated_        = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::DoUpdate
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
template< typename T >
void DEC_Knowledge_AgentDataRecognition::DoUpdate( const T& data )
{
    if( data.GetTimeLastUpdate() <= nTimeLastUpdate_ )
        return;

    MT_Float rNewOpState = data.GetOperationalState();
    if( rOperationalState_ != rNewOpState )
    {
        rOperationalState_        = rNewOpState;
        bOperationalStateChanged_ = true;
    }

    rMajorOperationalState_ = data.GetMajorOperationalState();

    composantes_ = data.GetComposantes();

    if( !pAgentType_ )
    {
        bAgentTypeUpdated_ = true;
        pArmy_      = data.GetArmy     ();
        bIsPC_      = data.IsPC        ();
        pAgentType_ = data.GetAgentType();
        assert( pAgentType_ );
        assert( pArmy_      );
    }

    nTimeLastUpdate_ = data.GetTimeLastUpdate();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::Update
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataRecognition::Update( const DEC_Knowledge_AgentPerceptionDataRecognition& data )
{
   DoUpdate( data );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::Update
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataRecognition::Update( const DEC_Knowledge_AgentDataRecognition& data )
{
   DoUpdate( data );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::SendChangedState
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataRecognition::SendChangedState( ASN1T_MsgUnitKnowledgeUpdate& asnMsg ) const
{
    if( bOperationalStateChanged_ )
    {
        asnMsg.m.etat_opPresent = 1;
        asnMsg.etat_op = std::max( 0, std::min( 100, (int)( rOperationalState_ * 100. ) ) );
    }

    if( bAgentTypeUpdated_ )
    {
        assert( pArmy_ );
        assert( pAgentType_ );

        asnMsg.m.campPresent      = 1;
        asnMsg.m.nature_pcPresent = 1;

        asnMsg.camp      = pArmy_->GetID();
        asnMsg.nature_pc = bIsPC_;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::SendFullState
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataRecognition::SendFullState( ASN1T_MsgUnitKnowledgeUpdate& asnMsg ) const
{
    if( nTimeLastUpdate_ == 0 )
        return;

    asnMsg.m.etat_opPresent = 1;
    asnMsg.etat_op = std::max( 0, std::min( 100, (int)( rOperationalState_ * 100. ) ) );

    assert( pArmy_ );
    assert( pAgentType_ );

    asnMsg.m.campPresent      = 1;
    asnMsg.m.nature_pcPresent = 1;
    
    asnMsg.camp      = pArmy_->GetID();
    asnMsg.nature_pc = bIsPC_;
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::GetNatureAtlas
// Created: NLD 2004-11-10
// Modified: JVT 2004-12-09
// -----------------------------------------------------------------------------
const PHY_NatureAtlas& DEC_Knowledge_AgentDataRecognition::GetNatureAtlas() const
{
    return pAgentType_ ? pAgentType_->GetNatureAtlas() : PHY_NatureAtlas::notDefined_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::IsHuman
// Created: NLD 2007-04-19
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentDataRecognition::IsHuman() const
{
    for( CIT_KnowledgeComposanteVector it = composantes_.begin(); it != composantes_.end(); ++it )
        if( it->GetType().GetProtection().IsHuman() )
            return true;
    return false;
}
