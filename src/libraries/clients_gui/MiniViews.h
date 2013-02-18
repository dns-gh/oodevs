// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MiniViews_h_
#define __MiniViews_h_

#include "clients_gui/RichDockWidget.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Profile_ABC;
}

namespace gui
{
    class SmartGridWidget;
    class GlWidget;
    class MiniView;
    class GlRenderPass_ABC;

// =============================================================================
/** @class  MiniViews
    @brief  MiniViews
*/
// Created: AGE 2006-06-23
// =============================================================================
class MiniViews : public RichDockWidget
                , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
                , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             MiniViews( QMainWindow* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile );
    virtual ~MiniViews();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnMiniView();
    void OnWidget2dChanged( gui::GlWidget* );
    //@}

private:
    //! @name Helpers
    //@{
    void BuildContextMenu( const kernel::Entity_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    void DeleteView( MiniView*& view, const kernel::Entity_ABC* entity );
    //@}

    //! @name Types
    //@{
    typedef std::map< const kernel::Entity_ABC*, MiniView* > T_MiniViews;
    typedef std::map< MiniView*, GlRenderPass_ABC* >         T_RenderPasses;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Profile_ABC& profile_;
    GlWidget* widget_;
    SmartGridWidget* grid_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    T_MiniViews miniViews_;
    T_RenderPasses renderPasses_;
    //@}
};

}

#endif // __MiniViews_h_
