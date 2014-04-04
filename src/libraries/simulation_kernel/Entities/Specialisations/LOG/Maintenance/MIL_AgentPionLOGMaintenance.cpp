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
#include "MissionController_ABC.h"
#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Maintenance.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AgentPionLOGMaintenance )

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_AgentPionLOGMaintenance* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID = pion->GetType().GetID();
    const MissionController_ABC* const controller = &pion->GetController();
    archive << nTypeID
            << controller;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AgentPionLOGMaintenance* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    MissionController_ABC* controller = 0;
    archive >> nTypeID
            >> controller;
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nTypeID );
    assert( pType );
    ::new( pion ) MIL_AgentPionLOGMaintenance( *pType, *controller );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMaintenance constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMaintenance::MIL_AgentPionLOGMaintenance( const MIL_AgentTypePion& type,
                                                          MissionController_ABC& controller,
                                                          MIL_Automate& automate,
                                                          xml::xistream& xis )
    : MIL_AgentPionLOG_ABC( type, controller, automate, xis )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMaintenance constructor
// Created: JSR 2010-03-09
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMaintenance::MIL_AgentPionLOGMaintenance( const MIL_AgentTypePion& type,
                                                          MissionController_ABC& controller )
    : MIL_AgentPionLOG_ABC( type, controller )
{
    // NOTHING
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
void MIL_AgentPionLOGMaintenance::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    PHY_RolePionLOG_Maintenance* role;
    file >> role;
    RegisterRole( *role );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMaintenance::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGMaintenance::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    const PHY_RolePionLOG_Maintenance* const role = &GetRole< PHY_RolePionLOG_Maintenance >();
    file << role;
}
