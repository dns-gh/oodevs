// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __DetectionMapIterator_h_
#define __DetectionMapIterator_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class DetectionMap;

// =============================================================================
/** @class  DetectionMapIterator
    @brief  DetectionMapIterator
*/
// Created: AGE 2006-04-06
// =============================================================================
class DetectionMapIterator : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DetectionMapIterator( const DetectionMap& map, const geometry::Point3f& vBeginPos, const geometry::Point3f& vEndPos );
    virtual ~DetectionMapIterator();
    //@}

    //! @name Operations
    //@{
    void Increment();
    bool IsDone() const;
    float Length() const;
    short Altitude() const;
    short Elevation() const;
    bool IsInTown() const;
    bool IsInForest() const;
    geometry::Point2f CurrentPoint() const;
    //@}

private:
    //! @name Helpers
    //@{
    // Transformations entre l'espace r�el et l'espace de l'algorithme
    void ToAlgorithmSpace ( float& rX, float& rY ) const;
    template< typename T >
    void ToRealSpace( T& x, T& y ) const;
    void OffsetToRealSpace( int& dX, int& dY ) const;
    void AlignFirstPointOnGrid();
    void SetOutPointXY( float x, float y );
    void SetOutPointZ( float z );
    //@}

private:
    //! @name Member data
    //@{
    const DetectionMap& map_;
    float cellSize_;
    std::pair< unsigned, unsigned > currentCell_;

    // gradients
    float rDz_;
    float rDl_;

    // Equations de la droite
    // y = a0 * x + b0
    // x = a1 * y + b1
    float rA0_;
    float rA1_;
    float rB0_;
    float rB1_;

    // Informations de tranformation de l'espace r�el en l'espace id�al ( 0, 45� ) pour l'algo de parcours
    bool bSwap_;
    bool bNegX_;
    bool bNegY_;
    bool bSwapOffset_;

    // Case r�elle initiale
    int nCellColOffset_;
    int nCellRowOffset_;

    // information de fin ( pour faire un end() d'it�rateur � la STL )
    enum E_IteratorState
    {
        eRunning,
        eLastPoint,
        eEnd
    };
    E_IteratorState eIteratorState_;

    // Distance parcourue dans la cellule courrante
    float rLength_;

    // Distance totale restant � parcourir ( condition de fin )
    float rRemainingLength_;

    // Prochaine position de l'it�rateur
    float rAlreadyUsedDX_; // Projection de la distance d�j� parcourue sur l'axe des x dans la pr�c�dente it�ration
    int nNextCellCol_;
    int nNextCellRow_;
    geometry::Point3f vOutPoint_; // X, Y dans l'espace de l'algorithme, Z non transform�e
    const float originalEndAltitude_;

    // Informations sur l'environnement pr�cedent
    float rGroundCoeff_;
    float rEnvCoeff_;

    // Environnement courrant
    bool bIsInGround_;
    bool bIsInEnv_;
    //@}
};

}

#endif // __DetectionMapIterator_h_
