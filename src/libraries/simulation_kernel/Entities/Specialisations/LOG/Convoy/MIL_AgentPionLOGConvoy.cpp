// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Convoy/MIL_AgentPionLOGConvoy.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 4 $
// $Workfile: MIL_AgentPionLOGConvoy.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentPionLOGConvoy.h"
#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOGConvoy_Supply.h"
#include "simulation_kernel/AlgorithmsFactories.h"

#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AgentPionLOGConvoy )

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_AgentPionLOGConvoy* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID = pion->GetType().GetID();
    const AlgorithmsFactories* const algorithmFactories = &pion->GetAlgorithms();
    archive << nTypeID
            << algorithmFactories;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AgentPionLOGConvoy* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    AlgorithmsFactories* algorithmFactories = 0;
    archive >> nTypeID
            >> algorithmFactories;
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nTypeID );
    assert( pType );
    ::new( pion )MIL_AgentPionLOGConvoy( *pType, *algorithmFactories );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy constructor
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPionLOGConvoy::MIL_AgentPionLOGConvoy( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, const std::string& name )
    : MIL_AgentPionLOG_ABC( type, automate, algorithmFactories, name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy constructor
// Created: JSR 2010-03-09
// -----------------------------------------------------------------------------
MIL_AgentPionLOGConvoy::MIL_AgentPionLOGConvoy( const MIL_AgentTypePion& type, const AlgorithmsFactories& algorithmFactories )
    : MIL_AgentPionLOG_ABC( type, algorithmFactories )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGConvoy::~MIL_AgentPionLOGConvoy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGConvoy::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    LoadRole< PHY_RolePionLOGConvoy_Supply >( file, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGConvoy::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    SaveRole< PHY_RolePionLOGConvoy_Supply >( file, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy::WriteODB
// Created: NLD 2006-06-05
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGConvoy::WriteODB( xml::xostream& /*xos*/ ) const
{
    // NOTHING : don't serialize Convoys
}
