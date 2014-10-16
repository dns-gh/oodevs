// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GUI_GLVIEW_ABC_H__
#define __GUI_GLVIEW_ABC_H__

#include "GLToolColors.h"
#include "Layer_ABC.h"
#include "clients_kernel/Types.h"
#include <QtGui/qfont.h>
#include <boost/noncopyable.hpp>
#include <boost/optional/optional_fwd.hpp>
#include <boost/tuple/tuple.hpp>
#include <vector>

class QImage;
class QCursor;

namespace kernel
{
    class Location_ABC;
}

namespace gui
{
    class GlTooltip_ABC;

// =============================================================================
/** @class  GLView_ABC
    @brief  GLView_ABC definition
*/
// Created: AGE 2006-03-16
// =============================================================================
class GLView_ABC : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    enum E_Unit
    {
        pixels,
        meters
    };

    typedef std::pair< unsigned int, E_LayerTypes > T_ObjectPicking;
    typedef std::vector< T_ObjectPicking >          T_ObjectsPicking;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             GLView_ABC() {}
    virtual ~GLView_ABC() {}
    //@}

    //! @name Options
    //@{
    virtual void CenterOn( const geometry::Point2f& point ) = 0;
    virtual void Zoom( float width ) = 0;
    virtual geometry::Point2f GetCenter() const = 0;

    virtual boost::tuple< bool, bool, bool > UnSelect() const = 0;
    virtual void Select( bool, bool, bool ) const = 0;  //!< Returns the previous selection state
    virtual bool ShouldDisplay( const std::string& name = std::string() ) const = 0;
    virtual bool ShouldDisplay( const std::string& name, bool autoCondition ) const = 0;
    virtual bool ShouldDisplay( E_LayerTypes type ) const = 0;
    //@}

    //! @name Picking
    //@{
    virtual void FillSelection( const geometry::Point2f& point, T_ObjectsPicking& selection,
            const boost::optional< E_LayerTypes >& type ) = 0;
    virtual void Picking() = 0;
    virtual void RenderPicking( const T_ObjectPicking& object ) = 0;
    virtual bool IsPickingMode() const = 0;
    virtual QColor GetPickingColor() const = 0;
    virtual geometry::Point2f MapToterrainCoordinates( int x, int y ) = 0;
    virtual bool HasFocus() = 0;
    //@}

    //! @name Accessors
    //@{
    virtual std::string     GetCurrentPass() const = 0;
    virtual float           Pixels( const geometry::Point2f& at = geometry::Point2f() ) const = 0;
    virtual float           LineWidth( float base ) const = 0;
    virtual unsigned short  StipplePattern( int factor = 1 ) const = 0;
    virtual float           Zoom() const = 0;
    virtual float           GetAdaptiveZoomFactor( bool bVariableSize = true ) const = 0;
    virtual float           GetCurrentAlpha() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual std::unique_ptr< gui::GlTooltip_ABC > CreateTooltip() const = 0;

    virtual void SetCurrentColor  ( float r, float g, float b, float a = 1 ) = 0;
    virtual void SetCurrentCursor ( const QCursor& cursor ) = 0;

    virtual void DrawCross        ( const geometry::Point2f& at, float size = -1.f, E_Unit unit = meters ) const = 0;
    virtual void DrawLine         ( const geometry::Point2f& from, const geometry::Point2f& to, float width = 1. ) const = 0;
    virtual void DrawStippledLine ( const geometry::Point2f& from, const geometry::Point2f& to ) const = 0;
    virtual void DrawLines        ( const T_PointVector& points ) const = 0;
    virtual void DrawRectangle    ( const T_PointVector& points ) const = 0;
    virtual void DrawPolygon      ( const T_PointVector& points ) const = 0;
    virtual void DrawSelectedPolygon( const T_PointVector& points ) const = 0;
    virtual void DrawDecoratedPolygon( const kernel::T_TessellatedPolygon& polygon, const T_PointVector& vertices,
                                       const geometry::Point2f& center, const std::string& name, unsigned int fontHeight ) = 0;
    virtual void DrawArrow        ( const geometry::Point2f& from, const geometry::Point2f& to, float size = -1.f, E_Unit unit = meters ) const = 0;
    virtual void DrawCurvedArrow  ( const geometry::Point2f& from, const geometry::Point2f& to, float curveRatio = 0.2f, float size = -1.f, E_Unit unit = meters ) const = 0;
    virtual void DrawArc          ( const geometry::Point2f& center, const geometry::Point2f& from, const geometry::Point2f& to, float width = 1. ) const = 0;
    virtual void DrawCircle       ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const = 0;
    virtual void DrawDisc         ( const geometry::Point2f& center, float radius = -1.f, E_Unit unit = meters ) const = 0;
    virtual void DrawHalfDisc     ( const geometry::Point2f& center, float angleDegrees, float radius = -1.f, E_Unit unit = meters ) const = 0;
    virtual void DrawLife         ( const geometry::Point2f& center, float height, float factor = 1.f, bool fixedSize = true ) const = 0;
    virtual void Print            ( const std::string& message, const geometry::Point2f& where ) const = 0;
    virtual void Print            ( const std::string& message, const geometry::Point2f& where, const QFont& font ) const = 0;
    virtual void DrawInfrastructureSymbol( const std::string& symbol, const geometry::Point2f& where, float factor, float thickness ) const = 0;
    virtual void DrawApp6Symbol   ( const std::string& symbol, const geometry::Point2f& where, float factor = 1.f, float thickness = 1.f, unsigned int direction = 0 ) const = 0;
    virtual void DrawApp6Symbol   ( const std::string& symbol, const std::string& style, const geometry::Point2f& where, float factor = 1.f, float thickness = 1.f, unsigned int direction = 0 ) const = 0;
    virtual void DrawIcon         ( const char** xpm, const geometry::Point2f& where, float size = -1.f, E_Unit unit = meters ) const = 0;
    virtual void DrawImage        ( const QImage& image, const geometry::Point2f& where ) const = 0;
    virtual void DrawCell         ( const geometry::Point2f& center ) const = 0;
    virtual void DrawSvg          ( const std::string& svg, const geometry::Point2f& center, float ratio = 1.f, bool fixedSize = true ) const = 0;
    virtual void DrawTacticalGraphics( const std::string& symbol, const kernel::Location_ABC& location, bool overlined, float pointSize, bool fixedSize = true ) const = 0;
    virtual void DrawApp6SymbolFixedSize( const std::string& symbol, const geometry::Point2f& where, float factor, unsigned int direction ) const = 0;
    virtual void DrawUnitSymbol( const std::string& symbol, const std::string& moveSymbol, const std::string& staticSymbol, const std::string& level, bool isMoving, const geometry::Point2f& where, float factor, unsigned int direction, float width, float depth ) const = 0;
    virtual void DrawUnitSymbolAndTail( const std::string& symbol, const std::string& level, const T_PointVector& points ) const = 0;
    virtual void DrawShapeText( const QImage& image, const geometry::Point2f& where ) const = 0;
    //@}
};

} //! namespace gui

#endif // __GUI_GLVIEW_ABC_H__
