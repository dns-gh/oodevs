//*****************************************************************************
//
// $Created: JDY 03-05-19 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_Polygon.h $
// $Author: Age $
// $Modtime: 16/06/05 9:57 $
// $Revision: 4 $
// $Workfile: TER_Polygon.h $
//
//*****************************************************************************

#ifndef __TER_Polygon_h_
#define __TER_Polygon_h_

#include "TER_Localisation_ABC.h"
#include "MT_Tools/MT_Rect.h"
#include <boost/shared_ptr.hpp>

class MT_Polyline;
class MT_Droite;

//*****************************************************************************
// Created: JDY 03-05-19
//*****************************************************************************
class TER_Polygon: public TER_Localisation_ABC
{
public:
    struct PolygonData;

    // @name Constructors/Destructor
    //@{
             TER_Polygon();
             TER_Polygon( const TER_Polygon& poly );
    virtual ~TER_Polygon();
    //@}

    // @name Operator
    //@{
    TER_Polygon &operator=( const TER_Polygon & );
    //@}

    // @name Reset
    //@{
    virtual void Reset( const T_PointVector& points, bool bConvexHull = false );
    virtual void Reset();
    //@}

    // @name Tools
    //@{
    virtual bool IsNull               () const;
    virtual bool IsInside             ( const MT_Vector2D& vPos ) const;
            bool IsInside             ( const MT_Vector2D& vPos, double rPrecision ) const; // $$$$ AGE 2005-01-31: stoopid precision
            bool IsInsidish           ( const MT_Vector2D& vPos ) const;
            bool IsOnBorder           ( const MT_Vector2D& vPos, double rPrecision ) const;
            bool IntersectWithBorder  ( const MT_Line& segment, double rPrecision     ) const;
            bool IntersectWithBorder  ( const MT_Line& segment, T_PointSet& res, double rPrecision ) const;
            bool IntersectWithBorder  ( const MT_Droite& line, T_PointSet& res ) const;
            bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const; // NB : return true if the circle is inside
            bool Intersect2D          ( const MT_Line& segment, double rPrecision     ) const;
            bool Intersect2D          ( const MT_Line& segment, T_PointSet& res, double rPrecision ) const;
            bool Intersect2D          ( const MT_Droite& line , T_PointSet& res, double rPrecision ) const;
            void Intersection         (       MT_Polyline& polyline, double rPrecision ) const;

            void ComputeExtremities   ( const MT_Droite& line, MT_Vector2D& vLeftPoint, MT_Vector2D& vRightPoint ) const; // Retourne les deux du polygone sur la droite

            double GetArea          () const;

    const T_PointVector&       GetBorderPoints() const; //$$$ A CLEANER

    virtual MT_Rect GetBoundingBox() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_BoundedSize
    {
        eOnUnboundedSide,
        eOnBoundary,
        eOnBoundedSide
    };
    //@}

    //! @name Helpers
    //@{
    double      ComputeArea() const;
    E_BoundedSize BoundedSide( const MT_Vector2D& pos ) const;
    bool          IsInBoundingBox( const MT_Vector2D& p, double rPrecision = 0 ) const;
    //@}

private:
    // Immutability allows it to be shared between instances.
    boost::shared_ptr< const PolygonData > pData_;
};

#endif // __TER_Polygon_h_
