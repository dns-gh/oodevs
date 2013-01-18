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
    class AutomatDecisions_ABC;
    class Time_ABC;
}

namespace gui
{
    class ChangeSuperiorDialog;
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
                        , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
                        , public tools::ElementObserver_ABC< kernel::AutomatDecisions_ABC >
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             TacticalTreeView( kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::ModelObserver_ABC& modelObserver, const gui::EntitySymbols& symbols, const StaticModel& staticModel, const kernel::Time_ABC& simulation, actions::ActionsModel& actionsModel, QWidget* parent = 0 );
    virtual ~TacticalTreeView();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyUpdated( const kernel::AutomatDecisions_ABC& decisions );
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
    void ChangeDisplay( int mode );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void drawRow( QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex &index ) const;
    virtual std::vector< const QPixmap* > GetEntityPixmap( const kernel::Entity_ABC& entity );
    virtual bool ApplyProfileFilter( QStandardItem& item, gui::StandardModel& model ) const;
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
    QPixmap icon_user_;
    //@}
};

#endif // __TacticalTreeView_h_
