// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Author: Jvt $
// $Modtime: 12/04/05 9:57 $
// $Revision: 2 $
// $Workfile: Sector.h $
//
// *****************************************************************************

#ifndef __Sector_h_
#define __Sector_h_

namespace gui
{
    class GLView_ABC;
}

// =============================================================================
// @class  Sector
// Created: JVT 2004-08-03
// =============================================================================
class Sector
{
public:
             Sector( const geometry::Point2f& origin, unsigned heading, float rAngle );
             Sector();
    virtual ~Sector();

    //! @name Operations
    //@{
    bool IsInCone  ( const geometry::Point2f& vPos, float rRadius ) const;
    bool IsInSector( const geometry::Point2f& vPos ) const;
    void Draw( const gui::GLView_ABC& tools, float radius ) const;
    void DrawFill( float radius ) const;
    //@}

    //! @name Accessors
    //@{
    const geometry::Vector2f& GetDirection() const;
    const geometry::Point2f&  GetOrigin   () const;
          float               GetAngle    () const;
    //@}

private:
    static const float pi_;
    geometry::Point2f vOrigin_;
    geometry::Vector2f vDirection_;
    float    rSemiAngle_;

    // Equation de la droite séparant le premier demi-plan
    float rA1_;
    float rB1_;
    float rC1_;

    // Equation de la droite séparant de deuxième demi-plan
    float rA2_;
    float rB2_;
    float rC2_;

    // draw
    float minAngle_;
    float maxAngle_;
    geometry::Vector2f direction1_;
    geometry::Vector2f direction2_;
};

#include "Sector.inl"

#endif // __Sector_h_
