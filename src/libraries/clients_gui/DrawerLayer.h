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
    class ParametersLayer;
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
             DrawerLayer( kernel::Controllers& controllers,
                          GLView_ABC& view,
                          ColorStrategy_ABC& strategy,
                          const std::shared_ptr< ParametersLayer >& parameters,
                          const kernel::Profile_ABC& profile,
                          ModelObserver_ABC& model );
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
    virtual void ContextMenu( const kernel::GraphicalEntity_ABC&, const geometry::Point2f&, const QPoint& );
    virtual bool HandleKeyPress( QKeyEvent* key );
    //@}

private:
    //! @name Member data
    //@{
    std::shared_ptr< ParametersLayer > parameters_;
    ModelObserver_ABC& model_;
    geometry::Rectangle2f viewport_;
    //@}
};

}

#endif // __gui_DrawerLayer_h_
