// *****************************************************************************
//
// $Created: RFT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Medical/MIL_AgentInjuredHumanPion.cpp $
// $Author: Rft $
// $Modtime: 6/04/05 11:48 $
// $Revision: 4 $
// $Workfile: MIL_AgentInjuredHumanPion.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentInjuredHumanPion.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include "xeumeuleu/xml.h"



BOOST_CLASS_EXPORT_GUID( MIL_AgentInjuredHumanPion, "MIL_AgentInjuredHumanPion" )


template< typename Archive > 
void save_construct_data( Archive& archive, const MIL_AgentInjuredHumanPion* unit, const unsigned int /*version*/ )
{
	unsigned int nTypeID = unit->GetType().GetID();
    unsigned int nID = unit->GetID() ;
	const MIL_Automate* const pAutomate = &unit->GetAutomate();
    const AlgorithmsFactories* const algorithmFactories = &unit->GetAlgorithms();
    archive << nTypeID 
        << nID 
        << pAutomate
        << algorithmFactories;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AgentInjuredHumanPion* unit, const unsigned int /*version*/ )
{
	unsigned int nTypeID, nID;
	MIL_Automate* pAutomate = 0;
    AlgorithmsFactories* algorithmFactories = 0;
    archive >> nTypeID
        >> nID
        >> pAutomate
        >> algorithmFactories;
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nTypeID );
    assert( pType );
    ::new( unit )MIL_AgentInjuredHumanPion( *pType, *pAutomate, *algorithmFactories );
}


// -----------------------------------------------------------------------------
// Name: MIL_AgentInjuredHumanPion constructor
// Created: RFT 2008-10-04
// -----------------------------------------------------------------------------
MIL_AgentInjuredHumanPion::MIL_AgentInjuredHumanPion( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis )
    : MIL_AgentPion( type, nID, automate, algorithmFactories, xis )
{
    if( !GetAutomate().GetType().IsInjuredHuman() )
        xis.error( "Error: not a logistic automaton for this logistic unit." );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentInjuredHumanPion constructor
// Created: RFT 2008-02-08
// -----------------------------------------------------------------------------
MIL_AgentInjuredHumanPion::MIL_AgentInjuredHumanPion( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories )
    : MIL_AgentPion( type, nID, automate, algorithmFactories )
{
    assert( automate.GetType().IsInjuredHuman() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentInjuredHumanPion constructor
// Created: RPD 2009-11-10
// -----------------------------------------------------------------------------
MIL_AgentInjuredHumanPion::MIL_AgentInjuredHumanPion( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories )
    : MIL_AgentPion( type, automate, algorithmFactories )
{
    assert( automate.GetType().IsInjuredHuman() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentInjuredHumanPion destructor
// Created: RFT 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentInjuredHumanPion::~MIL_AgentInjuredHumanPion()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentInjuredHumanPion::load
// Created: RPD 2009-11-10
// -----------------------------------------------------------------------------
void MIL_AgentInjuredHumanPion::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_AgentPion >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentInjuredHumanPion::save
// Created: RPD 2009-11-10
// -----------------------------------------------------------------------------
void MIL_AgentInjuredHumanPion::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_AgentPion >( *this );
}
