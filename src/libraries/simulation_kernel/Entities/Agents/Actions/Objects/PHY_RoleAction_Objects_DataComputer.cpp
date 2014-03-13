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
#include "Entities/Agents/Roles/Reinforcement/PHY_RoleInterface_Reinforcement.h"
#include "MIL_Time_ABC.h"
#include "simulation_kernel/DefaultComponentFunctorComputer.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer constructor
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_DataComputer::PHY_RoleAction_Objects_DataComputer( MIL_Agent_ABC& pion, E_Operation operation, const MIL_Object_ABC& object )
    : operation_( operation )
    , object_   ( object )
{
    CollectData( pion );
    ReserveConsumptions();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer destructor
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_DataComputer::~PHY_RoleAction_Objects_DataComputer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::CollectData
// Created: NLD 2007-02-12
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::CollectData( MIL_Agent_ABC& pion )
{
    pionsData_.push_back( PHY_RoleAction_Objects_DataComputerPionData( pion, operation_, object_ ) );
    DefaultComponentFunctorComputer componentComputer( *this );
    pion.Execute< OnComponentComputer_ABC >( componentComputer );
    const auto& reinforcements = pion.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    for( auto itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
        CollectData( **itReinforcement );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::operator()
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::operator() ( PHY_ComposantePion& composante )
{
    pionsData_.back()( composante );
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
// Name: PHY_RoleAction_Objects_DataComputer::GetDotationsNumber
// Created: LDC 2012-09-26
// -----------------------------------------------------------------------------
unsigned int PHY_RoleAction_Objects_DataComputer::GetDotationsNumber( const PHY_DotationCategory& category ) const
{
    unsigned int nNbrAvailable = 0;
    for( CRIT_PionDataVector it = pionsData_.rbegin(); it != pionsData_.rend(); ++it )
        nNbrAvailable += it->GetDotationValue( category );
    return nNbrAvailable;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::HasDotations
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects_DataComputer::HasDotations( const PHY_DotationCategory& category, unsigned int nNbr ) const
{
    unsigned int nNbrAvailable = 0;
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
void PHY_RoleAction_Objects_DataComputer::ConsumeDotations( const PHY_DotationCategory& category, double& nbr )
{
    for( RIT_PionDataVector it = pionsData_.rbegin(); it != pionsData_.rend() && nbr > 0.; ++it )
    {
        it->ConsumeDotations( category, nbr );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::RecoverDotations
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::RecoverDotations( const PHY_DotationCategory& category, unsigned int nNbr )
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
double PHY_RoleAction_Objects_DataComputer::ComputeDeltaPercentage()
{
    double rTimeTmp  = ComputeWorkTime();
    if( rTimeTmp == 0. )
        return 1.;
    if( rTimeTmp == std::numeric_limits< double >::max() )
        return std::numeric_limits< double >::max();
    return MIL_Time_ABC::GetTime().GetTickDuration() / rTimeTmp;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::ComputeWorkTime
// Created: GGE & PSN  2010-04-09
// -----------------------------------------------------------------------------
double PHY_RoleAction_Objects_DataComputer::ComputeWorkTime()
{
    double rTotalOperationTime  = 0.;
    unsigned int nTotalNbrComposantes = 0;
    for( auto it = pionsData_.begin(); it != pionsData_.end(); ++it )
    {
        double rOperationTime;
        unsigned int nNbrComposantes;
        it->GetTotalOperationSpeed( rOperationTime, nNbrComposantes );

        rTotalOperationTime  += rOperationTime;
        nTotalNbrComposantes += nNbrComposantes;
    }
    if( nTotalNbrComposantes == 0 )
    {
        RollbackConsumptionsReservations();
        return std::numeric_limits< double >::max();
    }
    return rTotalOperationTime != 0. ? 1.0 / rTotalOperationTime : 0;
}
