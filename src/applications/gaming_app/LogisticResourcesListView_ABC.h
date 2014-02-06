// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef LogisticResourcesListView_ABC_h
#define LogisticResourcesListView_ABC_h

#include "ResourcesListView_ABC.h"
#include "clients_gui/ObjectNameManager.h"

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  LogisticResourcesListView_ABC
    @brief  LogisticResourcesListView definition
*/
// Created: ABR 2014-02-05
// =============================================================================
template< typename Extension >
class LogisticResourcesListView_ABC : public ResourcesListView_ABC< Extension >
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticResourcesListView_ABC( const QString& objectName,
                                            QWidget* parent,
                                            kernel::Controllers& controllers,
                                            bool listenSelectionChanged = true );
    virtual ~LogisticResourcesListView_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void SelectEntity( const kernel::Entity_ABC* entity );
    //@}

private:
    //! @name Observer implementation
    //@{
    virtual void NotifyUpdated( const Extension& a );
    //@}
};

// -----------------------------------------------------------------------------
// Name: LogisticResourcesListView_ABC constructor
// Created: ABR 2014-02-05
// -----------------------------------------------------------------------------
template< typename Extension >
LogisticResourcesListView_ABC< Extension >::LogisticResourcesListView_ABC( const QString& objectName,
                                                                           QWidget* parent,
                                                                           kernel::Controllers& controllers,
                                                                           bool listenSelectionChanged /* = true */ )
    : ResourcesListView_ABC< Extension >( parent, controllers, listenSelectionChanged )
{
    gui::ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: LogisticResourcesListView_ABC destructor
// Created: ABR 2014-02-05
// -----------------------------------------------------------------------------
template< typename Extension >
LogisticResourcesListView_ABC< Extension >::~LogisticResourcesListView_ABC()
{
    gui::ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}

// -----------------------------------------------------------------------------
// Name: LogisticResourcesListView_ABC::NotifyUpdated
// Created: ABR 2014-02-05
// -----------------------------------------------------------------------------
template< typename Extension >
void LogisticResourcesListView_ABC< Extension >::NotifyUpdated( const Extension& a )
{
    if( !isVisible() || !selected_ )
        return;
    if( !HasRetrieveForLogistic( *selected_, a ) )
        return;
    DisplaySelectionAvailabilities();
}

// -----------------------------------------------------------------------------
// Name: LogisticResourcesListView_ABC::SelectEntity
// Created: ABR 2014-02-06
// -----------------------------------------------------------------------------
template< typename Extension >
void LogisticResourcesListView_ABC< Extension >::SelectEntity( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
    if( !entity )
        return;
    if( !HasRetrieveForLogistic( *selected_ ) )
    {
        hide();
        return;
    }
    DisplaySelectionAvailabilities();
    show();
}

#endif // LogisticResourcesListView_ABC_h
