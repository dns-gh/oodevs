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
#include "ConsumeDotationNotificationHandler_ABC.h"
#include "DefaultDotationComputer.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "simulation_terrain/TER_AnalyzerManager.h"
#include <spatialcontainer/TerrainData.h>

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_DataComputerPionData::PHY_RoleAction_Objects_DataComputerPionData()
    : operation_( eConstruct )
    , pObject_( 0 )
    , pPion_( 0 )
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

    double constructionSpeed = 1.;
    if( composante.HasConstructionSpeeds() )
    {
        const TER_Localisation& localisation = pObject_->GetLocalisation();
        TerrainData data = TER_AnalyzerManager::GetAnalyzerManager().GetTerrainData( localisation );
        constructionSpeed = composante.GetConstructionSpeed( data );
        if( constructionSpeed == 0 )
            return;
    }

    workingComposantes_.push_back( std::make_pair( &composante, rDeltaTime / constructionSpeed ) );
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
    for( auto it = workingComposantes_.begin(); it != workingComposantes_.end(); ++it )
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
    dotation::DefaultDotationComputer dotationComputer;
    pPion_->Execute< dotation::DotationComputer_ABC >( dotationComputer );
    return ( unsigned int ) dotationComputer.GetDotationValue( category );
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
// Name: PHY_RoleAction_Objects_DataComputerPionData::GetOperationTime
// Created: NLD 2007-02-13
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputerPionData::GetTotalOperationSpeed( double& rOperationTime, unsigned int& nNbrComposantes ) const
{
    rOperationTime  = 0.;
    nNbrComposantes = 0;
    if( !bConsumptionReserved_ )
        return;

    for( auto it = workingComposantes_.begin(); it != workingComposantes_.end(); ++it )
    {
        if( it->second != 0 )
            rOperationTime += ( 1.0 / it->second );
        ++ nNbrComposantes;
    }
}
