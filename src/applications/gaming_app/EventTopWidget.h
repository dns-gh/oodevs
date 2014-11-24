// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventTopWidget_h_
#define __EventTopWidget_h_

#include "EventWidget_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace gui
{
    class RichDateTimeEdit;
    class RichLabel;
}

namespace kernel
{
    class ActionController;
    class ContextMenu;
    class Time_ABC;
}

// =============================================================================
/** @class  EventTopWidget
    @brief  EventTopWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventTopWidget : public EventDefaultWidget_ABC
                     , public tools::Observer_ABC
                     , public kernel::ContextMenuObserver_ABC< QDateTime >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventTopWidget( gui::EventPresenter& presenter,
                             const kernel::Time_ABC& simulation,
                             kernel::ActionController& actionController );
    virtual ~EventTopWidget();
    //@}

private:
    //! @name EventDefaultWidget_ABC implementation
    //@{
    virtual void Purge();
    virtual void Build( const gui::EventViewState& state );
    virtual void BlockSignals( bool blocked );
    //@}

    //! @name kernel::ContextMenuObserver_ABC< QDateTime > implementation
    //@{
    virtual void NotifyContextMenu( const QDateTime& dateTime, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnBeginDateTimeSelected();
    void OnEndDateTimeSelected();
    void OnEndDateActivated( int );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Time_ABC& simulation_;
    kernel::ActionController& actionController_;

    QLabel* title_;
    QLabel* startDateLabel_;
    QAction* saveAction_;
    QAction* saveAsAction_;
    gui::RichDateTimeEdit* beginDateTimeEdit_;
    gui::RichDateTimeEdit* endDateTimeEdit_;
    QCheckBox* hasEndDateTimeCheckbox_;
    gui::RichLabel* endDateTimeLabel_;

    QDateTime selectedDateTime_;
    //@}
};

#endif // __EventTopWidget_h_
