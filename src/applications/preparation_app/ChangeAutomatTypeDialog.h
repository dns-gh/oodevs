// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ChangeAutomatTypeDialog_h_
#define __ChangeAutomatTypeDialog_h_

namespace gui
{
    class ItemFactory_ABC;
}

namespace kernel
{
    class AgentTypes;
    class Automat_ABC;
    class Controllers;
}

class ModelBuilder;

// =============================================================================
/** @class  ChangeAutomatTypeDialog
    @brief  ChangeAutomatTypeDialog
*/
// Created: JSR 2012-06-29
// =============================================================================
class ChangeAutomatTypeDialog : public QDialog
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             ChangeAutomatTypeDialog( QWidget* parent, kernel::Controllers& controllers, const kernel::AgentTypes& list, ModelBuilder& builder, gui::ItemFactory_ABC& factory, kernel::Automat_ABC& automat );
    virtual ~ChangeAutomatTypeDialog();
    //@}

    //! @name slots
    //@{
public slots:
    void OnOk();
    void SelectionChanged();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ModelBuilder& builder_;
    kernel::Automat_ABC& automat_;
    Q3ListView* list_;
    QPushButton* okBtn_;
    //@}
};

#endif // __ChangeAutomatTypeDialog_h_
