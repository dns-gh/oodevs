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

namespace gui
{
    class EventPresenter;
    struct EventViewState;
    class RichGroupBox;
    class RichLabel;
}

// =============================================================================
/** @class  EventBottomWidget
    @brief  EventBottomWidget
*/
// Created: ABR 2013-05-30
// =============================================================================
class EventBottomWidget : public EventDefaultWidget_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventBottomWidget( gui::EventPresenter& presenter );
    virtual ~EventBottomWidget();
    //@}

private:
    //! @name EventWidget_ABC implementation
    //@{
    virtual void Purge();
    virtual void Build( const gui::EventViewState& state );
    virtual void BlockSignals( bool blocked );
    //@}

private:
    //! @name Member data
    //@{
    gui::RichGroupBox* misengageBox_;
    gui::RichLabel*    misengageLabel_;
    gui::RichGroupBox* warningBox_;
    gui::RichLabel*    warningLabel_;

    QAction* detailAction_;
    QAction* clearAction_;
    QAction* triggerAction_;
    QToolButton* triggerButton_;
    QToolBar* toolBar_;
    //@}
};

#endif // __EventBottomWidget_h_
