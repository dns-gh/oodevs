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
#include "MissionController_ABC.h"
#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Medical.h"
#include "Entities/Agents/Roles/Logistic/PHY_RolePionLOG_Supply.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_StockSupplyManager.h"
#include "Entities/Automates/MIL_DotationSupplyManager.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AgentPionLOGTC2 )

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_AgentPionLOGTC2* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID = pion->GetType().GetID();
    const MissionController_ABC* const controller = &pion->GetController();
    archive << nTypeID
            << controller;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AgentPionLOGTC2* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    MissionController_ABC* controller = 0;
    archive >> nTypeID
            >> controller;
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nTypeID );
    assert( pType );
    ::new( pion )MIL_AgentPionLOGTC2( *pType, *controller );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2 constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOGTC2::MIL_AgentPionLOGTC2( const MIL_AgentTypePion& type,
                                          MissionController_ABC& controller,
                                          MIL_Automate& automate,
                                          xml::xistream& xis )
    : MIL_AgentPionLOG_ABC( type, controller, automate, xis )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2 constructor
// Created: JSR 2010-03-09
// -----------------------------------------------------------------------------
MIL_AgentPionLOGTC2::MIL_AgentPionLOGTC2( const MIL_AgentTypePion& type,
                                          MissionController_ABC& controller )
    : MIL_AgentPionLOG_ABC( type, controller )
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
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGTC2::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_AgentPionLOG_ABC >( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGTC2::SpecializedDelete
// Created: JSR 2013-02-11
// -----------------------------------------------------------------------------
void MIL_AgentPionLOGTC2::SpecializedDelete()
{
    MIL_AutomateLOG* logBrain = GetAutomate().GetBrainLogistic();
    if( logBrain )
        logBrain->ResetConsignsForProvider( *this );// inutile ??
    //GetAutomate().GetStockSupplyManager().ResetAutoConsignForProvider( *this );
    GetAutomate().GetStockSupplyManager().ResetAllConsigns();
    GetAutomate().GetDotationSupplyManager().ResetAllConsigns();
    GetRole< PHY_RolePionLOG_Maintenance >().ClearMaintenanceConsigns();
    GetRole< PHY_RolePionLOG_Medical >().ClearMedicalConsigns();
}
