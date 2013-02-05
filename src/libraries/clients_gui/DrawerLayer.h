// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerLayer_h_
#define __DrawerLayer_h_

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
             DrawerLayer( kernel::Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy,
                          ParametersLayer& parameters, View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~DrawerLayer();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnEditDrawing();
    void OnDeleteDrawing();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );
    virtual void Draw( const kernel::Entity_ABC& entity, Viewport_ABC& viewport );
    virtual void NotifySelectionChanged( const std::vector< const kernel::Drawing_ABC* >& elements );
    virtual void NotifyContextMenu( const kernel::Drawing_ABC& shape, kernel::ContextMenu& menu );
    virtual bool HandleKeyPress( QKeyEvent* key );
    //@}

private:
    //! @name Member data
    //@{
    ParametersLayer& parameters_;
    const GlTools_ABC& tools_;
    geometry::Rectangle2f viewport_;
    const kernel::Drawing_ABC* selected_;
    //@}
};

}

#endif // __DrawerLayer_h_
