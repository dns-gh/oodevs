// *****************************************************************************
//
// $Created: RFT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_AutomateInjuredHuman.cpp $
// $Author: Rft $
// $Modtime: 11/05/05 14:54 $
// $Revision: 18 $
// $Workfile: MIL_AutomateInjuredHuman.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AutomateInjuredHuman.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "MIL_AutomateTypeInjuredHuman.h"
#include "PHY_InjuredHuman.h"

#include "Entities/MIL_Formation.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Orders/MIL_Report.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AutomateInjuredHuman )


template< typename Archive >
void save_construct_data( Archive& archive, const MIL_AutomateInjuredHuman* automat, const unsigned int /*version*/ )
{
    unsigned int type = automat->RetrieveType()->GetID();
    archive << type;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AutomateInjuredHuman* automat, const unsigned int /*version*/ )
{
    unsigned int type;
    archive >> type;
    const MIL_AutomateTypeInjuredHuman* pType = dynamic_cast< const MIL_AutomateTypeInjuredHuman* > ( MIL_AutomateType::FindAutomateType( type ) );
    assert( pType );
    ::new( automat )MIL_AutomateInjuredHuman( *pType );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateInjuredHuman constructor
// Created: RFT 2004-12-21
// -----------------------------------------------------------------------------
MIL_AutomateInjuredHuman::MIL_AutomateInjuredHuman(     const MIL_AutomateTypeInjuredHuman& type
                                                        , unsigned int nID, MIL_Entity_ABC& parent
                                                        , xml::xistream& xis
                                                        , unsigned int gcPause
                                                        , unsigned int gcMult )
    : MIL_Automate( type, nID, parent, xis, gcPause, gcMult )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateInjuredHuman constructor
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
MIL_AutomateInjuredHuman::MIL_AutomateInjuredHuman( const MIL_AutomateType& type, unsigned int nID, MIL_Entity_ABC& parent, unsigned int knowledgeGroup, const std::string& name, unsigned int gcPause, unsigned int gcMult, unsigned int context )
    : MIL_Automate( type, nID, parent, knowledgeGroup, name, gcPause, gcMult, context )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateInjuredHuman constructor
// Created: RPD 2009-11-06
// -----------------------------------------------------------------------------
MIL_AutomateInjuredHuman::MIL_AutomateInjuredHuman( const MIL_AutomateTypeInjuredHuman& type, unsigned int nID )
    : MIL_Automate( type, nID )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateInjuredHuman destructor
// Created: RFT 2004-12-21
// -----------------------------------------------------------------------------
MIL_AutomateInjuredHuman::~MIL_AutomateInjuredHuman()
{
    // NOTHING
}

// =============================================================================
// CHEKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentInjuredHumanPion::load
// Created: RFT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AutomateInjuredHuman::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_Automate >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentInjuredHumanPion::save
// Created: RFT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AutomateInjuredHuman::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< const MIL_Automate >( *this );
}

// =============================================================================
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateInjuredHuman::UpdateState
// Created: RFT 2005-01-27
// -----------------------------------------------------------------------------
void MIL_AutomateInjuredHuman::UpdateState()
{
    MIL_Automate::UpdateState();
    for( IT_InjuredHumanList it = pInjuredHumans_.begin() ; it != pInjuredHumans_.end() ; ++it )
        (*it)->Update();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateInjuredHuman::Clean
// Created: RFT 2005-01-31
// -----------------------------------------------------------------------------
void MIL_AutomateInjuredHuman::Clean()
{
    MIL_Automate::Clean();
    while( ! pInjuredHumans_.empty() )
        pInjuredHumans_.pop_front();
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateInjuredHuman::GetAutomateInjuredHuman
// Created: RFT 2005-01-17
// -----------------------------------------------------------------------------
MIL_AutomateInjuredHuman* MIL_AutomateInjuredHuman::GetAutomateInjuredHuman( unsigned int nID ) const
{
    MIL_Automate* pAutomate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( nID );
    if( !pAutomate || !pAutomate->GetType().IsInjuredHuman() )
        return 0;
    return static_cast< MIL_AutomateInjuredHuman* >( pAutomate );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateInjuredHuman::RetrieveType
// Created: RPD 2009-11-10
// -----------------------------------------------------------------------------
const MIL_AutomateType* MIL_AutomateInjuredHuman::RetrieveType() const
{
    return &GetType();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateInjuredHuman::UpdateNetwork
// Created: MIL_AutomateInjuredHuman 2005-01-25
// -----------------------------------------------------------------------------
void MIL_AutomateInjuredHuman::UpdateNetwork() const
{
    MIL_Automate::UpdateNetwork();
    //Send state change
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateInjuredHuman::SendFullState
// Created: RFT 2005-02-01
// -----------------------------------------------------------------------------
void MIL_AutomateInjuredHuman::SendFullState() const
{
    MIL_Automate::SendFullState();
    //Send full state
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateInjuredHuman::HandleInjuredHuman
// Created: RFT 2005-02-01
// -----------------------------------------------------------------------------
void MIL_AutomateInjuredHuman::HandleInjuredHuman( PHY_InjuredHuman* injuredHuman )
{
    pInjuredHumans_.push_back( injuredHuman );
}