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
#include "clients_gui/ShapeHandler_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Agent_ABC;
    class AgentKnowledge_ABC;
    class Controllers;
    class ModelLoaded;
    class ModelUnLoaded;
    class Time_ABC;
    class Location_ABC;
}

namespace gui
{
    class GlTools_ABC;
    class LocationCreator;
    class PanelStack_ABC;
    class ParametersLayer;
    class Viewport_ABC;
}

namespace actions
{
    class ActionsModel;
}

class StaticModel;

enum E_StrikeType
{
    eStrikeOnUnit,
    eStrikeOnPosition
};

// =============================================================================
/** @class  FireCreationPanel
    @brief  Object creation panel
*/
// Created: MGD 2010-02-23
// =============================================================================
class FireCreationPanel : public gui::InfoPanel_ABC
                        , public tools::Observer_ABC
                        , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                        , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
                        , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                        , public kernel::ContextMenuObserver_ABC< kernel::AgentKnowledge_ABC >
                        , public gui::ShapeHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             FireCreationPanel( QWidget* parent,
                                gui::PanelStack_ABC& panel,
                                kernel::Controllers& controllers,
                                actions::ActionsModel& actionsModel,
                                const kernel::Time_ABC& simulation,
                                const StaticModel& staticModel,
                                const std::shared_ptr< gui::ParametersLayer >& paramLayer,
                                const gui::GlTools_ABC& tools );
    virtual ~FireCreationPanel();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::AgentKnowledge_ABC& kn, kernel::ContextMenu& menu );
    virtual void Handle( kernel::Location_ABC& location );
    void Draw( gui::Viewport_ABC& viewport );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTypeChanged();
    void Commit();
    void MenuItemTargetValidated();
    void MenuItemReporterValidated();
    void UpdateCommitButton();
    virtual void showEvent( QShowEvent* );
    virtual void hideEvent( QHideEvent* );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    void Reset();
    void RegisterIfNeeded();
    void UnregisterIfNeeded();
    bool CheckValidity() const;
    bool IsStrikeOnLocation() const;
    //@}

private:
    //! @name Member data
    //@{
    const StaticModel& staticModel_;
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const kernel::Time_ABC& simulation_;
    const gui::GlTools_ABC& tools_;

    gui::ValuedComboBox< E_StrikeType >* strikeCombo_;
    Q3HBox* locationBox_;
    Q3HBox* targetBox_;
    Q3HBox* reporterBox_;

    gui::LocationCreator* locationCreator_;
    kernel::Location_ABC* location_;
    QLabel* locationLabel_;
    QLabel* locationTitle_;
    bool isLocationRegistered_;

    unsigned long potentialTarget_;
    unsigned long selectedTarget_;
    const kernel::Agent_ABC* potentialReporter_;
    const kernel::Agent_ABC* selectedReporter_;

    QString potentialTargetName_;
    QLabel* targetLabel_;
    QLabel* reporterLabel_;
    QLabel* targetTitle_;
    QLabel* reporterTitle_;
    gui::ValuedComboBox< int >* ammunitionsBox_;
    QLineEdit* interventionType_;
    QPushButton* ok_;
    //@}
};

#endif // __FireCreationPanel_h_
