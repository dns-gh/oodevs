// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventTaskWidget_h_
#define __EventTaskWidget_h_

#include "EventWidget_ABC.h"

namespace gui
{
    class EventTaskPresenter;
    struct EventTaskViewState;
    class RichLineEdit;
    class RichTextEdit;
}

// =============================================================================
/** @class  EventTaskWidget
    @brief  EventTaskWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventTaskWidget : public EventTaskWidget_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventTaskWidget( gui::EventPresenter& presenter );
    virtual ~EventTaskWidget();
    //@}

private:
    //! @name gui::EventTaskWidget_ABC implementation
    //@{
    virtual void BlockSignals( bool blocked );
    virtual void Build( const gui::EventTaskViewState& state );
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< gui::EventTaskPresenter > taskPresenter_;

    gui::RichLineEdit* label_;
    gui::RichTextEdit* description_;
    gui::RichLineEdit* url_;
    QLabel* bytes_;
    QPushButton* showButton_;
    gui::RichTextEdit* payload_;
    QLabel* payloadLabel_;
    int cursorPos_;
    //@}
};

#endif // __EventTaskWidget_h_
