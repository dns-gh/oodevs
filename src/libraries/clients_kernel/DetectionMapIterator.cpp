// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "DetectionMapIterator.h"
#include "DetectionMap.h"

using namespace kernel;

static const float rIteratorEpsilon = 1e-10f;

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator constructor
// Created: JSR 2014-06-16
// -----------------------------------------------------------------------------
DetectionMapIterator::DetectionMapIterator( const DetectionMap& map, const geometry::Point3f& vBeginPos, const geometry::Point3f& vEndPos )
    : map_( map )
    , cellSize_( map_.GetCellSize() )
    , rDz_( vEndPos.Z() - vBeginPos.Z() )
    , eIteratorState_( eRunning )
    , nNextCellCol_( 0 )
    , nNextCellRow_( 0 )
    , vOutPoint_( vBeginPos )
    , originalEndAltitude_( vEndPos.Z() )
{
    float rDx = vEndPos.X() - vOutPoint_.X();
    float rDy = vEndPos.Y() - vOutPoint_.Y();

    if( rDx == 0 && rDy == 0 )  // -> vBeginPos == vEndPos
    {
        eIteratorState_ = eEnd;
        return;
    }

    bSwap_ = fabs( rDy ) > fabs( rDx );
    bNegX_ = ( bSwap_ ? rDy : rDx ) < 0;
    bNegY_ = ( bSwap_ ? rDx : rDy ) < 0;

    bSwapOffset_ = bNegX_ || bNegY_ && bNegX_ != bNegY_;

    SetOutPointXY( vOutPoint_.X() / cellSize_, vOutPoint_.Y() / cellSize_ );

    ToAlgorithmSpace( rDx, rDy );

    assert( rDx >= rDy );
    assert( rDx > 0. );
    assert( rDy >= 0. );

    rDl_ = cellSize_ * ::sqrt( rDx * rDx  + rDy * rDy + rDz_ * rDz_ ) / rDx;
    rDz_ /= ::sqrt( rDx * rDx + rDy * rDy );

    float x = vOutPoint_.X();
    float y = vOutPoint_.Y();
    ToAlgorithmSpace( x, y );
    SetOutPointXY( x, y );

    nCellColOffset_ = static_cast< int >( floor( vOutPoint_.X() ) );
    nCellRowOffset_ = static_cast< int >( floor( vOutPoint_.Y() ) );
    SetOutPointXY( vOutPoint_.X() -  nCellColOffset_, vOutPoint_.Y() - nCellRowOffset_ );
    rAlreadyUsedDX_ = vOutPoint_.X();

    assert( vOutPoint_.X() >= 0. && vOutPoint_.X() < 1. );
    assert( vOutPoint_.Y() >= 0. && vOutPoint_.Y() < 1. );

    rA0_ = rDy / rDx;
    rA1_ = rDy != 0. ? rDx / rDy : 0.f;
    rB0_ = vOutPoint_.Y() - rA0_ * vOutPoint_.X();
    rB1_ = vOutPoint_.X() - rA1_ * vOutPoint_.Y();

    rRemainingLength_ = rDl_ * rDx / cellSize_;

    // Calcul des coefficients d'environnement initiaux
    // rDx est utilisé ici pour stocker le rGroundCoeff initial
    geometry::Point2f vBeginPos2D( vBeginPos.X(), vBeginPos.Y() );
    rDx = vBeginPos.Z() - map_.ElevationAt( vBeginPos2D );

    currentCell_ = map_.Unmap( vBeginPos2D );
    // rDy est utilisé ici pour stocker le rEnvCoeff initial
    rDy = rDx - map_.EnvironmentData( currentCell_.first, currentCell_.second )->ElevationDelta();

    // Calcul des informations sur le point "sortant"
    AlignFirstPointOnGrid();
    if( eIteratorState_ == eEnd )
        return;

    // Calcul des coefficients d'environnement après le parcours du premier point
    geometry::Point2f realFirstPos = CurrentPoint();

    rGroundCoeff_ = vOutPoint_.Z() - map_.ElevationAt( realFirstPos );
    rEnvCoeff_    = rGroundCoeff_  - map_.EnvironmentAt( realFirstPos ).ElevationDelta();

    bIsInGround_  = ( rGroundCoeff_ < 0. ) || ( ( rGroundCoeff_ * rDx ) < 0. );
    bIsInEnv_     = ( rEnvCoeff_ < 0. ) || ( rEnvCoeff_ * rDy < 0. );
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::Increment
// Created: JSR 2014-06-16
// -----------------------------------------------------------------------------
void DetectionMapIterator::Increment()
{
    if( IsDone() )
        return;

    int nCellXOffset = 0;
    int nCellYOffset = 0;

    float rNextY = rA0_ * static_cast< float >( nNextCellCol_ + 1 ) + rB0_;

    // calcul de la prochaine position de l'iterateur dans l'espace de l'algorithme -> vOutPoint
    // calcul de la longueur réelle parcourue                                       -> rLenght
    // calcul du coefficient de barycentre sur l'arrête                             -> rNextY
    if( fabs( rNextY - ++nNextCellRow_ ) < rIteratorEpsilon )
    {
        nCellXOffset    = 1;
        nCellYOffset    = 1;
        SetOutPointXY( static_cast< float >( ++nNextCellCol_ ), static_cast< float >( nNextCellRow_ ) );
        rLength_        = rDl_ * ( 1.f - rAlreadyUsedDX_ );
        rNextY          = 0.;
        rAlreadyUsedDX_ = 0.;
    }
    else if( rNextY < nNextCellRow_ )
    {
        nCellXOffset    = 1;
        SetOutPointXY( static_cast< float >( ++nNextCellCol_ ), rNextY );
        --nNextCellRow_;
        rLength_        = rDl_ * ( 1.f - rAlreadyUsedDX_ );
        rNextY         -= static_cast< int >( rNextY );
        rAlreadyUsedDX_ = 0.;
    }
    else
    {
        assert( rAlreadyUsedDX_ == 0 );

        nCellYOffset    = 1;
        SetOutPointXY( rA1_ * nNextCellRow_ + rB1_, static_cast< float >( nNextCellRow_ ) );
        rAlreadyUsedDX_ = vOutPoint_.X() - static_cast< int >( vOutPoint_.X() );
        rLength_        = rDl_ * rAlreadyUsedDX_;
        rNextY          = rAlreadyUsedDX_;

        assert( rAlreadyUsedDX_ > 0. && rAlreadyUsedDX_ < 1. );
    }

    assert( rLength_ > 0. );
    assert( rNextY >= 0. );
    assert( rNextY < 1. );

    rRemainingLength_ -= rLength_;

    // condition de fin
    if( rRemainingLength_ < rIteratorEpsilon )
    {
        if( rRemainingLength_ < 0. )
        { // arrivée sur le dernier point qui se trouve entre 2 axes
            if( eIteratorState_ == eRunning )
            { // placement correct du dernier point
                float newX = vOutPoint_.X() + rRemainingLength_ / rDl_;
                SetOutPointXY( newX, rA0_ * newX + rB0_ );
                rLength_       += rRemainingLength_;

                eIteratorState_ = eLastPoint;

                assert ( rLength_ > 0. );
            }
            else
            { // fin de validité de l'itérateur
                eIteratorState_ = eEnd;
                return;
            }
        }
        else
        { // arrivée sur le dernier point ( qui se trouve sur un axe )
            rRemainingLength_ = -1;
            eIteratorState_   = eLastPoint;
        }
    }

    // calcul de la hauteur de LOS en point de sortie
    float newZ = vOutPoint_.Z() + rLength_ * rDz_;
    newZ = ( rDz_ > 0. )
        ? std::min( newZ, originalEndAltitude_ )
        : std::max( newZ, originalEndAltitude_ );
    SetOutPointZ( newZ );

    // calcul de la hauteur du SOL en point de sortie
    int nRealCellCol = nNextCellCol_ + nCellColOffset_;
    int nRealCellRow = nNextCellRow_ + nCellRowOffset_;
    OffsetToRealSpace( nCellXOffset, nCellYOffset );
    ToRealSpace      ( nRealCellCol, nRealCellRow );

    currentCell_.first = static_cast< unsigned int >( nRealCellCol );
    currentCell_.second =  static_cast< unsigned int >( nRealCellRow );

    float realX = ( nRealCellCol + rNextY * nCellXOffset ) * cellSize_;
    float realY = ( nRealCellRow + nCellYOffset * rNextY ) * cellSize_;
    float rGroundHeight = map_.ElevationAt( geometry::Point2f( realX, realY ) );

    // calcul des coefficients de changement d'environnement
    float rOldGroundCoeff = rGroundCoeff_;
    rGroundCoeff_ = vOutPoint_.Z() - rGroundHeight;

    float rOldEnvCoeff = rEnvCoeff_;
    rEnvCoeff_ = rGroundCoeff_ - map_.EnvironmentData( currentCell_.first, currentCell_.second )->ElevationDelta();

    bIsInGround_ = ( rGroundCoeff_ < 0. ) || ( ( rGroundCoeff_ * rOldGroundCoeff ) < 0. );
    bIsInEnv_ = ( rEnvCoeff_ < 0. ) || ( rEnvCoeff_ * rOldEnvCoeff < 0. );
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::IsDone
// Created: JSR 2014-06-16
// -----------------------------------------------------------------------------
bool DetectionMapIterator::IsDone() const
{
    return eIteratorState_ == eEnd;
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::Length
// Created: JSR 2014-06-16
// -----------------------------------------------------------------------------
float DetectionMapIterator::Length() const
{
    return rLength_;
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::Altitude
// Created: JSR 2014-06-16
// -----------------------------------------------------------------------------
short DetectionMapIterator::Altitude() const
{
    return map_.ElevationAt( CurrentPoint() );
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::Elevation
// Created: JSR 2014-06-16
// -----------------------------------------------------------------------------
short DetectionMapIterator::Elevation() const
{
    return Altitude() + map_.EnvironmentAt( CurrentPoint() ).ElevationDelta();
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::IsInTown
// Created: JSR 2014-06-16
// -----------------------------------------------------------------------------
bool DetectionMapIterator::IsInTown() const
{
    return map_.EnvironmentAt( CurrentPoint() ).IsInTown();

}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::IsInForest
// Created: JSR 2014-06-16
// -----------------------------------------------------------------------------
bool DetectionMapIterator::IsInForest() const
{
    return map_.EnvironmentAt( CurrentPoint() ).IsInForest();

}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::CurrentPoint
// Created: JSR 2014-06-16
// -----------------------------------------------------------------------------
geometry::Point2f DetectionMapIterator::CurrentPoint() const
{
    float x = vOutPoint_.X() + nCellColOffset_;
    float y = vOutPoint_.Y() + nCellRowOffset_;
    ToRealSpace( x, y );
    return geometry::Point2f( x * cellSize_, y*cellSize_ );
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::ToAlgorithmSpace
// Created: JSR 2014-06-16
// -----------------------------------------------------------------------------
void DetectionMapIterator::ToAlgorithmSpace( float& rX, float& rY ) const
{
    if( bSwap_ )
        std::swap( rX, rY );
    if( bNegX_ )
        rX = -rX;
    if( bNegY_ )
        rY = -rY;
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::ToRealSpace
// Created: JSR 2014-06-16
// -----------------------------------------------------------------------------
template< typename T >
void DetectionMapIterator::ToRealSpace( T& x, T& y ) const
{
    if( bNegX_ )
        x = -x;
    if( bNegY_ )
        y = -y;
    if( bSwap_ )
        std::swap( x, y );
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::OffsetToRealSpace
// Created: JSR 2014-06-16
// -----------------------------------------------------------------------------
void DetectionMapIterator::OffsetToRealSpace( int& dX, int& dY ) const
{
    bSwapOffset_ ? ToRealSpace( dY, dX ) : ToRealSpace( dX, dY );
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::AlignFirstPointOnGrid
// Created: JSR 2014-06-16
// -----------------------------------------------------------------------------
void DetectionMapIterator::AlignFirstPointOnGrid()
{
    float rNextY = rA0_ * static_cast< float >( nNextCellCol_ + 1 ) + rB0_;

    // calcul de la prochaine position de l'iterateur dans l'espace de l'algorithme -> vOutPoint
    // calcul de la longueur réelle parcourue                                       -> rLength
    if( fabs( rNextY - ++nNextCellRow_ ) < rIteratorEpsilon )
    {
        SetOutPointXY( static_cast< float >( ++nNextCellCol_ ), static_cast< float >( nNextCellRow_ ) );
        rLength_ = rDl_ * ( 1.f - rAlreadyUsedDX_ );
        rAlreadyUsedDX_ = 0.;
    }
    else if( rNextY < nNextCellRow_ )
    {
        SetOutPointXY( static_cast< float >( ++nNextCellCol_ ), rNextY );
        --nNextCellRow_;
        rLength_ = rDl_ * ( 1.f - rAlreadyUsedDX_ );
        rAlreadyUsedDX_ = 0.;
    }
    else
    {
        float rOld = rAlreadyUsedDX_;
        SetOutPointXY( rA1_ * nNextCellRow_ + rB1_, static_cast< float >( nNextCellRow_ ) );
        rAlreadyUsedDX_ = vOutPoint_.X() - static_cast< int >( vOutPoint_.X() );
        rLength_ = rDl_ * ( rAlreadyUsedDX_ - rOld );

        assert( rAlreadyUsedDX_ > 0. && rAlreadyUsedDX_ < 1. );
    }

    assert( rLength_ > 0. );

    rRemainingLength_ -= rLength_;

    // condition de fin
    if( rRemainingLength_ < rIteratorEpsilon )
    {
        if( rRemainingLength_ < 0. )
        { // arrivée sur le dernier point qui se trouve entre 2 axes
            if( eIteratorState_ == eRunning )
            { // placement correct du dernier point
                float tmpX = vOutPoint_.X() +rRemainingLength_ / rDl_;
                rLength_ += rRemainingLength_;
                float tmpY = rA0_ * tmpX + rB0_;

                SetOutPointXY( tmpX, tmpY );

                eIteratorState_ = eLastPoint;

                assert( rLength_ > 0. );
            }
            else
            { // fin de validité de l'itérateur
                eIteratorState_ = eEnd;
                return;
            }
        }
        else
        { // arrivée sur le dernier point ( qui se trouve sur un axe )
            rRemainingLength_ = -1;
            eIteratorState_ = eLastPoint;
        }
    }

    // calcul de la hauteur de LOS en point de sortie
    SetOutPointZ( vOutPoint_.Z() + rLength_ * rDz_ );
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::SetOutPointXY
// Created: JSR 2014-06-16
// -----------------------------------------------------------------------------
void DetectionMapIterator::SetOutPointXY( float x, float y )
{
    vOutPoint_ = geometry::Point3f( x, y, vOutPoint_.Z() );
}

// -----------------------------------------------------------------------------
// Name: DetectionMapIterator::SetOutPointZ
// Created: JSR 2014-06-16
// -----------------------------------------------------------------------------
void DetectionMapIterator::SetOutPointZ( float z )
{
    vOutPoint_ = geometry::Point3f( vOutPoint_.X(), vOutPoint_.Y(), z );
}
