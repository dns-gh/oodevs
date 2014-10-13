// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InhabitantCreationPanel_h_
#define __InhabitantCreationPanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "clients_gui/ValuedComboBox.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Resolver_ABC.h>
#include <tools/Resolver.h>

namespace kernel
{
    class Controllers;
    class Team_ABC;
    class InhabitantType;
    class ModelLoaded;
    class ModelUnLoaded;
    class AgentTypes;
}

namespace gui
{
    class GLView_ABC;
    class LocationCreator;
    class ParametersLayer;
    class RichLabel;
    class Viewport_ABC;
    class RichSpinBox;
    class RichLineEdit;
}

class AgentsModel;

// =============================================================================
/** @class  InhabitantCreationPanel
    @brief  Inhabitant creation panel
*/
// Created: SLG 2010-11-25
// =============================================================================
class InhabitantCreationPanel : public gui::InfoPanel_ABC
                              , public tools::Observer_ABC
                              , public tools::ElementObserver_ABC< kernel::Team_ABC >
                              , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                              , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
                              , public gui::ShapeHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InhabitantCreationPanel( QWidget* parent,
                                      gui::PanelStack_ABC& panel,
                                      kernel::Controllers& controllers,
                                      const kernel::AgentTypes& types,
                                      AgentsModel& agentsModel,
                                      const std::shared_ptr< gui::ParametersLayer >& layer,
                                      const gui::GLView_ABC& tools );
    virtual ~InhabitantCreationPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( gui::Viewport_ABC& viewport );
    virtual void Handle( kernel::Location_ABC& location );
    //@}

public slots:
    //! @name Slots
    //@{
    void Commit2();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTypeChanged();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyUpdated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    virtual void showEvent( QShowEvent* );
    virtual void hideEvent( QHideEvent* );
    void FillObjectTypes();
    void ResetLocation();
    bool CheckValidity() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const gui::GLView_ABC& tools_;
    const tools::Resolver_ABC< kernel::InhabitantType >& resolver_;
    AgentsModel& agentsModel_;
    kernel::Location_ABC* location_;
    gui::ValuedComboBox< const kernel::Team_ABC* >* teams_;
    gui::ValuedComboBox< const kernel::InhabitantType* >* inhabitantTypes_;
    gui::RichLineEdit* name_;
    gui::RichSpinBox* number_;
    gui::RichLabel* position_;
    QLabel* locationLabel_;
    gui::LocationCreator* locationCreator_;
    //@}
};

#endif // __InhabitantCreationPanel_h_