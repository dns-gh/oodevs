// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __EventMarkerWidget_h_
#define __EventMarkerWidget_h_

#include "EventWidget_ABC.h"
#include <boost/shared_ptr.hpp>

namespace gui
{
    class EventMarkerPresenter;
    class RichCheckBox;
    class RichLineEdit;
    class RichPathWidget;
    class RichTextEdit;
}

// =============================================================================
/** @class  EventMarkerWidget
    @brief  EventMarkerWidget
*/
// Created: JSR 2014-10-27
// =============================================================================
class EventMarkerWidget : public EventMarkerWidget_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventMarkerWidget( gui::EventPresenter& presenter, const tools::Path& exercisePath );
    virtual ~EventMarkerWidget();
    //@}

private:
    //! @name EventMarkerWidget_ABC implementation
    //@{
    virtual void BlockSignals( bool blocked );
    virtual void Build( const gui::EventMarkerViewState& state );
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< gui::EventMarkerPresenter > markerPresenter_;
    gui::RichLineEdit* label_;
    gui::RichTextEdit* description_;
    gui::RichCheckBox* resetDrawings_;
    gui::RichPathWidget* drawingsPath_;
    gui::RichPathWidget* configurationPath_;
    //@}
};

#endif // __EventMarkerWidget_h_
