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

#include "TER.h"

#include "MT_Tools/MT_Shared.h"
#include "MT_Tools/MT_Rect.h"

class MT_Polyline;

//*****************************************************************************
// Created: JDY 03-05-19
//*****************************************************************************
class TER_Polygon
{
    MT_BOOSTPOOLING_MEMBERS( TER_Polygon )

public:
    // @name Constructors/Destructor 
    //@{
             TER_Polygon();
    virtual ~TER_Polygon();
    //@}

    // @name Operator 
    //@{
    TER_Polygon &operator=( const TER_Polygon & );
    //@}

    // @name Reset
    //@{
    virtual void Reset( const T_PointPtrVector& points, bool bConvexHull = false );
    virtual void Reset( const T_PointVector& points, bool bConvexHull = false );
    virtual void Reset();
    //@}
    
    // @name Tools
    //@{
    virtual bool IsNull               () const;
            bool IsInside             ( const MT_Vector2D& vPos, MT_Float rPrecision = 0.1 ) const; // $$$$ AGE 2005-01-31: stoopid precision
            bool IsInsidish           ( const MT_Vector2D& vPos ) const;
            bool IsOnBorder           ( const MT_Vector2D& vPos, MT_Float rPrecision ) const;
            bool IntersectWithBorder  ( const MT_Line& segment, MT_Float rPrecision     ) const;
            bool IntersectWithBorder  ( const MT_Line& segment, T_PointSet& res, MT_Float rPrecision ) const;
            bool IntersectWithBorder  ( const MT_Droite& line, T_PointSet& res ) const;
            bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius ) const; // NB : return true if the circle is inside
            bool Intersect2D          ( const MT_Line& segment, MT_Float rPrecision     ) const;
            bool Intersect2D          ( const MT_Line& segment, T_PointSet& res, MT_Float rPrecision ) const;
            bool Intersect2D          ( const MT_Droite& line , T_PointSet& res, MT_Float rPrecision ) const;
            void Intersection         (       MT_Polyline& polyline, MT_Float rPrecision ) const;

            void ComputeExtremities   ( const MT_Droite& line, MT_Vector2D& vLeftPoint, MT_Vector2D& vRightPoint ) const; // Retourne les deux du polygone sur la droite
            
            MT_Float GetArea          () const;

    const T_TrianglePtrVector& GetTriangles   () const; //$$$ A GICLER
    const T_PointVector&       GetBorderPoints() const; //$$$ A CLEANER
    
    MT_Rect GetBoundingBox() const;
    //@}
    
protected:
    // @name 
    //@{
    TER_Polygon( const TER_Polygon& poly );
    //@}
        
private:
    // @name 
    //@{
    TER_Polygon( const T_PointPtrVector& points , bool bConvexHull = false );
    TER_Polygon( const T_PointVector& points    , bool bConvexHull = false );
    //@}

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
    MT_Float      ComputeArea() const;
    E_BoundedSize BoundedSide( const MT_Vector2D& pos ) const;
    void          Convexify();
    bool          IsInBoundingBox( const MT_Vector2D& p, MT_Float rPrecision = 0 ) const;
    void          ComputeBoundingBox();
    //@}

private:
    
    TER_Polygon(bool bEmpty);
    void        Detach();

    struct PolygonData : public MT_Shared
    {
        PolygonData() : bIsNull_( false ), rArea_( -1 ) {};
        bool             bIsNull_;
        mutable MT_Float rArea_; // $$$$ AGE 2005-06-14: late initialization
        T_PointVector    borderVector_;
        MT_Rect          boundingBox_;
    } * pData_;

    static TER_Polygon*       empty_polygon;

};

#endif // __TER_Polygon_h_