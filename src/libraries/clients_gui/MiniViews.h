// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MiniViews_h_
#define __MiniViews_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Entity_ABC;
    class Agent_ABC;
    class Population_ABC;
    class Controllers;
    class Automat_ABC;
}

namespace gui
{
    class SmartGridWidget;
    class GlWidget;
    class MiniView;

// =============================================================================
/** @class  MiniViews
    @brief  MiniViews
*/
// Created: AGE 2006-06-23
// =============================================================================
class MiniViews : public QDockWindow
                , public kernel::Observer_ABC
                , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MiniViews( QMainWindow* parent, kernel::Controllers& controllers, GlWidget* const& widget );
    virtual ~MiniViews();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnMiniView();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MiniViews( const MiniViews& );            //!< Copy constructor
    MiniViews& operator=( const MiniViews& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void BuildContextMenu( const kernel::Entity_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    //@}

    //! @name Types
    //@{
    typedef std::map< const kernel::Entity_ABC*, MiniView* > T_MiniViews;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    GlWidget* const & widget_;
    SmartGridWidget* grid_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    T_MiniViews miniViews_;
    //@}
};

}

#endif // __MiniViews_h_
