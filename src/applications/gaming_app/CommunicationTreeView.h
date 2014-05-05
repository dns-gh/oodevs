// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __CommunicationTreeView_h_
#define __CommunicationTreeView_h_

#include "clients_gui/ChangeSuperior_ABC.h"
#include "clients_gui/HierarchyTreeView.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class Time_ABC;
}

namespace gui
{
    class ChangeSuperiorDialog;
}

class StaticModel;

// =============================================================================
/** @class  CommunicationTreeView
    @brief  CommunicationTreeView
*/
// Created: JSR 2012-09-28
// =============================================================================
class CommunicationTreeView : public gui::HierarchyTreeView< kernel::CommunicationHierarchies >
                            , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::KnowledgeGroup_ABC >
                            , gui::ChangeSuperior_ABC
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             CommunicationTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile,
                                    gui::ModelObserver_ABC& modelObserver, const gui::EntitySymbols& symbols, const StaticModel& staticModel,
                                    const kernel::Time_ABC& simulation, actions::ActionsModel& actionsModel, QWidget* parent = 0 );
    virtual ~CommunicationTreeView();
    //@}

    //! @name Operations
    //@{
    std::vector< const QPixmap* > GetEntityPixmap( const kernel::Entity_ABC& entity );
    virtual bool CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const;
    virtual void DoChangeSuperior( kernel::Entity_ABC& entity, kernel::Entity_ABC& superior );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChangeKnowledgeGroup();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void drawRow( QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex &index ) const;
    virtual bool ApplyProfileFilter( QStandardItem& item ) const;
    virtual void NotifyCreated( const kernel::Entity_ABC& entity );
    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::KnowledgeGroup_ABC& group, kernel::ContextMenu& menu );
    virtual void Drop( const kernel::Agent_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::Automat_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::Entity_ABC& target );
    void UpdateLongName( const kernel::Entity_ABC& entity );
    void OnCreateKnowledgeGroup( const kernel::SafePointer< kernel::Entity_ABC >& entity, const std::string& type );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Profile_ABC& profile_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    actions::ActionsModel& actionsModel_;
    gui::ChangeSuperiorDialog* changeSuperiorDialog_;
    QPixmap lock_;
    QPixmap commandPost_;
    QPixmap scisors_;
    //@}
};

#endif // __CommunicationTreeView_h_
