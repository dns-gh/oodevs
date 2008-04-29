// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelineView_h_
#define __TimelineView_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include <qcanvas.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

class ActionsModel;
class Action_ABC;
class ActionsScheduler;
class Simulation;
class TimelineItem_ABC;
class TimelineRuler;
class TimelineActionItem;
class TimelineMarker;

// =============================================================================
/** @class  TimelineView
    @brief  TimelineView
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelineView : public QCanvasView
                   , public kernel::Observer_ABC
                   , public kernel::ElementObserver_ABC< Action_ABC >
                   , public kernel::ElementObserver_ABC< kernel::Entity_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             TimelineView( QWidget* parent, QCanvas* canvas, kernel::Controllers& controllers, ActionsModel& model, ActionsScheduler& scheduler, TimelineRuler& ruler );
    virtual ~TimelineView();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineView( const TimelineView& );            //!< Copy constructor
    TimelineView& operator=( const TimelineView& ); //!< Assignment operator
    //@}

    //! @name Events
    //@{
    virtual void contentsMousePressEvent( QMouseEvent* event );
    virtual void contentsMouseReleaseEvent( QMouseEvent* event );
    virtual void contentsMouseMoveEvent( QMouseEvent* event );
    virtual void contentsContextMenuEvent( QContextMenuEvent* event );
    virtual void keyPressEvent( QKeyEvent* event );
    virtual void setContentsPos( int x, int y );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Action_ABC& action );
    virtual void NotifyDeleted( const Action_ABC& action );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );

    void Update();
    void ClearSelection();
    void SetSelected( TimelineItem_ABC& item );
    void Select( const QPoint& point );
    //@}

    //! @name Types
    //@{
    typedef std::map< const Action_ABC*, TimelineActionItem* > T_Actions;
    typedef std::map< const kernel::Entity_ABC*, T_Actions >   T_EntityActions;
    typedef std::vector< const kernel::Entity_ABC* >           T_Entities;
    //@}

private:
    //! @name Statics
    //@{
    static const unsigned int rowHeight_;
    //@}

    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ActionsModel&        model_;
    T_EntityActions      actions_;
    T_Entities           entities_;
    TimelineRuler&       ruler_;
    TimelineMarker*      marker_;
    TimelineItem_ABC*    selectedItem_;
    QPoint               grabPoint_;
    QPalette             actionPalette_;
    //@}
};

#endif // __TimelineView_h_
