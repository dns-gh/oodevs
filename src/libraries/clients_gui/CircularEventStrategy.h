// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CircularEventStrategy_h_
#define __CircularEventStrategy_h_

#include <graphics/EventStrategy_ABC.h>
#include "Layer_ABC.h"
#include "LayersHelpers.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ColorStrategy;
    class DrawingTypes;
    class GLMainProxy;
    class GLView_ABC;
    class SelectionMenu;
    class Selection;
    class EntitySymbols;

// =============================================================================
/** @class  CircularEventStrategy
    @brief  CircularEventStrategy
*/
// Created: AGE 2006-08-21
// =============================================================================
class CircularEventStrategy : public QObject
                            , public EventStrategy_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit CircularEventStrategy( kernel::Controllers& controllers );
    virtual ~CircularEventStrategy();
    //@}

    //! @name Operations
    //@{
    void Initialize( kernel::Controllers& controllers,
                     EntitySymbols& entitySymbols,
                     ColorStrategy& colorStrategy,
                     DrawingTypes& drawingTypes,
                     const std::shared_ptr< GLMainProxy >& proxy,
                     const T_Layer& defaultLayer,
                     const T_LayersVector& layers );
    void SetExclusive( bool );
    //@}

    //! @name EventStrategy_ABC
    //@{
    virtual void HandleKeyPress        ( QKeyEvent* key );
    virtual void HandleKeyRelease      ( QKeyEvent* key );
    virtual void HandleMousePress      ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void HandleMouseRelease    ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void HandleMouseMove       ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual void HandleMouseWheel      ( QWheelEvent* mouse,     const geometry::Point2f& point );
    virtual void HandleDropEvent       ( QDropEvent*  event,     const geometry::Point2f& point );
    virtual void HandleEnterDragEvent  ( QDragEnterEvent* event, const geometry::Point2f& point );
    virtual void HandleMoveDragEvent   ( QDragMoveEvent*  event, const geometry::Point2f& point );
    virtual void HandleLeaveDragEvent  ( QDragLeaveEvent*  event                                );
    //@}

private:
    //! @name Helpers
    //@{
    template< typename Functor >
    bool Apply( Functor functor );

    void HideTooltip();

    void RetrieveEntities( QMouseEvent* mouse, const geometry::Point2f& point, Layer_ABC::T_LayerElements& extractedElements );
    bool DisplayDefaultMenu( QMouseEvent* mouse, const geometry::Point2f& point, Layer_ABC::T_LayerElements& extractedElements );
    bool DisplaySelectedMenu( QMouseEvent* mouse, const geometry::Point2f& point, Layer_ABC::T_LayerElements& extractedElements );
    //@}

private slots:
    //! @name slots
    //@{
    void OnDisplayToolTip();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    std::unique_ptr< Selection > selection_;
    std::unique_ptr< QTimer > timer_;
    T_LayersVector layers_;
    T_Layer default_;
    std::unique_ptr< SelectionMenu > menu_;
    std::shared_ptr< GLView_ABC > view_;
    bool exclusive_;
    bool tooltiped_;
    //@}
};

} //! namespace gui

#endif // __CircularEventStrategy_h_
