// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Tc2/MIL_AgentPionLOGTC2.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 5 $
// $Workfile: MIL_AgentPionLOGTC2.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentPionLOGTC2.h"
#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Medical.h"
#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Supply.h"
#include "simulation_kernel/AlgorithmsFactories.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AgentPionLOGTC2 )

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_AgentPionLOGTC2* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID = pion->GetType().GetID();
    const AlgorithmsFactories* const algorithmFactories = &pion->GetAlgorithms();
    archive << nTypeID
        << algorithmFactories;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AgentPionLOGTC2* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    AlgorithmsFactories* algorithmFactories = 0;
    archive >> nTypeID
        >> algorithmFactories;
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nTypeID );
    assert( pType );
    ::new( pion )MIL_AgentPionLOGTC2( *pType, *algorithmFactories );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2 constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGTC2::MIL_AgentPionLOGTC2( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis )
    : MIL_AgentPionLOG_ABC( type, automate, algorithmFactories, xis )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2 constructor
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPionLOGTC2::MIL_AgentPionLOGTC2( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, const std::string& name )
    : MIL_AgentPionLOG_ABC( type, automate, algorithmFactories, name )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2 constructor
// Created: JSR 2010-03-09
// -----------------------------------------------------------------------------
MIL_AgentPionLOGTC2::MIL_AgentPionLOGTC2( const MIL_AgentTypePion& type, const AlgorithmsFactories& algorithmFactories )
    : MIL_AgentPionLOG_ABC( type, algorithmFactories )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2 destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGTC2::~MIL_AgentPionLOGTC2()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGTC2::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    LoadRole< PHY_RolePionLOG_Maintenance >( file, *this );
    LoadRole< PHY_RolePionLOG_Medical >( file, *this );
    LoadRole< PHY_RolePionLOG_Supply >( file, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGTC2::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    SaveRole< PHY_RolePionLOG_Maintenance >( file, *this );
    SaveRole< PHY_RolePionLOG_Medical >( file, *this );
    SaveRole< PHY_RolePionLOG_Supply >( file, *this );
}
