// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __LogisticRouteWidget_h_
#define __LogisticRouteWidget_h_

#include "tools/Observer_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class Pathfind_ABC;
}

namespace actions
{
    namespace parameters
    {
        class PullFlowParameters;
        class PushFlowParameters;
    }
}

namespace gui
{
    class EntitySymbols;
}

// =============================================================================
/** @class  LogisticRouteWidget
    @brief  LogisticRouteWidget
*/
// Created: LGY 2014-09-03
// =============================================================================
class LogisticRouteWidget : public QWidget
                          , public tools::Observer_ABC
                          , public kernel::ContextMenuObserver_ABC< kernel::Pathfind_ABC >
                          , public tools::ElementObserver_ABC< kernel::Pathfind_ABC >
                          , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                          , public tools::ElementObserver_ABC< kernel::Formation_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LogisticRouteWidget( kernel::Controllers& controllers, const gui::EntitySymbols& symbols );
    virtual ~LogisticRouteWidget();
    //@}

    //! @name Operations
    //@{
    void AddRecipient( const kernel::Entity_ABC& recipient );
    void RemoveRecipient( const kernel::Entity_ABC& recipient );
    void AddRequester( const kernel::Entity_ABC* requester );
    void Clear();

    virtual void NotifyContextMenu( const kernel::Pathfind_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyDeleted( const kernel::Pathfind_ABC& entity );
    virtual void NotifyDeleted( const kernel::Automat_ABC& entity );
    virtual void NotifyDeleted( const kernel::Formation_ABC& entity );

    void FillPushFlowParameters( actions::parameters::PushFlowParameters& parameters );
    void FillPullFlowParameters( actions::parameters::PullFlowParameters& parameters );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnUp( int index );
    void OnDown( int index );
    void SelectRoute( int id );
    void ToggleRouteActivation( int id );
    //@}

private:
    //! @name Helpers
    //@{
    void Build();
    void Remove( const kernel::Entity_ABC& recipient );
    const kernel::Pathfind_ABC* GetPathfind( unsigned int id ) const;
    //@}

private:
    //! @name Helpers
    //@{
    typedef std::pair< bool, const kernel::Pathfind_ABC* > T_Route;
    //@}

private:
    //! @name Member data
    //@{
    QGridLayout* layout_;
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Pathfind_ABC > selectedPathfind_;
    const gui::EntitySymbols& symbols_;
    QIcon upIcon_;
    QIcon downIcon_;
    QSignalMapper* upSignalMapper_;
    QSignalMapper* downSignalMapper_;
    QSignalMapper* routeSignalMapper_;
    QSignalMapper* routeActivatedSignalMapper_;
    std::vector< const kernel::Entity_ABC* > recipients_;
    std::map< unsigned int, T_Route > routes_;
    //@}
};

#endif // __LogisticRouteWidget_h_
