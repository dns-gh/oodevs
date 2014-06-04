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
    class Controllers;
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class EntitySymbols;
    class RichGroupBox;
    class RichLabel;

// =============================================================================
/** @class  TaskerWidget
    @brief  TaskerWidget
*/
// Created: ABR 2013-12-17
// =============================================================================
class TaskerWidget : public QWidget
                   , public tools::Observer_ABC
                   , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TaskerWidget( kernel::Controllers& controllers,
                           const gui::EntitySymbols& symbols,
                           const QString& title,
                           bool showActivate = true,
                           bool showClear = true,
                           QWidget* parent = 0 );
    virtual ~TaskerWidget();
    //@}

    //! @name Operations
    //@{
    void SetTasker( const kernel::Entity_ABC* entity );
    const kernel::Entity_ABC* GetTasker() const;

    void BlockSignals( bool blocked );
    void EnableStaticWarning( bool warn, const QColor& color = QColor() );

    void setCheckable( bool checkable );
    bool isCheckable() const;

    void setChecked( bool checked );
    bool isChecked() const;
    //@}

signals:
    //! @name Signals
    //@{
    void Clicked( bool checked );
    void TaskerChanged( const kernel::Entity_ABC* tasker );
    void ClearClicked();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnActivateClicked();
    void OnClearClicked();
    //@}

private:
    //! @name tools::ElementObserver_ABC< kernel::Entity_ABC > implementation
    //@{
    virtual void NotifyDeleted( const kernel::Entity_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const gui::EntitySymbols& symbols_;
    const kernel::Entity_ABC* tasker_;

    gui::RichGroupBox* groupBox_;
    gui::RichLabel* nameLabel_;
    gui::RichLabel* symbolLabel_;
    QPushButton* activateButton_;
    QPushButton* clearButton_;
    //@}
};

} //!namespace gui

#endif // __TaskerWidget_h_
