// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// LTO
//
// *****************************************************************************

#ifndef __KnowledgeGroupMagicOrdersInterface_h_
#define __KnowledgeGroupMagicOrdersInterface_h_

#include "tools/Resolver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/KnowledgeGroupType.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Team_ABC;
    class KnowledgeGroup_ABC;
    class Profile_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

class StaticModel;
class KnowledgeAddInGroupDialog;

// =============================================================================
/** @class  KnowledgeGroupMagicOrdersInterface
    @brief  KnowledgeGroupMagicOrdersInterface
*/
// Created: SLG 2009-12-17
// =============================================================================
class KnowledgeGroupMagicOrdersInterface : public QObject
                                         , public tools::Observer_ABC
                                         , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                                         , public kernel::ContextMenuObserver_ABC< kernel::KnowledgeGroup_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeGroupMagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, const kernel::Profile_ABC& profile, const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types );
    virtual ~KnowledgeGroupMagicOrdersInterface();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::KnowledgeGroup_ABC& entity, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnToggleKnowledgeGroupActivation();
    void OnAddKnowledgeInGroup();
    void OnSetType();
    //@}

private:
    void OnCreateKnowledgeGroup( const kernel::SafePointer< kernel::Entity_ABC >& entity, const std::string& type );

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::KnowledgeGroupType, std::string >& types_;
    kernel::Controllers& controllers_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    const kernel::Profile_ABC& profile_;
    kernel::SafePointer< kernel::KnowledgeGroup_ABC > selectedEntity_;
    KnowledgeAddInGroupDialog* pAddKnowledgeDialog_;
    //@}
};

#endif // __ObjectMagicOrdersInterface_h_
