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
    class ModelObserver_ABC;
    class UnitTreeView;
    class RichPushButton;
}

namespace kernel
{
    class AgentTypes;
    class Entity_ABC;
    class Controllers;
}

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
             ChangeAutomatTypeDialog( QWidget* parent, kernel::Controllers& controllers, const kernel::AgentTypes& list,
                                      gui::ModelObserver_ABC& builder, kernel::Entity_ABC& entity, const std::string& typeName );
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
    gui::ModelObserver_ABC& builder_;
    kernel::Entity_ABC& entity_;
    gui::UnitTreeView* list_;
    gui::RichPushButton* okBtn_;
    //@}
};

#endif // __ChangeAutomatTypeDialog_h_
