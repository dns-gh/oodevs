// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeAddInGroupDialog_h_
#define __KnowledgeAddInGroupDialog_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
    class Agent_ABC;
    class Object_ABC;
    class KnowledgeGroup_ABC;
    class Population_ABC;
    class Controllers;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

class StaticModel;

// =============================================================================
/** @class  KnowledgeAddInGroupDialog
@brief  KnowledgeAddInGroupDialog
*/
// Created: MMC 2011-06-06
// =============================================================================
class KnowledgeAddInGroupDialog : public QDialog
    , public tools::Observer_ABC
    , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
    , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
    , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
    , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeAddInGroupDialog( QWidget* pParent, kernel::Controllers& controllers, const kernel::Time_ABC& simulation, actions::ActionsModel& actionsModel, const ::StaticModel& staticModel );
    virtual ~KnowledgeAddInGroupDialog();
    //@}

    //! @name Operations
    //@{
    void Show( kernel::SafePointer< kernel::KnowledgeGroup_ABC > knowledgeGroup );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnReject();
    void SetTarget();
    void closeEvent( QCloseEvent * e );
    //@}

private:
    //! @name Helpers
    //@{
    void Close();
    void setDefaultTargetName();
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu );
    void InsertInMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Time_ABC& simulation_;
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    QComboBox* pPerceptionCombo_;
    QLabel* pTargetNameTitle_;
    QLabel* pTargetName_;
    const kernel::Entity_ABC* pTempTarget_;
    const kernel::Entity_ABC* pSelectedTarget_;
    kernel::SafePointer< kernel::KnowledgeGroup_ABC > selectedKnowledgeGroup_;
    //@}
};

#endif // __KnowledgeAddInGroupDialog_h_
