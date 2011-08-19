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
#include "MIL_AgentServer.h"
#include "simulation_kernel/OnComponentFunctor_ABC.h"
#include "simulation_kernel/OnComponentFunctorComputer_ABC.h"
#include "simulation_kernel/OnComponentFunctorComputerFactory_ABC.h"
#include "simulation_kernel/AlgorithmsFactories.h"

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
    FilterData();
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
    std::auto_ptr< OnComponentComputer_ABC > componentComputer( pion.GetAlgorithms().onComponentFunctorComputerFactory_->Create( *this ) );
    pion.Execute( *componentComputer );
    const PHY_RoleInterface_Reinforcement::T_PionSet& reinforcements = pion.GetRole< PHY_RoleInterface_Reinforcement >().GetReinforcements();
    for( PHY_RoleInterface_Reinforcement::CIT_PionSet itReinforcement = reinforcements.begin(); itReinforcement != reinforcements.end(); ++itReinforcement )
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
// Name: PHY_RoleAction_Objects_DataComputer::FilterData
// Created: NLD 2007-02-12
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::FilterData()
{
    double rMinOperationTime = std::numeric_limits< double >::max();
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
    double rTotalOperationTime  = 0.;
    unsigned int nTotalNbrComposantes = 0;

    for( CIT_PionDataVector it = pionsData_.begin(); it != pionsData_.end(); ++it )
    {
        double rOperationTime;
        unsigned int nNbrComposantes;
        it->GetTotalOperationTime( rOperationTime, nNbrComposantes );
        rTotalOperationTime  += rOperationTime;
        nTotalNbrComposantes += nNbrComposantes;
    }

    if( nTotalNbrComposantes == 0 )
    {
        RollbackConsumptionsReservations();
        return std::numeric_limits< double >::max();
    }

    double rTimeTmp  = rTotalOperationTime / nTotalNbrComposantes; // <= Moyenne du temps de construction pour chaque composante
             rTimeTmp /= nTotalNbrComposantes;                       // <= Temps de construction de l'objet pour le pion

    if( rTimeTmp == 0. )
        return 1.;
    return MIL_AgentServer::GetWorkspace().GetTimeStepDuration() / rTimeTmp;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::ComputeWorkTime
// Created: GGE & PSN  2010-04-09
// -----------------------------------------------------------------------------
double PHY_RoleAction_Objects_DataComputer::ComputeWorkTime()
{
    double rTotalOperationTime  = 0.;
    unsigned int nTotalNbrComposantes = 0;
    for( CIT_PionDataVector it = pionsData_.begin(); it != pionsData_.end(); ++it )
    {
        double rOperationTime;
        unsigned int nNbrComposantes;
        it->GetTotalOperationTime( rOperationTime, nNbrComposantes );

        rTotalOperationTime  += rOperationTime;
        nTotalNbrComposantes += nNbrComposantes;
    }
    if( nTotalNbrComposantes == 0 )
    {
        RollbackConsumptionsReservations();
        return std::numeric_limits< double >::max();
    }

    double rTimeTmp  = rTotalOperationTime / nTotalNbrComposantes; // <= Moyenne du temps de construction pour chaque composante
             rTimeTmp /= nTotalNbrComposantes;                       // <= Temps de construction de l'objet pour le pion
    return rTimeTmp;
}