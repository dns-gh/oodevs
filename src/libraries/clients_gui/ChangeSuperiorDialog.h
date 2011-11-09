// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ChangeSuperiorDialog_h_
#define __ChangeSuperiorDialog_h_

#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
class ChangeSuperior_ABC;

// =============================================================================
/** @class  ChangeSuperiorDialog
    @brief  ChangeSuperiorDialog
*/
// Created: JSR 2011-11-08
// =============================================================================
class ChangeSuperiorDialog : public QDialog
                           , public tools::Observer_ABC
                           , public tools::SelectionObserver< kernel::Entity_ABC >
                           , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ChangeSuperiorDialog( QWidget* parent, kernel::Controllers& controllers, ChangeSuperior_ABC& changeSuperior, bool communication );
    virtual ~ChangeSuperiorDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    void Show( kernel::Entity_ABC& entity );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnOk();
    void OnCancel();
    //@}

private:
    //! @name Member data
    //@{
    ChangeSuperior_ABC& changeSuperior_;
    kernel::Controllers& controllers_;
    QLabel* superiorLabel_;
    QPushButton* okButton_;
    kernel::Entity_ABC* entity_;
    kernel::Entity_ABC* selected_;
    //@}
};

}

#endif // __ChangeSuperiorDialog_h_
