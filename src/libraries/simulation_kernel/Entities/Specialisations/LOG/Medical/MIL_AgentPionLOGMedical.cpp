// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/Medical/MIL_AgentPionLOGMedical.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 4 $
// $Workfile: MIL_AgentPionLOGMedical.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentPionLOGMedical.h"
#include "MissionController_ABC.h"
#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Medical.h"
#include "simulation_kernel/AlgorithmsFactories.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AgentPionLOGMedical )

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_AgentPionLOGMedical* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID = pion->GetType().GetID();
    const AlgorithmsFactories* const algorithmFactories = &pion->GetAlgorithms();
    const MissionController_ABC* const controller = &pion->GetController();
    archive << nTypeID
            << algorithmFactories
            << controller;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AgentPionLOGMedical* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    AlgorithmsFactories* algorithmFactories = 0;
    MissionController_ABC* controller = 0;
    archive >> nTypeID
            >> algorithmFactories
            >> controller;
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nTypeID );
    assert( pType );
    ::new( pion ) MIL_AgentPionLOGMedical( *pType, *algorithmFactories, *controller );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMedical constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMedical::MIL_AgentPionLOGMedical( const MIL_AgentTypePion& type,
                                                  const AlgorithmsFactories& algorithmFactories,
                                                  MissionController_ABC& controller,
                                                  MIL_Automate& automate,
                                                  xml::xistream& xis )
    : MIL_AgentPionLOG_ABC( type, algorithmFactories, controller, automate, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMedical constructor
// Created: JSR 2010-03-09
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMedical::MIL_AgentPionLOGMedical( const MIL_AgentTypePion& type,
                                                  const AlgorithmsFactories& algorithmFactories,
                                                  MissionController_ABC& controller )
    : MIL_AgentPionLOG_ABC( type, algorithmFactories, controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMedical destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGMedical::~MIL_AgentPionLOGMedical()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMedical::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGMedical::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );

    PHY_RolePionLOG_Medical* pRole;
    file >> pRole;
    RegisterRole( *pRole );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGMedical::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGMedical::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    const PHY_RolePionLOG_Medical* const role = &GetRole< PHY_RolePionLOG_Medical >();
    file << role;
}
