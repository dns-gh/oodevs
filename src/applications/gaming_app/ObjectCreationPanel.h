// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectCreationPanel_h_
#define __ObjectCreationPanel_h_

#include "clients_gui/InfoPanel_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class ModelLoaded;
    class Team_ABC;
    class Time_ABC;
    class Viewport_ABC;
}

namespace gui
{
    class ParametersLayer;
    class PanelStack_ABC;
}

class ObjectPrototype;
class StaticModel;

// =============================================================================
/** @class  ObjectCreationPanel
    @brief  Object creation panel
*/
// Created: SBO 2006-04-18
// =============================================================================
class ObjectCreationPanel : public gui::InfoPanel_ABC
                          , public tools::Observer_ABC
                          , public tools::ElementObserver_ABC< kernel::ModelLoaded >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ObjectCreationPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, const kernel::Team_ABC& noSideTeam, gui::ParametersLayer& layer, const kernel::GlTools_ABC& tools );
    virtual ~ObjectCreationPanel();
    //@}

    //! @name Operations
    //@{
    void Draw( const kernel::Viewport_ABC& viewport );
    //@}

private slots:
    //! @name Slots
    //@{
    void Commit();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectCreationPanel( const ObjectCreationPanel& );            //!< Copy constructor
    ObjectCreationPanel& operator=( const ObjectCreationPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const kernel::GlTools_ABC& tools_;
    ObjectPrototype* created_;
    //@}
};

#endif // __ObjectCreationPanel_h_
