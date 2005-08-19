// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_Conveyor.cpp $
// $Author: Nld $
// $Modtime: 19/07/05 18:18 $
// $Revision: 1 $
// $Workfile: PHY_Conveyor.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_Conveyor.h"

#include "PHY_SupplyStockConsign.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/MIL_AgentPion.h"

BOOST_CLASS_EXPORT_GUID( PHY_Conveyor, "PHY_Conveyor" )

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor constructor
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
PHY_Conveyor::PHY_Conveyor()
    : pConveyorComp_    ( 0 )
    , pConveyorPion_    ( 0 )
    , rWeightCapacity_  ( 0. )
    , rVolumeCapacity_  ( 0. )
    , dotationsConvoyed_( )
    , pLentTo_          ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor constructor
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
PHY_Conveyor::PHY_Conveyor( PHY_ComposantePion& conveyorComp, MIL_AgentPion& conveyorPion )
    : pConveyorComp_    ( &conveyorComp )
    , pConveyorPion_    ( &conveyorPion )
    , rWeightCapacity_  ( 0. )
    , rVolumeCapacity_  ( 0. )
    , dotationsConvoyed_( )
    , pLentTo_          ( 0 )
{
    pConveyorComp_->GetConvoyTransporterCapacity( rWeightCapacity_, rVolumeCapacity_ );
    assert( rWeightCapacity_ > 0. && rVolumeCapacity_ > 0. );
    pConveyorPion_->GetRole< PHY_RolePion_Supply >().StartUsingForLogistic( *pConveyorComp_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor destructor
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
PHY_Conveyor::~PHY_Conveyor()
{
    assert( !pLentTo_ );
    pConveyorPion_->GetRole< PHY_RolePion_Supply >().StopUsingForLogistic( *pConveyorComp_ );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_Conveyor::T_DotationMap& map, const uint nVersion )
        {
            split_free( file, map, nVersion );
        }
        
        template< typename Archive >
        void save( Archive& file, const PHY_Conveyor::T_DotationMap& map, const uint )
        {
            file << map.size();
            for ( PHY_Conveyor::CIT_DotationMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first->GetMosID();
                file << it->second;
            }   
        }
        
        template< typename Archive >
        void load( Archive& file, PHY_Conveyor::T_DotationMap& map, const uint )
        {
            uint nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                uint nID;
                
                file >> nID;
                file >> map[ PHY_DotationType::FindDotationCategory( nID ) ];
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::serialize
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_Conveyor::serialize( Archive& file, const uint )
{ 
    file & pConveyorComp_
         & pConveyorPion_
         & rWeightCapacity_
         & rVolumeCapacity_
         & dotationsConvoyed_
         & pLentTo_;
}

// =============================================================================
// DOTATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::ConvoyDotations
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
MT_Float PHY_Conveyor::ConvoyDotations( const PHY_DotationCategory& dotationCategory, const MT_Float rNbrToConvoy )
{
    MT_Float rVolumeToConvoy = std::min( rVolumeCapacity_, rNbrToConvoy * dotationCategory.GetVolume() );
    MT_Float rWeightToConvoy = std::min( rWeightCapacity_, rNbrToConvoy * dotationCategory.GetWeight() );

    MT_Float rNbrConvoyed = 0.;
    if( rVolumeToConvoy > rWeightToConvoy )
    {
        rNbrConvoyed      = rWeightToConvoy / dotationCategory.GetWeight();
        rWeightCapacity_ -= rWeightToConvoy;
        rVolumeCapacity_ -= ( rNbrConvoyed * dotationCategory.GetVolume() );
    }
    else
    {
        rNbrConvoyed      = rVolumeToConvoy / dotationCategory.GetVolume();                
        rVolumeCapacity_ -= rVolumeToConvoy;
        rWeightCapacity_ -= ( rNbrConvoyed * dotationCategory.GetWeight() );               
    }

    dotationsConvoyed_[ &dotationCategory ] += rNbrConvoyed;

    return rNbrConvoyed;
}

// =============================================================================
// LEND
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::LendTo
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
void PHY_Conveyor::LendTo( MIL_AgentPion& pion )
{
    assert( !pLentTo_ );
    pLentTo_ = &pion;
    pConveyorPion_->GetRole< PHY_RolePion_Supply      >().StopUsingForLogistic( *pConveyorComp_ );
    pConveyorPion_->GetRole< PHY_RolePion_Composantes >().LendComposante      ( pLentTo_->GetRole< PHY_RolePion_Composantes >(), *pConveyorComp_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::UndoLend
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
void PHY_Conveyor::UndoLend()
{
    assert( pLentTo_ );
    pConveyorPion_->GetRole< PHY_RolePion_Composantes >().UndoLendComposante( pLentTo_->GetRole< PHY_RolePion_Composantes >(), *pConveyorComp_ );
    pConveyorPion_->GetRole< PHY_RolePion_Supply      >().StartUsingForLogistic( *pConveyorComp_ );
    pLentTo_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::NotifyComposanteChanged
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
void PHY_Conveyor::NotifyComposanteChanged( PHY_SupplyStockConsign& stockConsign )
{
    for( CIT_DotationMap it = dotationsConvoyed_.begin(); it != dotationsConvoyed_.end(); ++it )
        stockConsign.RemoveConvoyedStock( *it->first, it->second );
    dotationsConvoyed_.clear();
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::GetLoadingTime
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
uint PHY_Conveyor::GetLoadingTime() const
{
    return pConveyorComp_->GetConvoyTransporterLoadingTime();
}

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::GetUnloadingTime
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
uint PHY_Conveyor::GetUnloadingTime() const
{
    return pConveyorComp_->GetConvoyTransporterUnloadingTime();
}

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::ApproximateTravelTime
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
uint PHY_Conveyor::ApproximateTravelTime( const MT_Vector2D& vStartPos, const MT_Vector2D& vEndPos )
{
    return pConveyorComp_->ApproximateTravelTime( vStartPos, vEndPos );
}

// =============================================================================
// DEBUG
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::Dump
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
void PHY_Conveyor::Dump() const
{
    printf( "Transporter type : %s (%p) (Pion %d)\n", pConveyorComp_->GetType().GetName().c_str(), &pConveyorComp_, pConveyorPion_->GetID() );
    for( CIT_DotationMap it = dotationsConvoyed_.begin(); it != dotationsConvoyed_.end(); ++it )
        printf( "\tDotation : %s : %.2f\n", it->first->GetName().c_str(), it->second );
}
