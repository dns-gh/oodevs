// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_AgentPionLOG_ABC.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 11:32 $
// $Revision: 6 $
// $Workfile: MIL_AgentPionLOG_ABC.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AgentPionLOG_ABC.h"
#include "MIL_AutomateLOG.h"
#include "MissionController_ABC.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Actions/PHY_ActionLogistic.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Maintenance.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Medical.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_AgentPionLOG_ABC )

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_AgentPionLOG_ABC* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID = pion->GetType().GetID();
    const MissionController_ABC* const controller = &pion->GetController();
    archive << nTypeID
            << controller;
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_AgentPionLOG_ABC* pion, const unsigned int /*version*/ )
{
    unsigned int nTypeID;
    MissionController_ABC* controller = 0;
    archive >> nTypeID
            >> controller;
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nTypeID );
    assert( pType );
    ::new( pion ) MIL_AgentPionLOG_ABC( *pType, *controller );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC constructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOG_ABC::MIL_AgentPionLOG_ABC( const MIL_AgentTypePion& type,
                                            MissionController_ABC& controller,
                                            MIL_Automate& automate,
                                            xml::xistream& xis )
    : MIL_AgentPion   ( type, controller, automate, xis )
    , pLogisticAction_( new PHY_ActionLogistic< MIL_AgentPionLOG_ABC >( *this ) )
{
    this->RegisterAction( pLogisticAction_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC constructor
// Created: JSR 2010-03-09
// -----------------------------------------------------------------------------
MIL_AgentPionLOG_ABC::MIL_AgentPionLOG_ABC( const MIL_AgentTypePion& type,
                                            MissionController_ABC& controller )
    : MIL_AgentPion   ( type, controller )
    , pLogisticAction_( new PHY_ActionLogistic< MIL_AgentPionLOG_ABC >( *this ) )
{
    this->RegisterAction( pLogisticAction_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC destructor
// Created: NLD 2004-10-04
// -----------------------------------------------------------------------------
MIL_AgentPionLOG_ABC::~MIL_AgentPionLOG_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC::UpdateLogistic
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
void MIL_AgentPionLOG_ABC::UpdateLogistic()
{
    if( IsMarkedForDestruction() )
        return;
    const bool bIsDead = IsDead();
    PHY_RoleInterface_Maintenance* roleMaintenance = RetrieveRole< PHY_RoleInterface_Maintenance >();
    if( roleMaintenance )
        roleMaintenance->UpdateLogistic( bIsDead );
    PHY_RoleInterface_Medical* roleMedical = RetrieveRole< PHY_RoleInterface_Medical >();
    if( roleMedical )
        roleMedical->UpdateLogistic( bIsDead );
    PHY_RoleInterface_Supply* roleSupply = RetrieveRole< PHY_RoleInterface_Supply >();
    if( roleSupply )
        roleSupply->UpdateLogistic( bIsDead );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC::FindLogisticManager
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
MIL_AutomateLOG* MIL_AgentPionLOG_ABC::FindLogisticManager() const
{
    return GetAutomate().FindLogisticManager();
}


// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOG_ABC::CancelAllActions
// Created: LDC 2012-10-24
// -----------------------------------------------------------------------------
void MIL_AgentPionLOG_ABC::CancelAllActions()
{
    MIL_AgentPion::CancelAllActions();
    this->RegisterAction( pLogisticAction_ );
}

template < typename Archive >
void MIL_AgentPionLOG_ABC::load( Archive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_AgentPion >( *this );
    // Only medical role does not auto register
    PHY_RoleInterface_Maintenance* maintenance;
    file >> maintenance;
    if( maintenance )
        RegisterRole( *maintenance );
    PHY_RoleInterface_Medical* medical;
    file >> medical;
    if( medical )
        RegisterRole( *medical );
    PHY_RoleInterface_Supply* supply;
    file >> supply;
    if( supply )
        RegisterRole( *supply );
}

template < typename Archive >
void MIL_AgentPionLOG_ABC::save( Archive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_AgentPion >( *this );
    const auto* const maintenance = RetrieveRole< PHY_RoleInterface_Maintenance >();
    file << maintenance;
    const auto* const medical = RetrieveRole< PHY_RoleInterface_Medical >();
    file << medical;
    const auto* const supply = RetrieveRole< PHY_RoleInterface_Supply >();
    file << supply;
}

template void MIL_AgentPionLOG_ABC::serialize( MIL_CheckPointInArchive&, const unsigned int );
template void MIL_AgentPionLOG_ABC::serialize( MIL_CheckPointOutArchive&, const unsigned int );
