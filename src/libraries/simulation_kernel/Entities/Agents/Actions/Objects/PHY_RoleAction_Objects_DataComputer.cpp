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

#include "MIL_pch.h"

#include "PHY_RoleAction_Objects_DataComputer.h"

#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Objects/MIL_RealObject_ABC.h"
#include "MIL_AgentServer.h"

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::sPionData::sPionData
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_DataComputer::sPionData::sPionData()
    : pPion_               ( 0 )
    , rTotalTime_          ( 0. )
    , nNbrComposantes_     ( 0 )
    , pConsumptionMode_    ( 0 )
    , bConsumptionReserved_( false )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::sPionData
// Created: NLD 2005-03-25
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_DataComputer::sPionData::sPionData( MIL_AgentPion& pion )
    : pPion_               ( &pion )
    , rTotalTime_          ( 0. )
    , nNbrComposantes_     ( 0 )
    , pConsumptionMode_    ( 0 )
    , bConsumptionReserved_( false )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer constructor
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_DataComputer::PHY_RoleAction_Objects_DataComputer( E_Operation nOperation, const MIL_RealObject_ABC& object )
    : nOperation_      ( nOperation )
    , object_          ( object )
    , pCurrentPionData_( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer destructor
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
PHY_RoleAction_Objects_DataComputer::~PHY_RoleAction_Objects_DataComputer()
{

}


// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::SetActivePion
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::SetActivePion( MIL_AgentPion& pion )
{
    for( IT_PionDataVector it = pionsData_.begin(); it != pionsData_.end(); ++it )
    {
        if( *it->pPion_ == pion )
        {
            pCurrentPionData_ = &(*it);
            return;
        }
    }
    pionsData_.push_back( sPionData( pion ) );
    pCurrentPionData_ = &pionsData_.back();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::operator()
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::operator() ( const PHY_ComposantePion& composante ) const
{
    const MIL_AgentPion& pion = composante.GetRole().GetPion();
    assert( pCurrentPionData_ && *pCurrentPionData_->pPion_ == pion );

    MT_Float rDeltaTime = 0.;
    switch( nOperation_ )
    {
        case eConstruct: rDeltaTime = composante.GetConstructionTime( object_.GetType(), object_.GetSizeCoef() ); break;
        case eDestroy  : rDeltaTime = composante.GetDestructionTime ( object_.GetType(), object_.GetSizeCoef() ); break;
        case eMine     : rDeltaTime = composante.GetMiningTime      ( object_.GetType() ); break;
        case eDemine   : rDeltaTime = composante.GetDeminingTime    ( object_.GetType() ); break;
        case eBypass   : rDeltaTime = composante.GetBypassTime      ( object_.GetType(), object_.GetSizeCoef(), object_.IsMined() ); break;
        case ePrepare  : rDeltaTime = composante.GetConstructionTime( object_.GetType(), object_.GetSizeCoef() ); break;
        default: assert( false );
    }
    assert( rDeltaTime >= 0. );
    if( rDeltaTime == std::numeric_limits< MT_Float >::max() )
        return;
    
    pCurrentPionData_->rTotalTime_ += rDeltaTime;
    ++ pCurrentPionData_->nNbrComposantes_;
    
    const PHY_ConsumptionType& consumptionMode = composante.GetConsumptionMode( object_.GetType() );
    if( !pCurrentPionData_->pConsumptionMode_ || *pCurrentPionData_->pConsumptionMode_ < consumptionMode  )
        pCurrentPionData_->pConsumptionMode_ = &consumptionMode;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::ReserveConsumptions
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::ReserveConsumptions()
{
    for( RIT_PionDataVector itData = pionsData_.rbegin(); itData != pionsData_.rend(); ++itData )
    {
        sPionData& data = *itData;

        if( data.nNbrComposantes_ )
        {
            assert( data.pConsumptionMode_ );
            data.bConsumptionReserved_ = data.pPion_->GetRole< PHY_RolePion_Dotations >().SetConsumptionMode( *data.pConsumptionMode_ );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::RollbackConsumptionsReservations
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::RollbackConsumptionsReservations()
{
    for( RIT_PionDataVector itData = pionsData_.rbegin(); itData != pionsData_.rend(); ++itData )
    {
        sPionData& data =  *itData;

        if( data.bConsumptionReserved_ )
        {
            data.bConsumptionReserved_ = false;
            data.pPion_->GetRole< PHY_RolePion_Dotations >().RollbackConsumptionMode();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::HasDotations
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
bool PHY_RoleAction_Objects_DataComputer::HasDotations( uint nNbr, const PHY_DotationCategory& category ) const
{
    uint nNbrAvailable = 0;
    for( CRIT_PionDataVector itData = pionsData_.rbegin(); itData != pionsData_.rend(); ++itData )
    {
        const sPionData& data = *itData;

        nNbrAvailable += (uint)data.pPion_->GetRole< PHY_RolePion_Dotations >().GetDotationValue( category );
        if( nNbrAvailable >= nNbr && data.pPion_->GetRole< PHY_RolePion_Dotations >().GetDotationCapacity( category ) > 0 )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::ConsumeDotations
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::ConsumeDotations( uint nNbr, const PHY_DotationCategory& category )
{
    for( RIT_PionDataVector itData = pionsData_.rbegin(); itData != pionsData_.rend(); ++itData )
    {
        sPionData& data = *itData;
    
        nNbr -= (uint)data.pPion_->GetRole< PHY_RolePion_Dotations >().ConsumeDotation( category, nNbr );
        if( nNbr == 0 )
            return;

    }
    assert( !__FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::RecoverDotations
// Created: NLD 2005-05-11
// -----------------------------------------------------------------------------
void PHY_RoleAction_Objects_DataComputer::RecoverDotations( uint nNbr, const PHY_DotationCategory& category )
{
    for( RIT_PionDataVector itData = pionsData_.rbegin(); itData != pionsData_.rend(); ++itData )
    {
        sPionData& data = *itData;
    
        nNbr -= data.pPion_->GetRole< PHY_RolePion_Dotations >().SupplyDotation( category, nNbr );
        if( nNbr == 0 )
            return;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_RoleAction_Objects_DataComputer::GetDeltaPercentage
// Created: NLD 2004-10-01
// -----------------------------------------------------------------------------
MT_Float PHY_RoleAction_Objects_DataComputer::GetDeltaPercentage() const
{
    MT_Float rTotalTime      = 0.;
    uint     nNbrComposantes = 0;

    for( CIT_PionDataVector itData = pionsData_.begin(); itData != pionsData_.end(); ++itData )
    {
        const sPionData& data = *itData;

        if( data.bConsumptionReserved_ )
        {
            rTotalTime      += data.rTotalTime_;
            nNbrComposantes += data.nNbrComposantes_;
        }
    }

    if( nNbrComposantes == 0 )
        return std::numeric_limits< MT_Float >::max();
    
    MT_Float rTimeTmp  = rTotalTime / nNbrComposantes; // <= Moyenne du temps de construction pour chaque composante
             rTimeTmp /= nNbrComposantes;              // <= Temps de construction de l'objet pour le pion
    
    if( rTimeTmp == 0. )
        return 1.;   
    return MIL_AgentServer::GetWorkspace().GetTimeStepDuration() / rTimeTmp;
}

