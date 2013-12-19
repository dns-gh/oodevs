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
#include "clients_gui/EventTaskView_ABC.h"

namespace gui
{
    class EventTaskPresenter;
    struct EventTaskViewState;
}

// =============================================================================
/** @class  EventTaskWidget
    @brief  EventTaskWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventTaskWidget : public EventWidget_ABC< gui::EventTaskView_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventTaskWidget( gui::EventPresenter& presenter );
    virtual ~EventTaskWidget();
    //@}

private:
    //! @name EventWidget_ABC implementation
    //@{
    virtual void Purge();
    virtual void BlockSignals( bool blocked );
    //@}

    //! @name gui::EventTaskView_ABC implementation
    //@{
    virtual void Build( const gui::EventTaskViewState& state );
    //@}

    //! @name Helpers
    //@{
    void SaveCursor( QTextEdit& textEdit );
    void RestoreCursor( QTextEdit& textEdit );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnDescriptionChanged();
    void OnPayloadChanged();
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< gui::EventTaskPresenter > taskPresenter_;

    QLineEdit* label_;
    QTextEdit* description_;
    QLineEdit* url_;
    QLabel* bytes_;
    QPushButton* showButton_;
    QTextEdit* payload_;
    QLabel* payloadLabel_;
    int cursorPos_;
    //@}
};

#endif // __EventTaskWidget_h_
