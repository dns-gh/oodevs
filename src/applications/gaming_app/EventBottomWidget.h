// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventBottomWidget_h_
#define __EventBottomWidget_h_

#include "EventWidget_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace gui
{
    class RichDateTimeEdit;
}

namespace kernel
{
    class ActionController;
    class ContextMenu;
    class Time_ABC;
}

namespace tools
{
    class ExerciseConfig;
}

// =============================================================================
/** @class  EventBottomWidget
    @brief  EventBottomWidget
*/
// Created: ABR 2013-05-30
// =============================================================================
class EventBottomWidget : public EventWidget_ABC
                        , public tools::Observer_ABC
                        , public kernel::ContextMenuObserver_ABC< QDateTime >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventBottomWidget( const kernel::Time_ABC& simulation, kernel::ActionController& actionController, const tools::ExerciseConfig& config );
    virtual ~EventBottomWidget();
    //@}

    //! @name Operations
    //@{
    //@}

signals:
    //! @name Signals
    //@{
    void Trigger();
    void CreateReminder();
    void ShowDetail();
    void Cancel();
    void Discard();
    void Save();
    //@}

public slots:
    //! @name Slots
    //@{
    void SetBeginDateTime( const QDateTime& dateTime );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSwitchToggled( bool checked );
    void OnEditingChanged( bool editing, bool fromTimeline );

    void OnBeginDateTimeChanged( const QDateTime& dateTime );
    void OnHasEndTimeChanged( int state );
    void OnBeginDateTimeSelected();
    void OnEndDateTimeSelected();
    //@}

private:
    //! @name EventWidget_ABC implementation
    //@{
    virtual void Fill( const Event& event );
    virtual void Commit( timeline::Event& event ) const;
    //@}

    //! @name kernel::ContextMenuObserver_ABC< QDateTime >
    //@{
    virtual void NotifyContextMenu( const QDateTime& dateTime, kernel::ContextMenu& menu );
    //@}

    //! @name Types
    //@{
    enum E_EventModes
    {
        eEventModes_Execution = 0,
        eEventModes_Planning = 1
    };
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Time_ABC& simulation_;
    kernel::ActionController& actionController_;
    bool editing_;
    bool alreadyDone_;

    std::vector< QAction* > planningActions_;
    QAction* switchAction_;
    QAction* detailAction_;
    QToolButton* triggerButton_;
    QAction* saveAction_;

    QString triggerText_;
    QString saveText_;
    QString triggerAsCopyText_;
    QString saveAsCopyText_;

    QDateTime selectedDateTime_;
    gui::RichDateTimeEdit* beginDateTimeEdit_;
    QCheckBox* hasEndDateTimeCheckbox_;
    gui::RichDateTimeEdit* endDateTimeEdit_;
    //@}
};

#endif // __EventBottomWidget_h_
