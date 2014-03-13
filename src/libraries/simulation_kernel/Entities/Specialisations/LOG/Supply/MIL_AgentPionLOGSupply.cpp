// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Supply/MIL_AgentPionLOGSupply.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 4 $
// $Workfile: MIL_AgentPionLOGSupply.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentPionLOGSupply.h"
#include "MissionController_ABC.h"
#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Supply.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AgentPionLOGSupply )

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_AgentPionLOGSupply* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID = pion->GetType().GetID();
    const MissionController_ABC* const controller = &pion->GetController();
    archive << nTypeID
            << controller;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AgentPionLOGSupply* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    MissionController_ABC* controller;
    archive >> nTypeID
            >> controller;
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nTypeID );
    assert( pType );
    ::new( pion ) MIL_AgentPionLOGSupply( *pType, *controller );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGSupply constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGSupply::MIL_AgentPionLOGSupply( const MIL_AgentTypePion& type,
                                                MissionController_ABC& controller,
                                                MIL_Automate& automate,
                                                xml::xistream& xis )
    : MIL_AgentPionLOG_ABC( type, controller, automate, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGSupply constructor
// Created: LDC 2010-03-09
// -----------------------------------------------------------------------------
MIL_AgentPionLOGSupply::MIL_AgentPionLOGSupply( const MIL_AgentTypePion& type,
                                                MissionController_ABC& controller )
    : MIL_AgentPionLOG_ABC( type, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGSupply destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGSupply::~MIL_AgentPionLOGSupply()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGSupply::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGSupply::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );

    PHY_RolePionLOG_Supply* pRole;
    file >> pRole;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGSupply::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGSupply::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    const PHY_RolePionLOG_Supply* const role = &GetRole< PHY_RolePionLOG_Supply >();
    file << role;
}
