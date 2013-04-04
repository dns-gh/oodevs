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

namespace kernel
{
    class Options;
}

namespace gui
{
    class ColorStrategy;
    class DrawingTypes;
    class GlTools_ABC;
    class SelectionMenu;
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
             CircularEventStrategy( kernel::Options& options, EntitySymbols& entitySymbols, ColorStrategy& colorStrategy,
                                    DrawingTypes& drawingTypes, GlTools_ABC& tools );
    virtual ~CircularEventStrategy();
    //@}

    //! @name Settings
    //@{
    void SetExclusive( bool );
    const SelectionMenu* GetSelectionMenu() const;
    //@}

    //! @name Operations
    //@{
    void SetDefault( Layer_ABC& layer );
    void Register  ( Layer_ABC& layer );
    void Remove    ( Layer_ABC& layer );

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
    template< typename It, typename Functor >
    bool Loop( It& use, It first, It begin, It end, Functor functor );

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
    Layer_ABC::T_Layers layers_;
    Layer_ABC* default_;
    bool exclusive_;
    Layer_ABC::T_Layers::const_reverse_iterator rlast_;
    std::auto_ptr< SelectionMenu > menu_;
    GlTools_ABC& tools_;
    QTimer* timer_;
    geometry::Point2f point_;
    bool tooltiped_;
    //@}
};

}

#endif // __CircularEventStrategy_h_
