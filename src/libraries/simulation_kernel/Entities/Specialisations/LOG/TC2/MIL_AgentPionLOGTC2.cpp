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
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePionLOG_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/Medical/PHY_RolePionLOG_Medical.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePionLOG_Supply.h"
#include "simulation_kernel/AlgorithmsFactories.h"

BOOST_CLASS_EXPORT_GUID( MIL_AgentPionLOGTC2, "MIL_AgentPionLOGTC2" )

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_AgentPionLOGTC2* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID = pion->GetType().GetID();
    const MIL_Automate* const pAutomate = &pion->GetAutomate();
    const AlgorithmsFactories* const algorithmFactories = &pion->GetAlgorithms();
    archive << nTypeID
        << pAutomate
        << algorithmFactories;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AgentPionLOGTC2* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    MIL_Automate* pAutomate = 0;
    AlgorithmsFactories* algorithmFactories = 0;
    archive >> nTypeID
        >> pAutomate
        >> algorithmFactories;
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nTypeID );
    assert( pType );
    ::new( pion )MIL_AgentPionLOGTC2( *pType, *pAutomate, *algorithmFactories );
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
MIL_AgentPionLOGTC2::MIL_AgentPionLOGTC2( const MIL_AgentTypePion& type, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories )
    : MIL_AgentPionLOG_ABC( type, automate, algorithmFactories )
{
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
void MIL_AgentPionLOGTC2::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    
    { PHY_RolePionLOG_Maintenance* pRole; file >> pRole; RegisterRole( *pRole ); }
    { PHY_RolePionLOG_Medical    * pRole; file >> pRole; RegisterRole( *pRole ); }
    { PHY_RolePionLOG_Supply     * pRole; file >> pRole; RegisterRole( *pRole ); }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGTC2::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    const PHY_RolePionLOG_Maintenance* const maintenance = &GetRole< PHY_RolePionLOG_Maintenance >();
    const PHY_RolePionLOG_Medical* const     medical     = &GetRole< PHY_RolePionLOG_Medical >();
    const PHY_RolePionLOG_Supply* const      supply      = &GetRole< PHY_RolePionLOG_Supply >();
    file << maintenance
         << medical
         << supply;
}
