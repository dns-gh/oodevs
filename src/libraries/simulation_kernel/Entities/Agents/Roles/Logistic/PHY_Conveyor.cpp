// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_Conveyor.cpp $
// $Author: Nld $
// $Modtime: 19/07/05 18:18 $
// $Revision: 1 $
// $Workfile: PHY_Conveyor.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_Conveyor.h"
#include "PHY_SupplyStockConsign.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include <boost/serialization/split_free.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_Conveyor )

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
    // NOTHING
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
    assert( pConveyorComp_->CanBePartOfConvoy() );

    pConveyorComp_->GetStockTransporterCapacity( rWeightCapacity_, rVolumeCapacity_ );
    assert( rWeightCapacity_ > 0. && rVolumeCapacity_ > 0. );
    pConveyorPion_->GetRole< PHY_RoleInterface_Supply >().StartUsingForLogistic( *pConveyorComp_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor destructor
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
PHY_Conveyor::~PHY_Conveyor()
{
    assert( !pLentTo_ );
    pConveyorPion_->GetRole< PHY_RoleInterface_Supply >().StopUsingForLogistic( *pConveyorComp_ );
}

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, PHY_Conveyor::T_DotationMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const PHY_Conveyor::T_DotationMap& map, const unsigned int )
        {
            std::size_t size = map.size();
            file << size;
            for ( PHY_Conveyor::CIT_DotationMap it = map.begin(); it != map.end(); ++it )
            {
                unsigned id = it->first->GetMosID();
                file << id;
                file << it->second;
            }
        }

        template< typename Archive >
        void load( Archive& file, PHY_Conveyor::T_DotationMap& map, const unsigned int )
        {
            std::size_t nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                unsigned int nID;

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
void PHY_Conveyor::serialize( Archive& file, const unsigned int )
{
    file & pConveyorComp_
         & pConveyorPion_
         & rWeightCapacity_
         & rVolumeCapacity_
         & dotationsConvoyed_
         & pLentTo_;
}

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::Convoy
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
double PHY_Conveyor::Convoy( PHY_SupplyConsign_ABC& consign, const PHY_DotationCategory& dotationCategory, const double rNbrToConvoy )
{
    assert( pConveyorComp_ );
    if( !pConveyorComp_->GetType().CanTransportStock( dotationCategory ) )
        return 0.;

    double rVolumeToConvoy = rNbrToConvoy * dotationCategory.GetVolume();
    double rWeightToConvoy = rNbrToConvoy * dotationCategory.GetWeight();

    if( rVolumeToConvoy > rVolumeCapacity_ )
    {
        rVolumeToConvoy = rVolumeCapacity_;
        rWeightToConvoy = ( rVolumeToConvoy / dotationCategory.GetVolume() ) * dotationCategory.GetWeight();
    }

    if( rWeightToConvoy > rWeightCapacity_ )
    {
        rWeightToConvoy = rWeightCapacity_;
        rVolumeToConvoy = ( rWeightToConvoy / dotationCategory.GetWeight() ) * dotationCategory.GetVolume();
    }

    assert( rVolumeToConvoy <= rVolumeCapacity_ );
    assert( rWeightToConvoy <= rWeightCapacity_ );

    rVolumeCapacity_ -= rVolumeToConvoy;
    rWeightCapacity_ -= rWeightToConvoy;

    assert( rWeightToConvoy / dotationCategory.GetWeight() == rVolumeToConvoy / dotationCategory.GetVolume() );

    const double rNbrConvoyed = std::min( rNbrToConvoy, std::max( rWeightToConvoy / dotationCategory.GetWeight(), rVolumeToConvoy / dotationCategory.GetVolume() ) );
    dotationsConvoyed_[ &dotationCategory ] += rNbrConvoyed;
    consign.AddConvoyedMerchandise( dotationCategory, rNbrConvoyed );
    return rNbrConvoyed;
}

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::LendTo
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
void PHY_Conveyor::LendTo( MIL_AgentPion& pion )
{
    assert( !pLentTo_ );
    pLentTo_ = &pion;
    pConveyorPion_->GetRole< PHY_RoleInterface_Supply      >().StopUsingForLogistic( *pConveyorComp_ );
    pConveyorPion_->GetRole< PHY_RoleInterface_Composantes >().LendComposante      ( pLentTo_->GetRole< PHY_RoleInterface_Composantes >(), *pConveyorComp_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::UndoLend
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
void PHY_Conveyor::UndoLend()
{
    assert( pLentTo_ );
    pConveyorPion_->GetRole< PHY_RoleInterface_Composantes >().RetrieveLentComposante( pLentTo_->GetRole< PHY_RoleInterface_Composantes >(), *pConveyorComp_ );
    pConveyorPion_->GetRole< PHY_RoleInterface_Supply      >().StartUsingForLogistic ( *pConveyorComp_ );
    pLentTo_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::NotifyConveyorDestroyed
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
void PHY_Conveyor::NotifyConveyorDestroyed( PHY_SupplyConsign_ABC& consign )
{
    for( CIT_DotationMap it = dotationsConvoyed_.begin(); it != dotationsConvoyed_.end(); ++it )
        consign.RemoveConvoyedMerchandise( *it->first, it->second );
    dotationsConvoyed_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::EmptyOut
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
void PHY_Conveyor::EmptyOut()
{
    dotationsConvoyed_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::ApproximateTravelTime
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
unsigned int PHY_Conveyor::ApproximateTravelTime( const MT_Vector2D& vStartPos, const MT_Vector2D& vEndPos )
{
    return pConveyorComp_->ApproximateTravelTime( vStartPos, vEndPos );
}

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

// -----------------------------------------------------------------------------
// Name: PHY_Conveyor::IsFull
// Created: NLD 2005-07-19
// -----------------------------------------------------------------------------
bool PHY_Conveyor::IsFull() const
{
    return rWeightCapacity_ <= 0. || rVolumeCapacity_ <= 0.;
}
