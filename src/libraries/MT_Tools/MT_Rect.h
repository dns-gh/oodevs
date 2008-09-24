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

#include "MT_Tools_Types.h"

#include "MT_Line.h"
#include "MT_Triangle.h"

//*****************************************************************************
// Created: FBD 02-11-07
// Last modified: JVT 03-09-02
//*****************************************************************************
class MT_Rect
{
//    MT_BOOSTPOOLING_MEMBERS( MT_Rect )

public:
     MT_Rect( MT_Float rPosX1 = 0., MT_Float rPosY1 = 0., MT_Float rPosX2 = 0., MT_Float rPosY2 = 0. );
     MT_Rect( const MT_Rect& rhs );
    ~MT_Rect();

    void Set( const MT_Vector2D&, const MT_Vector2D& );
    void Reset();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MT_Float           GetTop   () const;
    MT_Float           GetBottom() const;
    MT_Float           GetLeft  () const;
    MT_Float           GetRight () const;
                        
    MT_Float           GetWidth () const;
    MT_Float           GetHeight() const;

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
    bool IsInside   ( const MT_Vector2D& point, MT_Float rPrecision ) const;
    bool Intersect2D( const MT_Line& line ) const;
    bool Intersect2D( const MT_Rect& rect ) const;
    bool Intersect2D( const MT_Triangle& triangle ) const;
    bool Intersect2D( const MT_Vector2D& vPosSrc, const MT_Vector2D& vPosDest ) const;
    bool Intersect2D( const MT_Line& orientedLine, T_PointSet& collisionSet, MT_Float rPrecision ) const;

    bool Intersect2DWithCircle( const MT_Vector2D& vCircleCenter, MT_Float rRadius ) const; // NB : return true if the circle is inside the rect
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
    MT_Float rPosX1_;
    MT_Float rPosY1_;
    MT_Float rPosX2_;
    MT_Float rPosY2_;

    MT_Float rHeight_;
    MT_Float rWidth_;

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

#include "MT_Rect.inl"

#endif // __MT_Rect_h_