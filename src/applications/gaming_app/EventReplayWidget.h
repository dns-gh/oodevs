// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __EventReplayWidget_h_
#define __EventReplayWidget_h_

#include "EventWidget_ABC.h"
#include <boost/shared_ptr.hpp>

namespace gui
{
    class EventReplayPresenter;
    class RichCheckBox;
    class RichLineEdit;
    class RichTextEdit;
}

// =============================================================================
/** @class  EventReplayWidget
    @brief  EventReplayWidget
*/
// Created: JSR 2014-10-27
// =============================================================================
class EventReplayWidget : public EventReplayWidget_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit EventReplayWidget( gui::EventPresenter& presenter );
    virtual ~EventReplayWidget();
    //@}

private:
    //! @name EventReplayWidget_ABC implementation
    //@{
    virtual void BlockSignals( bool blocked );
    virtual void Build( const gui::EventReplayViewState& state );
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< gui::EventReplayPresenter > replayPresenter_;
    gui::RichLineEdit* label_;
    gui::RichTextEdit* description_;
    gui::RichCheckBox* enabled_;
    //@}
};

#endif // __EventReplayWidget_h_
