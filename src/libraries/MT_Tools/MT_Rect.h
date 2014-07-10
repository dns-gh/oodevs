//*****************************************************************************
//
// $Created: FBD 02-11-07 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Rect.h $
// $Author: Nld $
// $Modtime: 1/09/04 16:44 $
// $Revision: 17 $
// $Workfile: MT_Rect.h $
//
//*****************************************************************************

#ifndef __MT_Rect_h_
#define __MT_Rect_h_

#include "MT_Line.h"

class MT_Triangle;

//*****************************************************************************
// Created: FBD 02-11-07
// Last modified: JVT 03-09-02
//*****************************************************************************
class MT_Rect
{
public:
             MT_Rect( double rPosX1 = 0., double rPosY1 = 0., double rPosX2 = 0., double rPosY2 = 0. );
             MT_Rect( const MT_Rect& rhs );
    virtual ~MT_Rect();

    void Set( const MT_Vector2D&, const MT_Vector2D& );
    void Reset();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    double GetTop   () const;
    double GetBottom() const;
    double GetLeft  () const;
    double GetRight () const;
    double GetWidth () const;
    double GetHeight() const;

    const MT_Vector2D& GetPointUpLeft   () const;
    const MT_Vector2D& GetPointUpRight  () const;
    const MT_Vector2D& GetPointDownRight() const;
    const MT_Vector2D& GetPointDownLeft () const;

    const MT_Line& GetLineLeft   () const;
    const MT_Line& GetLineRight  () const;
    const MT_Line& GetLineUp     () const;
    const MT_Line& GetLineDown   () const;

    const MT_Vector2D&  GetCenter() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Intersection */
    //-------------------------------------------------------------------------
    //@{
    bool IsInside   ( const MT_Vector2D& point ) const;
    bool IsInside   ( const MT_Vector2D& point, double rPrecision ) const;
    bool Contains   ( const MT_Rect& rect ) const;
    bool Intersect2D( const MT_Line& line ) const;
    bool Intersect2D( const MT_Rect& rect ) const;
    bool Intersect2D( const MT_Triangle& triangle ) const;
    bool Intersect2D( const MT_Vector2D& vPosSrc, const MT_Vector2D& vPosDest ) const;
    bool Intersect2D( const MT_Line& orientedLine, T_PointSet& collisionSet, double rPrecision ) const;

    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, double rRadius ) const; // NB : return true if the circle is inside the rect
    //@}

    //-------------------------------------------------------------------------
    /** @name Tools */
    //-------------------------------------------------------------------------
    //@{
    void ClipPoint( MT_Vector2D& vPosToClip ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Operators */
    //-------------------------------------------------------------------------
    //@{
    MT_Rect& operator=( const MT_Rect& rhs );
    //@}

private:
    double rPosX1_;
    double rPosY1_;
    double rPosX2_;
    double rPosY2_;

    double rHeight_;
    double rWidth_;

    MT_Vector2D pointUpLeft_;
    MT_Vector2D pointUpRight_;
    MT_Vector2D pointDownRight_;
    MT_Vector2D pointDownLeft_;

    MT_Line lineUp_;
    MT_Line lineRight_;
    MT_Line lineDown_;
    MT_Line lineLeft_;

    MT_Vector2D vCenter_;
};

#endif // __MT_Rect_h_
