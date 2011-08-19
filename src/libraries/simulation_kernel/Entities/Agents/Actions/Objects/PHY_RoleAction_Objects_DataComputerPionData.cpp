// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Objects/PHY_RoleAction_Objects_DataComputerPionData.cpp $
// $Author: Nld $
// $Modtime: 12/05/05 14:51 $
// $Revision: 6 $
// $Workfile: PHY_RoleAction_Objects_DataComputerPionData.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoleAction_Objects_DataComputerPionData.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "AlgorithmsFactories.h"
#include "DotationComputer_ABC.h"
#include "DotationComputerFactory_ABC.h"
#include "ConsumeDotationNotificationHandler_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_DataComputerPionData::PHY_RoleAction_Objects_DataComputerPionData()
    : pObject_             ( 0 )
    , pPion_               ( 0 )
    , bConsumptionReserved_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData
// Created: NLD 2005-03-25
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_DataComputerPionData::PHY_RoleAction_Objects_DataComputerPionData( MIL_Agent_ABC& pion, E_Operation operation, const MIL_Object_ABC& object )
    : operation_           ( operation )
    , pObject_             ( &object )
    , pPion_               ( &pion )
    , bConsumptionReserved_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::operator()
// Created: NLD 2007-02-12
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputerPionData::operator() ( const PHY_ComposantePion& composante )
{
    double rDeltaTime = 0.;
    switch( operation_ )
    {
        case eConstruct: rDeltaTime = composante.GetConstructionTime( pObject_->GetType(), (*pObject_)().GetSizeCoef() ); break;
        case eDestroy  : rDeltaTime = composante.GetDestructionTime( pObject_->GetType(), (*pObject_)().GetSizeCoef() ); break;
        case eMine     : rDeltaTime = composante.GetMiningTime( pObject_->GetType() ); break;
        case eDemine   : rDeltaTime = composante.GetDeminingTime( pObject_->GetType() ); break;
        case eBypass   : rDeltaTime = composante.GetBypassTime( pObject_->GetType(), (*pObject_)().GetSizeCoef(), (*pObject_)().IsMined() ); break;
        case eExtinguish   : rDeltaTime = composante.GetExtinguishingTime( pObject_->GetType() ); break;
        default: assert( false );
    }
    //@TODO MGD rDeltaTime can be negative to accelerate work with EBG, need to replace algorithm
    if( rDeltaTime == std::numeric_limits< double >::max() )
        return;
    workingComposantes_.push_back( std::make_pair( &composante, rDeltaTime ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::RemoveSlowComposantes
// Created: NLD 2007-02-12
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputerPionData::RemoveSlowComposantes( double rMinOperationTime )
{
    for( IT_ComposanteDataVector it = workingComposantes_.begin(); it != workingComposantes_.end(); )
    {
        if( it->second > rMinOperationTime )
            it = workingComposantes_.erase( it );
        else
            ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::ReserveConsumptions
// Created: NLD 2007-02-12
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputerPionData::ReserveConsumptions()
{
    if( workingComposantes_.empty() )
        return;
    assert( pPion_ );
    const PHY_ConsumptionType* pConsumptionMode = 0;
    for( CIT_ComposanteDataVector it = workingComposantes_.begin(); it != workingComposantes_.end(); ++it )
    {
        const PHY_ConsumptionType& composanteConsumptionMode = it->first->GetConsumptionMode( pObject_->GetType() );
        if( !pConsumptionMode || *pConsumptionMode < composanteConsumptionMode  )
            pConsumptionMode = &composanteConsumptionMode;
    }
    bConsumptionReserved_ = pPion_->GetRole< dotation::PHY_RoleInterface_Dotations >().SetConsumptionMode( *pConsumptionMode );
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::RollbackConsumptionsReservations
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputerPionData::RollbackConsumptionsReservations()
{
    if( bConsumptionReserved_ )
    {
        assert( pPion_ );
        bConsumptionReserved_ = false;
        pPion_->GetRole< dotation::PHY_RoleInterface_Dotations >().RollbackConsumptionMode();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::GetDotationValue
// Created: NLD 2007-02-12
// -----------------------------------------------------------------------------
unsigned int PHY_RoleAction_Objects_DataComputerPionData::GetDotationValue( const PHY_DotationCategory& category ) const
{
    assert( pPion_ );
 //   return ( unsigned int ) pPion_->GetRole< PHY_RoleInterface_Dotations >().GetDotationValue( category );
    std::auto_ptr< dotation::DotationComputer_ABC > dotationComputer( pPion_->GetAlgorithms().dotationComputerFactory_->Create() );
    pPion_->Execute( *dotationComputer );
    return ( unsigned int ) dotationComputer->GetDotationValue( category );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::ConsumeDotations
// Created: NLD 2007-02-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputerPionData::ConsumeDotations( const PHY_DotationCategory& category, double& nbr )
{
    pPion_->Apply( &dotation::ConsumeDotationNotificationHandler_ABC::NotifyConsumeDotation, category, nbr );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::RecoverDotations
// Created: NLD 2007-02-13
// -----------------------------------------------------------------------------
unsigned int PHY_RoleAction_Objects_DataComputerPionData::RecoverDotations( const PHY_DotationCategory& category, unsigned int nNbr )
{
    return (unsigned int)pPion_->GetRole< dotation::PHY_RoleInterface_Dotations >().SupplyDotation( category, nNbr );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::GetMinOperationTime
// Created: NLD 2007-02-13
// -----------------------------------------------------------------------------
double PHY_RoleAction_Objects_DataComputerPionData::GetMinOperationTime() const
{
    double rMinOperationTime = std::numeric_limits< double >::max();
    if( !bConsumptionReserved_ )
        return rMinOperationTime;

    for( CIT_ComposanteDataVector it = workingComposantes_.begin(); it != workingComposantes_.end(); ++it )
        rMinOperationTime = std::min( rMinOperationTime, it->second );
    return rMinOperationTime;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::GetOperationTime
// Created: NLD 2007-02-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputerPionData::GetTotalOperationTime( double& rOperationTime, unsigned int& nNbrComposantes ) const
{
    rOperationTime  = 0.;
    nNbrComposantes = 0;
    if( !bConsumptionReserved_ )
        return;

    for( CIT_ComposanteDataVector it = workingComposantes_.begin(); it != workingComposantes_.end(); ++it )
    {
        rOperationTime += it->second;
        ++ nNbrComposantes;
    }
}
