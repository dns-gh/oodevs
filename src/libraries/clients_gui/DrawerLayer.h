// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __gui_DrawerLayer_h_
#define __gui_DrawerLayer_h_

#include "EntityLayer.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/Drawing_ABC.h"

namespace kernel
{
    class Controllers;
    class Drawing_ABC;
}

namespace gui
{
    class GlTools_ABC;
    class ParametersLayer;
}

namespace gui
{
    class ModelObserver_ABC;
// =============================================================================
/** @class  DrawerLayer
    @brief  DrawerLayer
*/
// Created: AGE 2006-08-31
// =============================================================================
class DrawerLayer : public EntityLayer< kernel::Drawing_ABC >
                  , public kernel::ContextMenuObserver_ABC< kernel::Drawing_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DrawerLayer( kernel::Controllers& controllers, GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                          ParametersLayer& parameters, View_ABC& view, const kernel::Profile_ABC& profile, ModelObserver_ABC& model );
    virtual ~DrawerLayer();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnEditDrawing();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Drawing_ABC& shape, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );
    virtual void Draw( const kernel::Entity_ABC& entity, Viewport_ABC& viewport, bool pickingMode );
    virtual void NotifySelectionChanged( const std::vector< const kernel::Drawing_ABC* >& elements );
    virtual void ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& );
    virtual bool HandleKeyPress( QKeyEvent* key );
    virtual void NotifyDeleted( const kernel::Drawing_ABC& drawing );
    //@}

private:
    //! @name Member data
    //@{
    ParametersLayer& parameters_;
    GlTools_ABC& tools_;
    ModelObserver_ABC& model_;
    geometry::Rectangle2f viewport_;
    const kernel::Drawing_ABC* selected_;
    //@}
};

}

#endif // __gui_DrawerLayer_h_
