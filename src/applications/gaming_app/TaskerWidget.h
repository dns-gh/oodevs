// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __TaskerWidget_h_
#define __TaskerWidget_h_

#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class ActionController;
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class EntitySymbols;
    class RichGroupBox;
    class RichLabel;
}

// =============================================================================
/** @class  TaskerWidget
    @brief  TaskerWidget
*/
// Created: ABR 2013-12-17
// =============================================================================
class TaskerWidget : public QWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TaskerWidget( kernel::Controllers& controllers,
                           const gui::EntitySymbols& symbols,
                           bool showActivate = true,
                           bool showClear = true,
                           QWidget* parent = 0 );
    virtual ~TaskerWidget();
    //@}

    //! @name Operations
    //@{
    void SetTasker( kernel::Entity_ABC* entity );
    const kernel::Entity_ABC* GetTasker() const;

    void BlockSignals( bool blocked );
    void EnableStaticWarning( bool warn, const QColor& color = QColor() );
    //@}

signals:
    //! @name Signals
    //@{
    void ClearClicked();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnActivateClicked();
    void OnClearClicked();
    //@}

private:
    //! @name Member data
    //@{
    kernel::ActionController& actionController_;
    const gui::EntitySymbols& symbols_;
    kernel::SafePointer< kernel::Entity_ABC > tasker_;

    gui::RichGroupBox* groupBox_;
    gui::RichLabel* nameLabel_;
    gui::RichLabel* symbolLabel_;
    QPushButton* activateButton_;
    QPushButton* clearButton_;
    //@}
};

#endif // __TaskerWidget_h_
