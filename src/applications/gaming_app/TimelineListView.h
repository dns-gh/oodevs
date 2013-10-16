// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelineListView_h_
#define __TimelineListView_h_

#include <tools/ElementObserver_ABC.h>
#include <tools/SelectionObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace actions
{
    class Action_ABC;
    class ActionsFilter_ABC;
    enum EActionType;
}

class Model;

// =============================================================================
/** @class  TimelineListView
    @brief  TimelineListView
*/
// Created: SBO 2008-04-22
// =============================================================================
class TimelineListView : public QTreeWidget
                       , public tools::Observer_ABC
                       , public tools::ElementObserver_ABC< actions::Action_ABC >
                       , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                       , public tools::SelectionObserver< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TimelineListView( QWidget* parent, kernel::Controllers& controllers, const Model& model );
    virtual ~TimelineListView();
    //@}

    //! @name Operations
    //@{
    void SetFilter( const actions::ActionsFilter_ABC& filter );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSelectionChange( const QItemSelection&, const QItemSelection& );
    void SetContentsPos( int dx, int dy );
    void OnVScrollbarChanged( int y );
    //@}

signals:
    //! @name Signals
    //@{
    void ContentsMoving( int, int );
    //@}

private:
    //! @name Helpers
    //@{
    void Update();
    bool ShouldDisplay( const kernel::Entity_ABC& entity ) const;
    QTreeWidgetItem* FindItem( const kernel::Entity_ABC* entity ) const;
    QTreeWidgetItem* FindListItem( const actions::Action_ABC& action, actions::EActionType& actionType ) const;
    virtual void NotifyCreated( const actions::Action_ABC& action );
    virtual void NotifyDeleted( const actions::Action_ABC& action );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const actions::Action_ABC* >        T_Actions;
    typedef std::map< const kernel::Entity_ABC*, T_Actions > T_EntityActions;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const Model& model_;
    T_EntityActions entityActions_;
    T_Actions weatherActions_;
    T_Actions objectsActions_;
    T_Actions magicActions_;
    const actions::ActionsFilter_ABC* filter_;
    QTreeWidgetItem* magicItem_;
    QTreeWidgetItem* weatherItem_;
    QTreeWidgetItem* objectItem_;
    bool blockSelect_;
    //@}
};

#endif // __TimelineListView_h_
