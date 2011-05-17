// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************
#include "simulation_kernel_pch.h"
#include "PHY_RawVisionDataIterator.h"
#include "MIL_AgentServer.h"
#include "Meteo/PHY_MeteoDataManager.h"

static const double rIteratorEpsilon = 1e-10;

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionDataIterator::AlignFirstPointOnGrid
// Created: JVT 03-06-23
// Last modified: JVT 03-06-24
//-----------------------------------------------------------------------------
void PHY_RawVisionDataIterator::AlignFirstPointOnGrid()
{
    double rNextY = rA0_ * static_cast< double >( nNextCellCol_ + 1 ) + rB0_;

    // calcul de la prochaine position de l'iterateur dans l'espace de l'algorithme -> vOutPoint
    // calcul de la longueur réelle parcourue                                       -> rLenght
    if( fabs( rNextY - ++nNextCellRow_ ) < rIteratorEpsilon )
    {
        vOutPoint_.rX_ = ++nNextCellCol_;
        vOutPoint_.rY_ = nNextCellRow_;
        rLength_ = rDl_ * ( 1. - rAlreadyUsedDX_ );
        rAlreadyUsedDX_ = 0.;
    }
    else if( rNextY < nNextCellRow_ )
    {
        vOutPoint_.rX_ = ++nNextCellCol_;
        vOutPoint_.rY_ = rNextY;
        --nNextCellRow_;
        rLength_ = rDl_ * ( 1. - rAlreadyUsedDX_ );
        rAlreadyUsedDX_ = 0.;
    }
    else
    {
        double rOld = rAlreadyUsedDX_;
        vOutPoint_.rY_ = nNextCellRow_;
        vOutPoint_.rX_ = rA1_ * vOutPoint_.rY_ + rB1_;
        rAlreadyUsedDX_ = vOutPoint_.rX_ - static_cast< int >( vOutPoint_.rX_ );
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
                vOutPoint_.rX_ += rRemainingLength_ / rDl_;
                rLength_ += rRemainingLength_;
                vOutPoint_.rY_ = rA0_ * vOutPoint_.rX_ + rB0_;

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
    vOutPoint_.rZ_ += rLength_ * rDz_;
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionDataIterator::PHY_RawVisionDataIterator
// Created: JVT 03-03-29
// Last modified: JVT 03-06-24
//-----------------------------------------------------------------------------
/*
Transformations pour passer de l'espace réel en l'espace de l'algorithme :
_ rotation + symétrie pour que le rayon soit dans le secteur [ 0°, 45° ]
_ homothétie pour rendre normaliser la taille des cases
_ translation pour obtenir des ids de case positifs dans l'espace de l'algorithme

_ le calcul de la longueur parcourue se fait par rapport au ratio de la projection
  de la longueur réelle sur l'axe X de l'espace de l'algorithme
_ le calcul du gradian de hauteur se fait par projection de ce gradian sur la longueur
*/
PHY_RawVisionDataIterator::PHY_RawVisionDataIterator( const MT_Vector3D& vBeginPos, const MT_Vector3D& vEndPos )
    : data_          ( MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData() )
    , rDz_           ( vEndPos.rZ_ - vBeginPos.rZ_ )
    , eIteratorState_( eRunning )
    , nNextCellCol_  ( 0 )
    , nNextCellRow_  ( 0 )
    , vOutPoint_     ( vBeginPos )
    , pCurrentCell_  ( 0 )
{
    double rDx = vEndPos.rX_ - vOutPoint_.rX_;
    double rDy = vEndPos.rY_ - vOutPoint_.rY_;

    if( rDx == 0 && rDy == 0 )  // -> vBeginPos == vEndPos
    {
        eIteratorState_ = eEnd;
        return;
    }

    bSwap_ = fabs( rDy ) > fabs( rDx );
    bNegX_ = ( bSwap_ ? rDy : rDx ) < 0;
    bNegY_ = ( bSwap_ ? rDx : rDy ) < 0;

    bSwapOffset_ = bNegX_ || bNegY_ && bNegX_ != bNegY_;

    const double rCellSize = data_.GetCellSize();

    vOutPoint_.rX_ /= rCellSize;
    vOutPoint_.rY_ /= rCellSize;

    ToAlgorithmSpace( rDx, rDy );

    assert( rDx >= rDy );
    assert( rDx > 0. );
    assert( rDy >= 0. );

    rDl_ = rCellSize * ::sqrt( rDx * rDx  + rDy * rDy + rDz_ * rDz_ ) / rDx;
    rDz_ /= ::sqrt( rDx * rDx + rDy * rDy );

    ToAlgorithmSpace( vOutPoint_.rX_, vOutPoint_.rY_ );

    rAlreadyUsedDX_ = ( vOutPoint_.rX_ -= ( nCellColOffset_ = static_cast< int >( floor( vOutPoint_.rX_ ) ) ) );
    vOutPoint_.rY_ -= ( nCellRowOffset_ = static_cast< int >( floor( vOutPoint_.rY_ ) ) );

    assert( vOutPoint_.rX_ >= 0. && vOutPoint_.rX_ < 1. );
    assert( vOutPoint_.rY_ >= 0. && vOutPoint_.rY_ < 1. );

    rA0_ = rDy / rDx;
    rA1_ = rDy != 0. ? rDx / rDy : 0.;
    rB0_ = vOutPoint_.rY_ - rA0_ * vOutPoint_.rX_;
    rB1_ = vOutPoint_.rX_ - rA1_ * vOutPoint_.rY_;

    rRemainingLength_ = rDl_ * rDx / rCellSize;

    // Calcul des coefficients d'environnement initiaux
    // rDx est utilisé ici pour stocker le rGroundCoeff initial
    rDx = vBeginPos.rZ_ - data_.GetAltitude( vBeginPos.rX_, vBeginPos.rY_ );

    pCurrentCell_ = &data_( vBeginPos.rX_, vBeginPos.rY_ );
    // rDy est utilisé ici pour stocker le rEnvCoeff initial
    rDy = rDx - pCurrentCell_->GetEnvHeight();

    // Calcul des informations sur le point "sortant"
    AlignFirstPointOnGrid();
    if( eIteratorState_ == eEnd )
        return;

    // Calcul des coefficients d'environnement après le parcours du premier point
    MT_Vector2D realFirstPos( vOutPoint_.rX_ + nCellColOffset_, vOutPoint_.rY_ + nCellRowOffset_ );
    ToRealSpace( realFirstPos.rX_, realFirstPos.rY_ );
    realFirstPos *= rCellSize;

    rGroundCoeff_ = vOutPoint_.rZ_ - data_.GetAltitude( realFirstPos.rX_, realFirstPos.rY_ );
    rEnvCoeff_    = rGroundCoeff_  - data_( realFirstPos.rX_, realFirstPos.rY_ ).GetEnvHeight();

    bIsInGround_  = ( rGroundCoeff_ < 0. ) || ( ( rGroundCoeff_ * rDx ) < 0. );
    bIsInEnv_     = ( rEnvCoeff_ < 0. ) || ( rEnvCoeff_ * rDy < 0. );
}

//-----------------------------------------------------------------------------
// Name: PHY_RawVisionDataIterator::operator ++
// Created: JVT 03-03-29
// Last modified: JVT 03-06-24
//-----------------------------------------------------------------------------
PHY_RawVisionDataIterator& PHY_RawVisionDataIterator::operator ++()
{
    if( End() )
        return *this;

    int nCellXOffset = 0;
    int nCellYOffset = 0;

    double rNextY = rA0_ * static_cast< double >( nNextCellCol_ + 1 ) + rB0_;

    // calcul de la prochaine position de l'iterateur dans l'espace de l'algorithme -> vOutPoint
    // calcul de la longueur réelle parcourue                                       -> rLenght
    // calcul du coefficient de barycentre sur l'arrête                             -> rNextY
    if( fabs( rNextY - ++nNextCellRow_ ) < rIteratorEpsilon )
    {
        nCellXOffset    = 1;
        nCellYOffset    = 1;
        vOutPoint_.rX_  = ++nNextCellCol_;
        vOutPoint_.rY_  = nNextCellRow_;
        rLength_        = rDl_ * ( 1. - rAlreadyUsedDX_ );
        rNextY          = 0.;
        rAlreadyUsedDX_ = 0.;
    }
    else if( rNextY < nNextCellRow_ )
    {
        nCellXOffset    = 1;
        vOutPoint_.rX_  = ++nNextCellCol_;
        vOutPoint_.rY_  = rNextY;
        --nNextCellRow_;
        rLength_        = rDl_ * ( 1. - rAlreadyUsedDX_ );
        rNextY         -= static_cast< int >( rNextY );
        rAlreadyUsedDX_ = 0.;
    }
    else
    {
        assert( rAlreadyUsedDX_ == 0 );

        nCellYOffset    = 1;
        vOutPoint_.rY_  = nNextCellRow_;
        vOutPoint_.rX_  = rA1_ * vOutPoint_.rY_ + rB1_;
        rAlreadyUsedDX_ = vOutPoint_.rX_ - static_cast< int >( vOutPoint_.rX_ );
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
                vOutPoint_.rX_ += rRemainingLength_ / rDl_;
                rLength_       += rRemainingLength_;
                vOutPoint_.rY_  = rA0_ * vOutPoint_.rX_ + rB0_;

                eIteratorState_ = eLastPoint;

                assert ( rLength_ > 0. );
            }
            else
            { // fin de validité de l'itérateur
                eIteratorState_ = eEnd;
                return *this;
            }
        }
        else
        { // arrivée sur le dernier point ( qui se trouve sur un axe )
            rRemainingLength_ = -1;
            eIteratorState_   = eLastPoint;
        }
    }

    // calcul de la hauteur de LOS en point de sortie
    vOutPoint_.rZ_ += rLength_ * rDz_;

    // calcul de la hauteur du SOL en point de sortie
    int nRealCellCol = nNextCellCol_ + nCellColOffset_;
    int nRealCellRow = nNextCellRow_ + nCellRowOffset_;
    OffsetToRealSpace( nCellXOffset, nCellYOffset );
    ToRealSpace      ( nRealCellCol, nRealCellRow );

    pCurrentCell_ = &data_( static_cast< unsigned int >( nRealCellCol ), static_cast< unsigned int >( nRealCellRow ) );

    double rGroundHeight  = rNextY * data_( static_cast< unsigned int >( nRealCellCol + nCellYOffset ), static_cast< unsigned int >( nRealCellRow + nCellXOffset ) ).GetAltitude() + pCurrentCell_->GetAltitude() * ( 1. - rNextY );

    // calcul des coefficients de changement d'environnement
    double rOldGroundCoeff = rGroundCoeff_;
    rGroundCoeff_ = vOutPoint_.rZ_ - rGroundHeight;

    double rOldEnvCoeff = rEnvCoeff_;
    rEnvCoeff_ = rGroundCoeff_ - pCurrentCell_->GetEnvHeight();

    bIsInGround_ = ( rGroundCoeff_ < 0. ) || ( ( rGroundCoeff_ * rOldGroundCoeff ) < 0. );
    bIsInEnv_ = ( rEnvCoeff_ < 0. ) || ( rEnvCoeff_ * rOldEnvCoeff < 0. );

    return *this;
}
