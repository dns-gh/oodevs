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
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_DataComputerPionData::PHY_RoleAction_Objects_DataComputerPionData()
    : operation_           ()
    , pObject_             ( 0 )
    , pPion_               ( 0 )
    , bConsumptionReserved_( false )
    , workingComposantes_  ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData
// Created: NLD 2005-03-25
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_DataComputerPionData::PHY_RoleAction_Objects_DataComputerPionData( MIL_AgentPion& pion, E_Operation operation, const MIL_RealObject_ABC& object )
    : operation_           ( operation )
    , pObject_             ( &object )
    , pPion_               ( &pion )
    , bConsumptionReserved_( false )
    , workingComposantes_  ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::operator()
// Created: NLD 2007-02-12
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputerPionData::operator() ( const PHY_ComposantePion& composante )
{
    MT_Float rDeltaTime = 0.;
    switch( operation_ )
    {
        case eConstruct: rDeltaTime = composante.GetConstructionTime( pObject_->GetType(), pObject_->GetSizeCoef() ); break;
        case eDestroy  : rDeltaTime = composante.GetDestructionTime ( pObject_->GetType(), pObject_->GetSizeCoef() ); break;
        case eMine     : rDeltaTime = composante.GetMiningTime      ( pObject_->GetType() ); break;
        case eDemine   : rDeltaTime = composante.GetDeminingTime    ( pObject_->GetType() ); break;
        case eBypass   : rDeltaTime = composante.GetBypassTime      ( pObject_->GetType(), pObject_->GetSizeCoef(), pObject_->IsMined() ); break;
        case ePrepare  : rDeltaTime = composante.GetConstructionTime( pObject_->GetType(), pObject_->GetSizeCoef() ); break;
        default: assert( false );
    }
    assert( rDeltaTime >= 0. );
    if( rDeltaTime == std::numeric_limits< MT_Float >::max() )
        return;
    workingComposantes_.push_back( std::make_pair( &composante, rDeltaTime ) );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::RemoveSlowComposantes
// Created: NLD 2007-02-12
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputerPionData::RemoveSlowComposantes( MT_Float rMinOperationTime )
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

    const PHY_ConsumptionType* pConsumptionMode = 0;
    for( CIT_ComposanteDataVector it = workingComposantes_.begin(); it != workingComposantes_.end(); ++it )
    {
        const PHY_ConsumptionType& composanteConsumptionMode = it->first->GetConsumptionMode( pObject_->GetType() );
        if( !pConsumptionMode || *pConsumptionMode < composanteConsumptionMode  )
            pConsumptionMode = &composanteConsumptionMode;
    }
    bConsumptionReserved_ = pPion_->GetRole< PHY_RolePion_Dotations >().SetConsumptionMode( *pConsumptionMode );
}


// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::RollbackConsumptionsReservations
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputerPionData::RollbackConsumptionsReservations()
{
    if( bConsumptionReserved_ )
    {
        bConsumptionReserved_ = false;
        pPion_->GetRole< PHY_RolePion_Dotations >().RollbackConsumptionMode();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::GetDotationValue
// Created: NLD 2007-02-12
// -----------------------------------------------------------------------------
uint PHY_RoleAction_Objects_DataComputerPionData::GetDotationValue( const PHY_DotationCategory& category ) const
{
    return (uint)pPion_->GetRole< PHY_RolePion_Dotations >().GetDotationValue( category );;    
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::ConsumeDotations
// Created: NLD 2007-02-13
// -----------------------------------------------------------------------------
uint PHY_RoleAction_Objects_DataComputerPionData::ConsumeDotations( const PHY_DotationCategory& category, uint nNbr )
{
    return (uint)pPion_->GetRole< PHY_RolePion_Dotations >().ConsumeDotation( category, nNbr );    
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::RecoverDotations
// Created: NLD 2007-02-13
// -----------------------------------------------------------------------------
uint PHY_RoleAction_Objects_DataComputerPionData::RecoverDotations( const PHY_DotationCategory& category, uint nNbr )
{
    return (uint)pPion_->GetRole< PHY_RolePion_Dotations >().SupplyDotation( category, nNbr );    
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputerPionData::GetMinOperationTime
// Created: NLD 2007-02-13
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Objects_DataComputerPionData::GetMinOperationTime() const
{
    MT_Float rMinOperationTime = std::numeric_limits< MT_Float >::max();
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
void PHY_RoleAction_Objects_DataComputerPionData::GetTotalOperationTime( MT_Float& rOperationTime, uint& nNbrComposantes ) const
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
