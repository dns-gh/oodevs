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

#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace actions
{
    class ActionsFilter_ABC;
    class ActionsModel;
    class Action_ABC;
    enum EActionType;
}

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
class TimelineView : public Q3CanvasView
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< actions::Action_ABC >
                   , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                   , public tools::SelectionObserver< actions::Action_ABC >
                   , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TimelineView( QWidget* parent, Q3Canvas* canvas, kernel::Controllers& controllers, actions::ActionsModel& model, ActionsScheduler& scheduler, TimelineRuler& ruler );
    virtual ~TimelineView();
    //@}

    //! @name Operations
    //@{
    void SetFilter( const actions::ActionsFilter_ABC& filter );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const actions::Action_ABC*, TimelineActionItem* > T_Actions;
    typedef std::map< const kernel::Entity_ABC*, T_Actions >            T_EntityActions;
    typedef std::vector< const kernel::Entity_ABC* >                    T_OrderedEntities;
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
    virtual void NotifyCreated( const actions::Action_ABC& action );
    virtual void NotifyDeleted( const actions::Action_ABC& action );
    virtual void NotifySelected( const actions::Action_ABC* action );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );

    void Update();
    void DrawActions( T_Actions& actions, int& row );
    void ClearSelection();
    T_Actions* FindActions( const actions::Action_ABC& action, actions::EActionType& actionType );
    void SetSelected( TimelineItem_ABC& item );
    void Select( const QPoint& point );
    //@}

private:
    //! @name Statics
    //@{
    static const unsigned int rowHeight_;
    //@}

    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& model_;
    T_EntityActions entityActions_;
    T_Actions magicActions_;
    T_Actions weatherActions_;
    T_Actions objectsActions_;
    T_OrderedEntities orderedActions_;
    bool magicVisible_;
    bool weatherVisible_;
    bool objectsVisible_;
    TimelineRuler& ruler_;
    TimelineMarker* marker_;
    TimelineItem_ABC* selectedItem_;
    QPoint grabPoint_;
    const actions::ActionsFilter_ABC* filter_;
    //@}
};

#endif // __TimelineView_h_
