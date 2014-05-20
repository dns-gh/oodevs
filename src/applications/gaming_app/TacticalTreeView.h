// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __TacticalTreeView_h_
#define __TacticalTreeView_h_

#include "clients_gui/TacticalTreeView.h"
#include "clients_gui/ChangeSuperior_ABC.h"
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
    class Decisions_ABC;
}

class StaticModel;

// =============================================================================
/** @class  TacticalTreeView
    @brief  TacticalTreeView
*/
// Created: JSR 2012-09-27
// =============================================================================
class TacticalTreeView  : public gui::TacticalTreeView
                        , public gui::ChangeSuperior_ABC
                        , public tools::ElementObserver_ABC< gui::Decisions_ABC >
                        , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                        , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                        , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                        , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             TacticalTreeView( const QString& objectName, kernel::Controllers& controllers, const kernel::Profile_ABC& profile,
                               gui::ModelObserver_ABC& modelObserver, const gui::EntitySymbols& symbols, const StaticModel& staticModel,
                               const kernel::Time_ABC& simulation, actions::ActionsModel& actionsModel, QWidget* parent = 0 );
    virtual ~TacticalTreeView();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyUpdated( const gui::Decisions_ABC& decisions );
    virtual bool CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const;
    virtual void DoChangeSuperior( kernel::Entity_ABC& entity, kernel::Entity_ABC& superior );
    virtual void Drop( const kernel::Agent_ABC& item, const kernel::Entity_ABC& target );
    virtual void Drop( const kernel::Automat_ABC& item, const kernel::Entity_ABC& target);
    virtual void Drop( const kernel::Formation_ABC& item, const kernel::Entity_ABC& target);
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChangeSuperior();
    void OnCreateFormation( int level );
    void OnCreateLogisticBase( int level );
    void ChangeDisplay( int mode );
    virtual void OnActivate( const QModelIndex& index );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void drawRow( QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex &index ) const;
    virtual std::vector< const QPixmap* > GetEntityPixmap( const kernel::Entity_ABC& entity );
    virtual bool ApplyProfileFilter( QStandardItem& item ) const;
    void CreateFormation( int level, bool isLogisticBase );
    void AddCommonMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Types
    //@{
    enum EDisplayMode
    {
        eObservableUnits,
        eControlledUnits,
        eSides
    };
    //@}

private:
    //! @name Member data
    //@{
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    actions::ActionsModel& actionsModel_;
    EDisplayMode displayMode_;
    gui::ChangeSuperiorDialog* changeSuperiorDialog_;
    const kernel::Entity_ABC* currentEntity_;
    QPixmap icon_user_;
    //@}
};

#endif // __TacticalTreeView_h_
