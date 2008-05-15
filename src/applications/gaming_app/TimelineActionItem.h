// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelineActionItem_h_
#define __TimelineActionItem_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "TimelineItem_ABC.h"

namespace kernel
{
    class Controllers;
}

class Action_ABC;
class ActionsModel;
class ActionTiming;
class TimelineRuler;

// =============================================================================
/** @class  TimelineActionItem
    @brief  TimelineActionItem
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelineActionItem : public QObject
                         , public TimelineItem_ABC
                         , public kernel::Observer_ABC
                         , public kernel::ElementObserver_ABC< ActionTiming >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TimelineActionItem( QCanvas* canvas, const TimelineRuler& ruler, kernel::Controllers& controllers, ActionsModel& model, const Action_ABC& action, const QPalette& palette );
    virtual ~TimelineActionItem();
    //@}

    //! @name Operations
    //@{
    virtual void Update();
    virtual void Move( long offset );
    virtual void Delete();
    virtual void DisplayToolTip( QWidget* parent ) const;
    virtual void DisplayContextMenu( QPopupMenu* menu ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnDelete();
    void OnRename();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineActionItem( const TimelineActionItem& );            //!< Copy constructor
    TimelineActionItem& operator=( const TimelineActionItem& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void setVisible( bool visible );
    virtual void draw( QPainter& painter );
    virtual void NotifyUpdated( const ActionTiming& timing );
    void ComputeTextWidth( QPainter& painter );
    void SetTime( const QDateTime& datetime );
    //@}

private:
    //! @name Member data
    //@{
    const TimelineRuler& ruler_;
    kernel::Controllers& controllers_;
    ActionsModel& model_;
    const Action_ABC& action_;
    unsigned int textWidth_;
    const QPalette& palette_;
    //@}
};

#endif // __TimelineActionItem_h_
