// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
// LTO
// *****************************************************************************

#ifndef __FireCreationPanel_h_
#define __FireCreationPanel_h_

#include "clients_gui/ValuedComboBox.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_gui/InfoPanel_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Agent_ABC;
    class AgentKnowledge_ABC;
    class Controllers;
    class ModelLoaded;
    class Time_ABC;
}

namespace gui
{
    class PanelStack_ABC;
}

namespace actions
{
    class ActionsModel;
}

class StaticModel;

// =============================================================================
/** @class  FireCreationPanel
    @brief  Object creation panel
*/
// Created: MGD 2010-02-23
// =============================================================================
class FireCreationPanel : public gui::InfoPanel_ABC
                        , public tools::Observer_ABC
                        , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                        , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                        , public kernel::ContextMenuObserver_ABC< kernel::AgentKnowledge_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             FireCreationPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers
                              , actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation, const StaticModel& staticModel );
    virtual ~FireCreationPanel();
    //@}

    //! @name Notification
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::AgentKnowledge_ABC& kn, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void Commit();
    void MenuItemTargetValidated();
    void MenuItemReporterValidated();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    FireCreationPanel( const FireCreationPanel& );            //!< Copy constructor
    FireCreationPanel& operator=( const FireCreationPanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    //@}

private:
    //! @name Member data
    //@{
    const StaticModel& staticModel_;
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const kernel::Time_ABC& simulation_;

    const kernel::AgentKnowledge_ABC* potentialTarget_;
    const kernel::AgentKnowledge_ABC* selectedTarget_;
    const kernel::Agent_ABC* potentialReporter_;
    const kernel::Agent_ABC* selectedReporter_;

    QLabel* targetLabel_;
    QLabel* reporterLabel_;
    gui::ValuedComboBox< int >* ammunitionsBox_;
    QLineEdit* interventionType_;
    //@}
};

#endif // __FireCreationPanel_h_
