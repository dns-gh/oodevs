//*****************************************************************************
//
// $Created: JVT 03-06-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Meteo/RawVisionData/PHY_RawVisionDataIterator.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_RawVisionDataIterator.h $
//
//*****************************************************************************

#ifndef __PHY_RawVisionDataIterator_h_
#define __PHY_RawVisionDataIterator_h_

#include "PHY_RawVisionData.h"

//*****************************************************************************
// Created: JVT 03-06-05
// Last modified: JVT 03-08-05
//*****************************************************************************
class PHY_RawVisionDataIterator
{
    MT_COPYNOTALLOWED( PHY_RawVisionDataIterator )

public:
    PHY_RawVisionDataIterator( const MT_Vector3D& beginPos, const MT_Vector3D& endPos );

    PHY_RawVisionDataIterator& operator ++ ();
    
    bool                       End             () const;
    MT_Float                   Length          () const;
    PHY_RawVisionData::envBits GetCurrentEnv   () const;
    const PHY_Lighting&       GetLighting     () const;
    const PHY_Precipitation&  GetPrecipitation() const;

    const MT_Vector2D          GetPos()               const;
    void                       GetPos( MT_Vector2D& ) const;
    
protected:
    // Transformations entre l'espace réel et l'espace de l'algorithme
    void ToAlgorithmSpace ( MT_Float& rX, MT_Float& rY ) const;
    void ToAlgorithmSpace ( int& nX, int& nY ) const;
    void ToRealSpace      ( MT_Float& rX, MT_Float& rY ) const;
    void ToRealSpace      ( int& nX, int& nY ) const;
    void OffsetToRealSpace( int& dX, int& dY ) const;

private:
    inline void AlignFirstPointOnGrid();

protected:
    // Informations statiques environnementales
    const PHY_RawVisionData& data_;

    // gradients
    /*const*/ MT_Float rDz_;
    /*const*/ MT_Float rDl_;

    // Equations de la droite
    // y = a0 * x + b0
    // x = a1 * y + b1
    /*const*/ MT_Float rA0_;
    /*const*/ MT_Float rA1_;
    /*const*/ MT_Float rB0_;
    /*const*/ MT_Float rB1_;

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
    MT_Float rLenght_;

    // Distance totale restant à parcourir ( condition de fin )
    MT_Float rRemainingLength_;
    
    // Prochaine position de l'itérateur
    MT_Float    rAlreadyUsedDX_;        // Projection de la distance déjà parcourue sur l'axe des x dans la précédente itération
    int         nNextCellCol_;
    int         nNextCellRow_;
    MT_Vector3D vOutPoint_;             // X, Y dans l'espace de l'algorithme, Z non transformée
    
    // Informations sur l'environnement précedent
    MT_Float rGroundCoeff_;
    MT_Float rEnvCoeff_;

    // Environnement courrant
    const PHY_RawVisionData::sCell*  pCurrentCell_;
    bool                             bIsInGround_;
    bool                             bIsInEnv_;
};

#	include "PHY_RawVisionDataIterator.inl"
#endif // __PHY_RawVisionDataIterator_h_