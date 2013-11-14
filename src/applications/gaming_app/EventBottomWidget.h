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
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventBottomWidget( const tools::ExerciseConfig& config );
    virtual ~EventBottomWidget();
    //@}

signals:
    //! @name Signals
    //@{
    void Discard();
    void ShowDetail();
    void Trigger();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnEnableTriggerEvent( bool enable );
    //@}

private:
    //! @name EventWidget_ABC implementation
    //@{
    virtual void Fill( const Event& event );
    virtual void Commit( timeline::Event& event ) const;
    //@}

private:
    //! @name Member data
    //@{
    QAction* detailAction_;
    QToolButton* triggerButton_;
    //@}
};

#endif // __EventBottomWidget_h_
