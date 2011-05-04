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
#include "Types.h"
#include <qfont.h>

class QImage;
class QCursor;

namespace kernel
{
    class Controllers;
    class GlTooltip_ABC;
    class Location_ABC;
    class UrbanColor_ABC;

// =============================================================================
/** @class  GlTools_ABC
    @brief  GlTools definition
*/
// Created: AGE 2006-03-16
// =============================================================================
class GlTools_ABC
{
public:
    //! @name Types
    //@{
    enum E_Unit
    {
        pixels,
        meters
    };
    //@}

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
    virtual std::pair< bool, bool > UnSelect() const = 0;
    virtual void Select( bool, bool ) const = 0;  //!< Returns the previous selection state
    virtual bool ShouldDisplay( const std::string& name = std::string() ) const = 0;
    virtual bool ShouldDisplay( const std::string& name, bool autoCondition ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual float           Pixels( const geometry::Point2f& at = geometry::Point2f() ) const = 0;
    virtual unsigned short  StipplePattern( int factor = 1 ) const = 0;
    virtual float           Zoom() const = 0;
    virtual float           GetAdaptiveZoomFactor() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< GlTooltip_ABC > CreateTooltip() const = 0;

    virtual void SetCurrentColor  ( float r, float g, float b, float a = 1 ) = 0;
    virtual void SetCurrentCursor ( const QCursor& cursor ) = 0;

    virtual void DrawCross        ( const geometry::Point2f& at, float size = -1.f, E_Unit unit = meters ) const = 0;
    virtual void DrawLine         ( const geometry::Point2f& from, const geometry::Point2f& to ) const = 0;
    virtual void DrawLines        ( const T_PointVector& points ) const = 0;
    virtual void DrawRectangle    ( const T_PointVector& points ) const = 0;
    virtual void DrawPolygon      ( const T_PointVector& points ) const = 0;
    virtual void DrawDecoratedPolygon( const geometry::Polygon2f& polygon, const kernel::UrbanColor_ABC& urbanColor, const std::string& name, unsigned int height, bool selected ) const = 0;
    virtual void DrawArrow        ( const geometry::Point2f& from, const geometry::Point2f& to, float size = -1.f, E_Unit unit = meters ) const = 0;
    virtual void DrawCurvedArrow  ( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio = 0.2f, float size = -1.f, E_Unit unit = meters ) const = 0;
    virtual void DrawArc          ( const geometry::Point2f& center, const geometry::Point2f& from, const geometry::Point2f& to ) const = 0;
    virtual void DrawCircle       ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const = 0;
    virtual void DrawDisc         ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const = 0;
    virtual void DrawLife         ( const geometry::Point2f& center, float height, float factor = 1.f ) const = 0;
    virtual void Print            ( const std::string& message, const geometry::Point2f& where ) const = 0;
    virtual void Print            ( const std::string& message, const geometry::Point2f& where, const QFont& font ) const = 0;
    virtual void DrawApp6Symbol   ( const std::string& symbol, const geometry::Point2f& where, float factor = 1.f, float thickness = 1.f ) const = 0;
    virtual void DrawApp6Symbol   ( const std::string& symbol, const std::string& style, const geometry::Point2f& where, float factor = 1.f, float thickness = 1.f ) const = 0;
    virtual void DrawIcon         ( const char** xpm, const geometry::Point2f& where, float size = -1.f, E_Unit unit = meters ) const = 0;
    virtual void DrawImage        ( const QImage& image, const geometry::Point2f& where ) const = 0;
    virtual void DrawCell         ( const geometry::Point2f& center ) const = 0;
    virtual void DrawSvg          ( const std::string& svg, const geometry::Point2f& center, float ratio = 1.f ) const = 0;
    virtual void DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location, bool overlined ) const = 0;
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
