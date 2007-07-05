// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelineEditor_h_
#define __TimelineEditor_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include <qcanvas.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

class ActionsModel;
class Action_ABC;
class TimelineEntityItem;

// =============================================================================
/** @class  TimelineEditor
    @brief  TimelineEditor
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelineEditor : public QCanvasView
                     , public kernel::Observer_ABC
                     , public kernel::ElementObserver_ABC< Action_ABC >
                     , public kernel::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TimelineEditor( QWidget* parent, QCanvas* canvas, kernel::Controllers& controllers, ActionsModel& actions );
    virtual ~TimelineEditor();
    //@}

private slots:
    //! @name Slots
    //@{
    void Update();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineEditor( const TimelineEditor& );            //!< Copy constructor
    TimelineEditor& operator=( const TimelineEditor& ); //!< Assignment operator
    //@}

    //! @name Events
    //@{
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );
    virtual void keyPressEvent( QKeyEvent* event );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Action_ABC& action );
    virtual void NotifyDeleted( const Action_ABC& action );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );

    void ClearSelection();
    void SetSelected( QCanvasItem& item );
    QPoint ConvertToContent( const QPoint& point ) const;
    //@}

    //! @name Types
    //@{
    typedef std::map< const kernel::Entity_ABC*, TimelineEntityItem* > T_EntityItems;
    typedef T_EntityItems::const_iterator                            CIT_EntityItems;
    typedef std::vector< TimelineEntityItem* >                         T_Lines;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ActionsModel&        actions_;
    T_EntityItems        items_;
    T_Lines              lines_;
    QTimer*              updateTimer_;
    QCanvasItem*         selectedItem_;
    QPoint               grabPoint_;
    //@}
};

#endif // __TimelineEditor_h_
