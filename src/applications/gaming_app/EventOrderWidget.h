// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventOrderWidget_h_
#define __EventOrderWidget_h_

#include "EventWidget_ABC.h"
#include "clients_gui/ValuedComboBox.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;
        class MissionInterface_ABC;
        class Param_ABC;
    }
}

namespace gui
{
    class Decisions_ABC;
    class EntitySymbols;
    class EventOrderPresenter;
    struct EventOrderViewState;
    class GlTools_ABC;
    class RichGroupBox;
    class RichLabel;
    template< typename T > class RichWarnWidget;
}

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Population_ABC;
    class Profile_ABC;
    class Time_ABC;
}

namespace tools
{
    class ExerciseConfig;
}

class Model;
class TaskerWidget;

// =============================================================================
/** @class  EventOrderWidget
    @brief  EventOrderWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventOrderWidget : public EventOrderWidget_ABC
                       , public tools::Observer_ABC
                       , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
                       , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                       , public tools::ElementObserver_ABC< gui::Decisions_ABC >
                       , public tools::ElementObserver_ABC< actions::gui::Param_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventOrderWidget( gui::EventPresenter& presenter,
                               kernel::Controllers& controllers,
                               Model& model,
                               const tools::ExerciseConfig& config,
                               actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                               const kernel::Profile_ABC& profile,
                               gui::GlTools_ABC& tools,
                               const kernel::Time_ABC& simulation,
                               const gui::EntitySymbols& entitySymbols );
    virtual ~EventOrderWidget();
    //@}

private:
    //! @name gui::EventBaseView_ABC implementation
    //@{
    virtual void Purge();
    virtual void Draw( gui::Viewport_ABC& viewport );
    virtual void BlockSignals( bool blocked );
    //@}

    //! @name gui::EventView_ABC< gui::EventOrderViewState > implementation
    //@{
    virtual void Build( const gui::EventOrderViewState& state );
    //@}

    //! @name Observers implementation
    //@{
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    virtual void NotifyUpdated( const gui::Decisions_ABC& decisions );
    virtual void NotifyUpdated( const actions::gui::Param_ABC& param );
    //@}

    //! @name Helpers
    //@{
    void OnOrderClicked( const kernel::Entity_ABC* entity );
    void OnTargetChanged( const kernel::Entity_ABC* entity );
    void AddReplaceTargetToMenu( kernel::ContextMenu& menu );

    template< typename T >
    void NotifyOrderReceived( const T& message, int context );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnMissionTypeChanged( const QString& missionType );
    void OnOrderClicked();
    void OnOrderAutomatClicked();
    void OnReplaceTargetClicked();
    void OnClearTaskerClicked();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    actions::gui::InterfaceBuilder_ABC& interfaceBuilder_;
    const kernel::Profile_ABC& profile_;
    gui::GlTools_ABC& tools_;
    const kernel::Time_ABC& simulation_;
    boost::shared_ptr< gui::EventOrderPresenter > orderPresenter_;

    unsigned int lastTaskerId_;
    TaskerWidget* taskerWidget_;
    gui::RichWarnWidget< QComboBox >* missionTypeCombo_;
    QVBoxLayout* missionComboLayout_;
    gui::RichWarnWidget< QComboBox >* missionCombo_;
    QStandardItemModel missionModel_;
    boost::scoped_ptr< actions::gui::MissionInterface_ABC > missionInterface_;

    kernel::SafePointer< kernel::Entity_ABC > selectedEntity_;
    kernel::SafePointer< kernel::Entity_ABC > selectedEngagedAutomat_;
    //@}
};

#endif // __EventOrderWidget_h_
