// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Maintenance/MIL_AgentPionLOGMaintenance.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 4 $
// $Workfile: MIL_AgentPionLOGMaintenance.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentPionLOGMaintenance.h"
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePionLOG_Maintenance.h"
#include "simulation_kernel/AlgorithmsFactories.h"

BOOST_CLASS_EXPORT_GUID( MIL_AgentPionLOGMaintenance, "MIL_AgentPionLOGMaintenance" )

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_AgentPionLOGMaintenance* pion, const unsigned int /*version*/ )
{
	unsigned int nTypeID = pion->GetType().GetID();
    unsigned int nID = pion->GetID() ;
	const MIL_Automate* const pAutomate = &pion->GetAutomate();
    const AlgorithmsFactories* const algorithmFactories = &pion->GetAlgorithms();
	archive << nTypeID 
            << nID 
            << pAutomate
            << algorithmFactories;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AgentPionLOGMaintenance* pion, const unsigned int /*version*/ )
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
    ::new( pion )MIL_AgentPionLOGMaintenance( *pType, nID, *pAutomate, *algorithmFactories );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMaintenance constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMaintenance::MIL_AgentPionLOGMaintenance( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories, xml::xistream& xis )
    : MIL_AgentPionLOG_ABC( type, nID, automate, algorithmFactories, xis )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMaintenance constructor
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMaintenance::MIL_AgentPionLOGMaintenance( const MIL_AgentTypePion& type, uint nID, MIL_Automate& automate, const AlgorithmsFactories& algorithmFactories )
    : MIL_AgentPionLOG_ABC( type, nID, automate, algorithmFactories )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMaintenance destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMaintenance::~MIL_AgentPionLOGMaintenance()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMaintenance::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGMaintenance::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    
    PHY_RolePionLOG_Maintenance* pRole;
    file >> pRole;
    RegisterRole( *pRole );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMaintenance::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGMaintenance::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    const PHY_RolePionLOG_Maintenance* const role = &GetRole< PHY_RolePionLOG_Maintenance >();
    file << role;
}
