// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RawVisionDataIterator_h_
#define __PHY_RawVisionDataIterator_h_

#include "PHY_RawVisionData.h"
#include "MT_Tools/MT_Vector3D.h"

//*****************************************************************************
// Created: JVT 03-06-05
// Last modified: JVT 03-08-05
//*****************************************************************************
class PHY_RawVisionDataIterator : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_RawVisionDataIterator( const MT_Vector3D& beginPos, const MT_Vector3D& endPos );
    virtual ~PHY_RawVisionDataIterator() {}
    //@}

    PHY_RawVisionDataIterator& operator ++ ();

    bool End() const;
    double Length() const;
    PHY_RawVisionData::envBits GetCurrentEnv() const;
    const weather::PHY_Lighting& GetLighting() const;
    const weather::PHY_Precipitation& GetPrecipitation() const;

    const MT_Vector2D GetPos() const;
    void GetPos( MT_Vector2D& ) const;

protected:
    // Transformations entre l'espace réel et l'espace de l'algorithme
    void ToAlgorithmSpace ( double& rX, double& rY ) const;
    void ToAlgorithmSpace ( int& nX, int& nY ) const;
    void ToRealSpace      ( double& rX, double& rY ) const;
    void ToRealSpace      ( int& nX, int& nY ) const;
    void OffsetToRealSpace( int& dX, int& dY ) const;

private:
    inline void AlignFirstPointOnGrid();

protected:
    // Informations statiques environnementales
    const PHY_RawVisionData& data_;

    // gradients
    /*const*/ double rDz_;
    /*const*/ double rDl_;

    // Equations de la droite
    // y = a0 * x + b0
    // x = a1 * y + b1
    /*const*/ double rA0_;
    /*const*/ double rA1_;
    /*const*/ double rB0_;
    /*const*/ double rB1_;

    // Informations de tranformation de l'espace réel en l'espace idéal ( 0, 45° ) pour l'algo de parcours
    /*const*/ bool bSwap_;
    /*const*/ bool bNegX_;
    /*const*/ bool bNegY_;
    /*const*/ bool bSwapOffset_;

    // Case réelle initiale
    /*const*/ int nCellColOffset_;
    /*const*/ int nCellRowOffset_;

    // information de fin ( pour faire un end() d'itérateur à la STL )
    enum E_IteratorState
    {
        eRunning,
        eLastPoint,
        eEnd
    };
    E_IteratorState eIteratorState_;

    // Distance parcourue dans la cellule courrante
    double rLength_;

    // Distance totale restant à parcourir ( condition de fin )
    double rRemainingLength_;

    // Prochaine position de l'itérateur
    double rAlreadyUsedDX_; // Projection de la distance déjà parcourue sur l'axe des x dans la précédente itération
    int nNextCellCol_;
    int nNextCellRow_;
    MT_Vector3D vOutPoint_; // X, Y dans l'espace de l'algorithme, Z non transformée

    // Informations sur l'environnement précedent
    double rGroundCoeff_;
    double rEnvCoeff_;

    // Environnement courrant
    const PHY_RawVisionData::sCell* pCurrentCell_;
    bool bIsInGround_;
    bool bIsInEnv_;
};

#include "PHY_RawVisionDataIterator.inl"

#endif // __PHY_RawVisionDataIterator_h_
