//*****************************************************************************
//
// $Created: NLD 2003-07-22 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_Localisation.h $
// $Author: Jvt $
// $Modtime: 23/03/05 18:04 $
// $Revision: 2 $
// $Workfile: TER_Localisation.h $
//
//*****************************************************************************

#ifndef __TER_Localisation_h_
#define __TER_Localisation_h_

#include "TER.h"

#include "TER_Polygon.h"
#include "MT_Tools/MT_Polyline.h"

template< typename T > class MT_InputArchive_Logger;
class MT_XXmlInputArchive;

//=============================================================================
// Created: NLD 2003-07-22
// Last modified: JVT 04-05-28
//=============================================================================
class TER_Localisation
{
public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    enum E_TypeLocalisation
    {
        eCircle     = 0,
        eEllipse    = 1,
        eLine       = 2,
        ePolygon    = 3,
        ePoint      = 4,
        eSector     = 5,
        eNone       = 6
    };
    //@}

public:
     TER_Localisation();
     TER_Localisation( const TER_Localisation& );
     TER_Localisation( E_TypeLocalisation, const T_PointVector& );
     TER_Localisation( const MT_Vector2D& vPos, MT_Float rRadius );
    ~TER_Localisation();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( boost::archive::binary_iarchive&, const uint );
    void save( boost::archive::binary_oarchive&, const uint ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Main */
    //-------------------------------------------------------------------------
    //@{
    // $$$$ JVT : ?????
    bool Reset( E_TypeLocalisation nType, const T_PointVector& pointVector );
    void Reset();
    void Reset( const TER_Localisation& localisation );
    void Reset( const MT_Vector2D& vPos );
    bool Reset( const T_PointVector& pointVector );
    bool Reset( const T_PointList&   pointList   );
    void Read ( MT_InputArchive_Logger< MT_XXmlInputArchive >& archive );
    void Write( MT_OutputArchive_ABC& archive ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Operators */
    //-------------------------------------------------------------------------
    //@{
    bool operator == ( const TER_Localisation& localisation ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Geometry */
    //-------------------------------------------------------------------------
    //@{
    const MT_Rect& GetBoundingBox() const;
    MT_Float       GetArea       () const;
    MT_Float       GetLength     () const;

    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius ) const; // NB : return true if the circle is inside
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius, T_PointVector& shape ) const; // NB : return true if the circle is inside
    bool IsInside             ( const MT_Vector2D& vPos ) const;
    bool IsOnBorder           ( const MT_Vector2D& vPos ) const;
    bool IsIntersecting       ( const TER_Localisation& localisation ) const;
    bool Intersect2D          ( const MT_Line& orientedLine, T_PointSet& collisions ) const;
    bool Intersect2D          ( const MT_Line& line ) const;

    bool ComputeNearestPoint            ( const MT_Vector2D& vSrc, MT_Vector2D& vResult ) const; // Calcule la position de la localisation la plus proche de vSrc
    void GetPointsClippedByPolygon      ( const TER_Polygon& polygon, T_PointVector& clippedPointVector ) const;
    void GetPointsClippedByLocalisation ( const TER_Localisation& localisation, T_PointVector& clippedPointVector ) const;
    void GetPointsClippedBetweenTwoLines( const MT_Droite& leftDroite, const MT_Droite& rightDroite, const MT_Vector2D& vDroitesIntersection, T_PointVector& clippedPointVector ) const;
    void GetPointsClippedBetweenTwoLines( const MT_Droite& leftDroite, const MT_Droite& rightDroite, T_PointVector& clippedPointVector ) const;

    void Split( uint nNbrParts, T_LocalisationPtrVector& locVector ) const;
    void Scale( MT_Float rDist );

    MT_Vector2D ComputeBarycenter() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const T_PointVector& GetPoints() const;
    E_TypeLocalisation   GetType  () const;

    // Circle <---- $$$ C'est de la grosse merde en plastique, vraiment
          bool         WasACircle     () const;
          MT_Float     GetCircleRadius() const;
    const MT_Vector2D& GetCircleCenter() const;
    //@}
    
    //! @name Tools
    //@{
    static E_TypeLocalisation ConvertLocalisationType( const std::string& strType ); // A deplacer dans TER_Tools si nécessaire
    static std::string        ConvertLocalisationType( E_TypeLocalisation );         // A deplacer dans TER_Tools si nécessaire
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    bool Initialize();

    bool InitializePolygon  ();
    bool InitializeLine     ();
    bool InitializePoint    ();
    bool InitializeSector   ();
    bool InitializeCircle   ();
    bool InitializeEllipse  ();

    void InitializeBoundingBox( CT_PointVector& pointVector );
    //@}
    
protected:
    E_TypeLocalisation nType_;
    T_PointVector      pointVector_; // Initial points

    // $$$$ JVT : Hummmm ça ne serait pas une union...                  ?
    // $$$$ JVT : Hummmm et les autres types ( ellipse, cercles, .... ) ?
    // $$$$ JVT : Pourquoi on n'a pas une Locatiosation_ABC avec les fils suivant le type ?

    TER_Polygon    polygon_;  // Used when the localisation is a polygon
    MT_Polyline    polyline_;

    MT_Rect boundingBox_;

    // Circle   
    bool         bWasCircle_;
    MT_Vector2D  vCircleCenter_;
    MT_Float     rCircleRadius_;

private:
    static const MT_Float rPrecision_;
};

#endif // __TER_Localisation_h_
