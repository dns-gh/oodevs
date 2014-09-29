// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParametersLayer_h_
#define __ParametersLayer_h_

#include "Layer.h"

namespace kernel
{
    class Positions;
    class Location_ABC;
}

namespace gui
{
    class CursorStrategy_ABC;
    class GlTools_ABC;
    class LocationEditor_ABC;
    class ShapeHandler_ABC;
    class TextEditor;
}

namespace gui
{
// =============================================================================
/** @class  ParametersLayer
    @brief  ParametersLayer
*/
// Created: AGE 2006-03-23
// =============================================================================
class ParametersLayer : public Layer
{
public:
    //! @name Constructors/Destructor
    //@{
             ParametersLayer( kernel::Controllers& controllers, GlTools_ABC& tools, TextEditor& textEditor );
    virtual ~ParametersLayer();
    //@}

    //! @name Operations
    //@{
    void SetPoint       ( ShapeHandler_ABC& handler, const geometry::Point2f& point ); // set current location to the input point, without edition
    void StartPoint     ( ShapeHandler_ABC& handler ); // start point edition
    void StartLine      ( ShapeHandler_ABC& handler );
    void StartRectangle ( ShapeHandler_ABC& handler );
    void StartPolygon   ( ShapeHandler_ABC& handler );
    void StartCircle    ( ShapeHandler_ABC& handler );
    void StartCurve     ( ShapeHandler_ABC& handler );
    void StartText      ( ShapeHandler_ABC& handler, const QColor& color );
    void StartPath      ( ShapeHandler_ABC& handler, const kernel::Positions& position );
    void Start          ( ShapeHandler_ABC& handler, const kernel::Location_ABC& location );
    void Start          ( ShapeHandler_ABC& handler, const kernel::Location_ABC& location, const QColor& color );
    void SelectRaster   ( ShapeHandler_ABC& handler );
    void Reset          ();

    void AddPoint( const geometry::Point2f& point );

    bool IsInsideWorld( const geometry::Point2f& point ) const;

    virtual void Initialize( const geometry::Rectangle2f& extent );
    virtual void Paint( Viewport_ABC& viewport );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool HandleKeyPress        ( QKeyEvent* key );
    virtual bool HandleMouseMove       ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMousePress      ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point );

    void NotifyDone();
    //@}

private:
    //! @name Member data
    //@{
    GlTools_ABC& tools_;
    TextEditor& textEditor_;
    std::unique_ptr< CursorStrategy_ABC > cursors_;
    ShapeHandler_ABC* handler_;
    kernel::Location_ABC* current_;
    geometry::Rectangle2f world_;
    geometry::Rectangle2f viewport_;
    geometry::Point2f lastPoint_;
    bool validation_;
    //@}
};

}

#endif // __ParametersLayer_h_
