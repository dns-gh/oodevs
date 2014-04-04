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
#include "MissionController_ABC.h"
#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOGConvoy_Supply.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_StockSupplyManager.h"
#include "Entities/Automates/MIL_DotationSupplyManager.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AgentPionLOGConvoy )

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_AgentPionLOGConvoy* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID = pion->GetType().GetID();
    const MissionController_ABC* const controller = &pion->GetController();
    archive << nTypeID
            << controller;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AgentPionLOGConvoy* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    MissionController_ABC* controller = 0;
    archive >> nTypeID
            >> controller;
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nTypeID );
    assert( pType );
    ::new( pion ) MIL_AgentPionLOGConvoy( *pType, *controller );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy constructor
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPionLOGConvoy::MIL_AgentPionLOGConvoy( const MIL_AgentTypePion& type,
                                                MissionController_ABC& controller,
                                                MIL_Automate& automate,
                                                xml::xistream& xis )
    : MIL_AgentPionLOG_ABC( type, controller, automate, xis )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy constructor
// Created: JSR 2010-03-09
// -----------------------------------------------------------------------------
MIL_AgentPionLOGConvoy::MIL_AgentPionLOGConvoy( const MIL_AgentTypePion& type,
                                                MissionController_ABC& controller )
    : MIL_AgentPionLOG_ABC( type, controller )
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
    PHY_RolePionLOGConvoy_Supply* role;
    file >> role;
    RegisterRole( *role );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGConvoy::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
    const PHY_RolePionLOGConvoy_Supply* const role = &GetRole< PHY_RolePionLOGConvoy_Supply >();
    file << role;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy::WriteODB
// Created: NLD 2006-06-05
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGConvoy::WriteODB( xml::xostream& /*xos*/ ) const
{
    // NOTHING : don't serialize Convoys
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy::SpecializedDelete
// Created: JSR 2013-02-06
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGConvoy::SpecializedDelete()
{
    MIL_AutomateLOG* logBrain = GetAutomate().GetBrainLogistic();
    if( logBrain )
        logBrain->ResetConsignsForConvoyPion( *this );
    GetAutomate().GetStockSupplyManager().ResetConsignsForConvoyPion( *this );
    GetAutomate().GetDotationSupplyManager().ResetConsignsForConvoyPion( *this );
}
