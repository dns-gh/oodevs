// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Objects/PHY_RoleAction_Objects_DataComputer.cpp $
// $Author: Nld $
// $Modtime: 12/05/05 14:51 $
// $Revision: 6 $
// $Workfile: PHY_RoleAction_Objects_DataComputer.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_RoleAction_Objects_DataComputer.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RoleInterface_Reinforcement.h"
#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer constructor
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_DataComputer::PHY_RoleAction_Objects_DataComputer( MIL_AgentPion& pion, PHY_RoleAction_Objects_DataComputerPionData::E_Operation operation, const MIL_Object_ABC& object )
    : pion_     ( pion )
    , operation_( operation )
    , object_   ( object )
{
    CollectData        ( pion );
    ReserveConsumptions();
    FilterData         ();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer destructor
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_DataComputer::~PHY_RoleAction_Objects_DataComputer()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::CollectData
// Created: NLD 2007-02-12
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::CollectData( MIL_AgentPion& pion )
{
    pionsData_.push_back( PHY_RoleAction_Objects_DataComputerPionData( pion, operation_, object_ ) );
    pion.GetRole< PHY_RolePion_Composantes >().Apply( *this );
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = pion.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    for( PHY_RoleInterface_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
        CollectData( **itReinforcement );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::operator()
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::operator() ( const PHY_ComposantePion& composante )
{    
    pionsData_.back()( composante );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::FilterData
// Created: NLD 2007-02-12
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::FilterData()
{
    MT_Float rMinOperationTime = std::numeric_limits< MT_Float >::max();
    for( CIT_PionDataVector it = pionsData_.begin(); it != pionsData_.end(); ++it )
        rMinOperationTime = std::min( rMinOperationTime, it->GetMinOperationTime() );

    for( IT_PionDataVector it = pionsData_.begin(); it != pionsData_.end(); ++it )
        it->RemoveSlowComposantes( rMinOperationTime );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::ReserveConsumptions
// Created: NLD 2007-02-12
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::ReserveConsumptions()
{
    for( IT_PionDataVector it = pionsData_.begin(); it != pionsData_.end(); ++it )
        it->ReserveConsumptions();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::RollbackConsumptionsReservations
// Created: NLD 2007-02-12
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::RollbackConsumptionsReservations()
{
    for( IT_PionDataVector it = pionsData_.begin(); it != pionsData_.end(); ++it )
        it->RollbackConsumptionsReservations();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::HasDotations
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects_DataComputer::HasDotations( const PHY_DotationCategory& category, uint nNbr ) const
{
    uint nNbrAvailable = 0;
    for( CRIT_PionDataVector it = pionsData_.rbegin(); it != pionsData_.rend(); ++it )
    {
        nNbrAvailable += it->GetDotationValue( category );
        if( nNbrAvailable >= nNbr )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::ConsumeDotations
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::ConsumeDotations( const PHY_DotationCategory& category, uint nNbr )
{
    for( RIT_PionDataVector it = pionsData_.rbegin(); it != pionsData_.rend(); ++it )
    {
        nNbr -= it->ConsumeDotations( category, nNbr );
        if( nNbr == 0 )
            return;
    }
    assert( !__FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::RecoverDotations
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::RecoverDotations( const PHY_DotationCategory& category, uint nNbr )
{
    for( RIT_PionDataVector it = pionsData_.rbegin(); it != pionsData_.rend(); ++it )
    {
        nNbr -= it->RecoverDotations( category, nNbr );
        if( nNbr == 0 )
            return;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::ComputeDeltaPercentage
// Created: NLD 2007-02-13
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Objects_DataComputer::ComputeDeltaPercentage()
{
    MT_Float rTotalOperationTime  = 0.;
    uint     nTotalNbrComposantes = 0;

    for( CIT_PionDataVector it = pionsData_.begin(); it != pionsData_.end(); ++it )
    {
        MT_Float rOperationTime;
        uint     nNbrComposantes;
        it->GetTotalOperationTime( rOperationTime, nNbrComposantes );

        rTotalOperationTime  += rOperationTime;
        nTotalNbrComposantes += nNbrComposantes;
    }

    if( nTotalNbrComposantes == 0 )
    {
        RollbackConsumptionsReservations();
        return std::numeric_limits< MT_Float >::max();
    }
    
    MT_Float rTimeTmp  = rTotalOperationTime / nTotalNbrComposantes; // <= Moyenne du temps de construction pour chaque composante
             rTimeTmp /= nTotalNbrComposantes;                       // <= Temps de construction de l'objet pour le pion
    
    if( rTimeTmp == 0. )
        return 1.;   
    return MIL_AgentServer::GetWorkspace().GetTimeStepDuration() / rTimeTmp;
}