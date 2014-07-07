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
#include "protocol/ClientSenders.h"
#include "DEC_Knowledge_AgentComposante.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_AgentDataRecognition )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition constructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentDataRecognition::DEC_Knowledge_AgentDataRecognition()
    : nTimeLastUpdate_         ( 0 )
    , pArmy_                   ( 0 )
    , rOperationalState_       ( std::numeric_limits< double >::max() )
    , rMajorOperationalState_  ( std::numeric_limits< double >::max() )
    , maxSquareRange_          ( 0 )
    , pAgentType_              ( 0 )
    , bIsPC_                   ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition destructor
// Created: NLD 2004-11-09
// -----------------------------------------------------------------------------
DEC_Knowledge_AgentDataRecognition::~DEC_Knowledge_AgentDataRecognition()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataRecognition::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> nTimeLastUpdate_;
    file >> rOperationalState_;
    file >> rMajorOperationalState_;
    file >> maxSquareRange_;
    file >> composantes_;
    file >> const_cast< MIL_Army_ABC*& >( pArmy_ );
    file >> bIsPC_;
    unsigned int nID;
    file >> nID;
    pAgentType_ = MIL_AgentTypePion::Find( nID );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataRecognition::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned agentType = ( pAgentType_ ? pAgentType_->GetID() : (unsigned int)-1 );
    file << nTimeLastUpdate_;
    file << rOperationalState_;
    file << rMajorOperationalState_;
    file << maxSquareRange_;
    file << composantes_;
    file << pArmy_;
    file << bIsPC_;
    file << agentType;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::WriteKnowledges
// Created: NPT 2012-08-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataRecognition::WriteKnowledges( xml::xostream& xos ) const
{
    xos << xml::start( "data-recognition" )
            << xml::attribute( "time-last-update", nTimeLastUpdate_ );
    if( rOperationalState_ != std::numeric_limits< double >::max() )
        xos << xml::attribute( "op-state", rOperationalState_ );
    if( rMajorOperationalState_ != std::numeric_limits< double >::max() )
        xos << xml::attribute( "major-op-state", rMajorOperationalState_ );
    unsigned int number = 1;
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
    {
        auto itNext = it + 1;
        if( itNext == composantes_.end() || ( it->GetType().GetMosID().id() != itNext->GetType().GetMosID().id() ) )
        {
            it->WriteKnowledges( xos, number );
            number = 1;
        }
        else
            ++number;
    }

    if( pAgentType_ )
        xos << xml::attribute( "agent-type", pAgentType_->GetID() );
    xos << xml::end;
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
    double rNewOpState = data.GetOperationalState();
    if( rOperationalState_ != rNewOpState )
        rOperationalState_ = rNewOpState;
    rMajorOperationalState_ = data.GetMajorOperationalState();
    composantes_ = data.GetComposantes();
    double maxRange = 0;
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        maxRange = std::max( maxRange, it->GetMaxRange() );
    maxSquareRange_ = maxRange * maxRange;
    if( !pAgentType_ )
    {
        pArmy_= data.GetArmy();
        bIsPC_ = data.IsPC();
        pAgentType_ = data.GetAgentType();
        assert( pAgentType_ );
        assert( pArmy_ );
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
    for( auto it = composantes_.begin(); it != composantes_.end(); ++it )
        if( it->GetType().GetProtection().IsHuman() )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::GetOperationalState
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
double DEC_Knowledge_AgentDataRecognition::GetOperationalState() const
{
    return rOperationalState_ == std::numeric_limits< double >::max() ? 1. : rOperationalState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::GetMajorOperationalState
// Created: NLD 2005-11-30
// -----------------------------------------------------------------------------
double DEC_Knowledge_AgentDataRecognition::GetMajorOperationalState() const
{
    return rMajorOperationalState_ == std::numeric_limits< double >::max() ? 1. : rMajorOperationalState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::GetMaxSquareRange
// Created: MCO 2012-11-23
// -----------------------------------------------------------------------------
double DEC_Knowledge_AgentDataRecognition::GetMaxSquareRange() const
{
    return maxSquareRange_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::GetComposantes
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
const T_KnowledgeComposanteVector& DEC_Knowledge_AgentDataRecognition::GetComposantes() const
{
    return composantes_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::GetArmy
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
const MIL_Army_ABC* DEC_Knowledge_AgentDataRecognition::GetArmy() const
{
    return pArmy_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::GetTimeLastUpdate
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
unsigned int DEC_Knowledge_AgentDataRecognition::GetTimeLastUpdate() const
{
    return nTimeLastUpdate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::IsPC
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
bool DEC_Knowledge_AgentDataRecognition::IsPC() const
{
    return bIsPC_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::GetAgentType
// Created: NLD 2004-11-15
// -----------------------------------------------------------------------------
const MIL_AgentType_ABC* DEC_Knowledge_AgentDataRecognition::GetAgentType() const
{
    return pAgentType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_AgentDataRecognition::Extrapolate
// Created: JVT 2004-11-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_AgentDataRecognition::Extrapolate( const MIL_Agent_ABC& /*agentKnown*/ )
{
    // NOTHING
}
