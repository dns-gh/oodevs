// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlTools_ABC_h_
#define __GlTools_ABC_h_

#include "GLToolColors.h"

namespace kernel
{
    class Controllers;

// =============================================================================
/** @class  GlTools_ABC
    @brief  GlTools definition
*/
// Created: AGE 2006-03-16
// =============================================================================
class GlTools_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GlTools_ABC() {};
    virtual ~GlTools_ABC() {};
    //@}

    //! @name Options  
    // $$$$ AGE 2006-05-19: Sortir de GlTools_ABC ?
    // $$$$ AGE 2006-05-19: et trouver mieux
    //@{
    virtual bool Select( bool ) const = 0;  //!< Returns the previous selection state
    virtual bool ShouldDisplay( const std::string& name ) const = 0;
    virtual bool ShouldDisplay( const std::string& name, bool autoCondition ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual float Pixels() const = 0;
    virtual unsigned short StipplePattern( int factor = 1 ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void DrawCross      ( const geometry::Point2f& at, float size = -1.f ) const = 0;
    virtual void DrawLine       ( const geometry::Point2f& from, const geometry::Point2f& to ) const = 0;
    virtual void DrawLines      ( const T_PointVector& points ) const = 0;
    virtual void DrawArrow      ( const geometry::Point2f& from, const geometry::Point2f& to, float size = -1.f ) const = 0;
    virtual void DrawCurvedArrow( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio = 0.2f, float size = -1.f ) const = 0;
    virtual void DrawArc        ( const geometry::Point2f& center, const geometry::Point2f& from, const geometry::Point2f& to ) const = 0;
    virtual void DrawCircle     ( const geometry::Point2f& center, float radius = -1.f ) const = 0;
    virtual void DrawDisc       ( const geometry::Point2f& center, float radius = -1.f ) const = 0;
    virtual void DrawLife       ( const geometry::Point2f& center, float height, float factor = 1.f ) const = 0;
    virtual void Print          ( const std::string& message, const geometry::Point2f& where ) const = 0;
    virtual void DrawRectangle  ( const geometry::Point2f& where, float factor = 1.f ) const = 0;
    virtual void DrawApp6Symbol ( const std::string& symbol, const geometry::Point2f& where, float factor = 1.f ) const = 0;
    virtual void DrawIcon       ( const char** xpm, const geometry::Point2f& where, float size = -1.f ) const = 0;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GlTools_ABC( const GlTools_ABC& );
    GlTools_ABC& operator=( const GlTools_ABC& );
    //@}

};

}

#endif // __GlTools_ABC_h_
