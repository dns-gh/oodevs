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

#include "TER_Polygon.h"
#include "TER_Localisation_ABC.h"
#include "MT_Tools/MT_Polyline.h"
#include <boost/serialization/split_member.hpp>

namespace xml
{
    class xistream;
    class xostream;
}

class MT_Circle;
class MT_Rect;
class MT_Ellipse;

//=============================================================================
// Created: NLD 2003-07-22
// Last modified: JVT 04-05-28
//=============================================================================
class TER_Localisation: public TER_Localisation_ABC
{
public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    enum E_LocationType
    {
        eCircle     = 0,
        eEllipse    = 1,
        eLine       = 2,
        eRectangle  = 3,
        ePolygon    = 4,
        ePoint      = 5,
        eSector     = 6,
        eNone       = 7
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TER_Localisation();
             TER_Localisation( const TER_Localisation& );
             TER_Localisation( E_LocationType, const T_PointVector& );
             TER_Localisation( const TER_Polygon&  );
             TER_Localisation( const MT_Vector2D& vPos, double rRadius );
             TER_Localisation( const MT_Ellipse& ellipse );
    virtual ~TER_Localisation();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    template< class Archive >
    void load( Archive&, const unsigned int );
    template< class Archive >
    void save( Archive&, const unsigned int ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< TER_Localisation > > T_LocalisationPtrVector;
    //@}

    //-------------------------------------------------------------------------
    /** @name Main */
    //-------------------------------------------------------------------------
    //@{
    // $$$$ JVT : ?????
    bool Reset( E_LocationType nType, const T_PointVector& pointVector, double rPointSize = 250. );
    bool Reset( E_LocationType nType, const T_PointList& pointList, double rPointSize = 250. );
    void Reset();
    void Reset( const TER_Localisation& localisation, double rPointSize = 0. );
    void Reset( const MT_Vector2D& vPos );
    bool Reset( const T_PointVector& pointVector );
    bool Reset( const T_PointList&   pointList   );
    void Read ( xml::xistream& xis ) ;
    void Write( xml::xostream& xos ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Operators */
    //-------------------------------------------------------------------------
    //@{
    bool operator == ( const TER_Localisation& localisation ) const;
    bool operator != ( const TER_Localisation& localisation ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Geometry */
    //-------------------------------------------------------------------------
    //@{
    virtual MT_Rect GetBoundingBox() const;
    virtual bool IsInside( const MT_Vector2D& vPos ) const;

    bool IsEmpty() const;
    double       GetArea                      () const;
    double       GetLength                    () const;
    double       GetIntersectionAreaWithCircle( const MT_Circle& circle ) const;

    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const; // NB : return true if the circle is inside
    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius, T_PointVector& shape ) const; // NB : return true if the circle is inside
    bool IsInside             ( const MT_Vector2D& vPos, double rPrecision ) const;
    bool IsOnBorder           ( const MT_Vector2D& vPos, double rPrecision = rPrecision_ ) const;
    bool IsIntersecting       ( const TER_Localisation& localisation, double rPrecision = rPrecision_ ) const;
    bool IsIntersecting       ( const TER_Polygon& polygon, double rPrecision = rPrecision_ ) const;
    bool Intersect2D          ( const MT_Line& orientedLine, T_PointSet& collisions, double rPrecision = rPrecision_ ) const;
    bool Intersect2D          ( const MT_Line& line, double rPrecision = rPrecision_ ) const;
    bool Contains             ( const TER_Localisation& localisation, double rPrecision = rPrecision_ ) const;
    
    void ComputeNearestPoint            ( const MT_Vector2D& vSrc, MT_Vector2D& vResult ) const; // Calcule la position de la localisation la plus proche de vSrc
    void ComputeNearestOutsidePoint     ( const MT_Vector2D& vSrc, MT_Vector2D& vResult ) const;
    void ComputeNearestPoint            ( const TER_Localisation& localisation, MT_Vector2D& vResult, double& rMinDistance ) const;
    void GetPointsClippedByPolygon      ( const TER_Polygon& polygon, T_PointVector& clippedPointVector ) const;
    void GetPointsClippedByLocalisation ( const TER_Localisation& localisation, T_PointVector& clippedPointVector ) const;
    void GetPointsClippedBetweenTwoLines( const MT_Droite& leftDroite, const MT_Droite& rightDroite, const MT_Vector2D& vDroitesIntersection, T_PointVector& clippedPointVector ) const;
    void GetPointsClippedBetweenTwoLines( const MT_Droite& leftDroite, const MT_Droite& rightDroite, T_PointVector& clippedPointVector ) const;

    void Split    ( unsigned int nNbrParts, T_LocalisationPtrVector& locVector, const MT_Vector2D* splitDirection = 0 ) const;
    void Scale    ( double rDist );
    void Convexify();

    MT_Vector2D ComputeBarycenter() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const T_PointVector& GetPoints() const;
    E_LocationType   GetType  () const;
    std::string GetTypeString() const;

    // Circle <---- $$$ C'est de la grosse merde en plastique, vraiment
          bool         WasACircle     () const;
          double     GetCircleRadius() const;
    const MT_Vector2D& GetCircleCenter() const;
    //@}

    //! @name Tools
    //@{
    static E_LocationType ConvertLocalisationType( const std::string& strType ); // A deplacer dans TER_Tools si nécessaire
    static std::string        ConvertLocalisationType( E_LocationType );         // A deplacer dans TER_Tools si nécessaire
    static double DefaultPrecision();
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    bool Initialize( double rPointSize = 250. );

    bool InitializePolygon  ();
    bool InitializeLine     ();
    bool InitializePoint    ( double rPointSize );
    bool InitializeSector   ();
    bool InitializeCircle   ();
    bool InitializeEllipse  ();

    void InitializeBoundingBox( CT_PointVector& pointVector );
    //@}

    //! @name Helpers
    //@{
    void ReadPoint( xml::xistream& xis );
    bool IsIntersectingWithBorderLines( const MT_Line& line ) const;
    //@}

protected:
    E_LocationType nType_;
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
    double     rCircleRadius_;

private:
    static const double rPrecision_;
};

// -----------------------------------------------------------------------------
// Name: TER_Localisation::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
template<class Archive>
void TER_Localisation::load( Archive& ar, const unsigned int )
{
    ar >> nType_
       >> pointVector_
       >> bWasCircle_
       >> vCircleCenter_
       >> rCircleRadius_;
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: TER_Localisation::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
template<class Archive>
void TER_Localisation::save( Archive& ar, const unsigned int ) const
{
    ar << nType_
       << pointVector_
       << bWasCircle_
       << vCircleCenter_
       << rCircleRadius_;
}

#endif // __TER_Localisation_h_
