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

#include "TimelineItem_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace actions
{
    class Action_ABC;
    class ActionsModel;
    class ActionTiming;
}

class TimelineRuler;

// =============================================================================
/** @class  TimelineActionItem
    @brief  TimelineActionItem
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelineActionItem : public QObject
                         , public TimelineItem_ABC
                         , public tools::Observer_ABC
                         , public tools::ElementObserver_ABC< actions::ActionTiming >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TimelineActionItem( Q3Canvas* canvas, const TimelineRuler& ruler, kernel::Controllers& controllers, actions::ActionsModel& model, const actions::Action_ABC& action );
    virtual ~TimelineActionItem();
    //@}

    //! @name Operations
    //@{
    virtual void Update();
    virtual void Move( long offset );
    virtual void Delete();
    virtual void DisplayToolTip( QWidget* parent ) const;
    virtual void DisplayContextMenu( Q3PopupMenu* menu ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnDelete();
    void OnRename();
    void DoRename();
    void OnToggleEnabled();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineActionItem( const TimelineActionItem& );            //!< Copy constructor
    TimelineActionItem& operator=( const TimelineActionItem& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void setEnabled( bool enabled );
    virtual void setSelected( bool selected );
    virtual void draw( QPainter& painter );
    virtual void NotifyUpdated( const actions::ActionTiming& timing );
    void ComputeTextWidth( QPainter& painter );
    void SetTime( const QDateTime& datetime );
    //@}

private:
    //! @name Member data
    //@{
    const TimelineRuler& ruler_;
    kernel::Controllers& controllers_;
    actions::ActionsModel& model_;
    const actions::Action_ABC& action_;
    unsigned int textWidth_;
    mutable QLineEdit* nameEditor_;
    //@}
};

#endif // __TimelineActionItem_h_
