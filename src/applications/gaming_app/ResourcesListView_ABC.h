// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ResourcesListView_ABC_h_
#define __ResourcesListView_ABC_h_

#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_gui/ListDisplayer.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Controllers;
    class Displayer_ABC;
}

namespace gui
{
    class ItemFactory_ABC;
    class ValuedListItem;
}

// =============================================================================
/** @class  ResourcesListView_ABC
    @brief  ResourcesListView_ABC
*/
// Created: SBO 2007-02-16
// =============================================================================
template< typename ConcreteDisplayer, typename Extension >
class ResourcesListView_ABC : public gui::ListDisplayer< ConcreteDisplayer >
                            , public tools::Observer_ABC
                            , public tools::ElementObserver_ABC< Extension >
                            , public tools::SelectionObserver< kernel::Entity_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourcesListView_ABC( QWidget* parent, ConcreteDisplayer& displayer, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~ResourcesListView_ABC();
    //@}

protected:
    //! @name Operations
    //@{
    bool ShouldUpdate( const Extension& a ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ResourcesListView_ABC( const ResourcesListView_ABC& );            //!< Copy constructor
    ResourcesListView_ABC& operator=( const ResourcesListView_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void polish();
    virtual void showEvent( QShowEvent* );
    virtual void NotifySelected( const kernel::Entity_ABC* entity );
    virtual void NotifyUpdated( const Extension& a ) = 0;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC constructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Extension >
ResourcesListView_ABC< ConcreteDisplayer, Extension >::ResourcesListView_ABC( QWidget* parent, ConcreteDisplayer& displayer, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : gui::ListDisplayer< ConcreteDisplayer >( parent, displayer, factory )
    , controllers_( controllers )
    , selected_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC destructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Extension >
ResourcesListView_ABC< ConcreteDisplayer, Extension >::~ResourcesListView_ABC()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC::ShouldUpdate
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Extension >
bool ResourcesListView_ABC< ConcreteDisplayer, Extension >::ShouldUpdate( const Extension& a ) const
{
    return isVisible() && selected_ && selected_->Retrieve< Extension >() == &a;
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC::polish
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Extension >
void ResourcesListView_ABC< ConcreteDisplayer, Extension >::polish()
{
    gui::ListDisplayer< ConcreteDisplayer >::polish();
    setResizeMode( Q3ListView::AllColumns );
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC::showEvent
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Extension >
void ResourcesListView_ABC< ConcreteDisplayer, Extension >::showEvent( QShowEvent* event )
{
    const kernel::Entity_ABC* selected = selected_;
    selected_ = 0;
    NotifySelected( selected );
    gui::ListDisplayer< ConcreteDisplayer >::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: ResourcesListView_ABC::NotifySelected
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
template< typename ConcreteDisplayer, typename Extension >
void ResourcesListView_ABC< ConcreteDisplayer, Extension >::NotifySelected( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
    if( const Extension* extension = selected_ ? selected_->Retrieve< Extension >() : 0 )
    {
        show();
        NotifyUpdated( *extension );
    }
    else
        hide();
}

#endif // __ResourcesListView_ABC_h_
