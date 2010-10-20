// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __CreateFormationDialog_h_
#define __CreateFormationDialog_h_

#include "tools/Observer_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class AgentTypes;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class FormationLevels;
    class HierarchyLevel_ABC;
    class Profile_ABC;
    class Team_ABC;
}

// =============================================================================
/** @class  CreateFormationDialog
    @brief  CreateFormationDialog
*/
// Created: LDC 2010-10-12
// =============================================================================
class CreateFormationDialog : public QDialog
                            , public tools::Observer_ABC
                            , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
{

    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    CreateFormationDialog( QWidget* parent, kernel::Controllers& controllers, const kernel::FormationLevels& levels, const kernel::Profile_ABC& profile, actions::ActionsModel& actionsModel, kernel::AgentTypes& agentTypes );
    virtual ~CreateFormationDialog();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Team_ABC&, kernel::ContextMenu& );
    virtual void NotifyContextMenu( const kernel::Formation_ABC&, kernel::ContextMenu& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CreateFormationDialog( const CreateFormationDialog& );            //!< Copy constructor
    CreateFormationDialog& operator=( const CreateFormationDialog& ); //!< Assignment operator
    //@}
    
    //! @name Helpers
    //@{
    void NotifyContextMenu( const kernel::Entity_ABC&, const kernel::HierarchyLevel_ABC* level, kernel::ContextMenu& menu);
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreateFormation( int level );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profile_;
    const kernel::FormationLevels& levels_;
    actions::ActionsModel& actionsModel_;
    const kernel::Entity_ABC* currentEntity_;
    kernel::AgentTypes& agentTypes_;
    //@}
};

#endif // __CreateFormationDialog_h_
