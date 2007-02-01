// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ElementListView_h_
#define __ElementListView_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/ValuedListItem.h"

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  ElementListView
    @brief  ElementListView
*/
// Created: SBO 2007-02-01
// =============================================================================
template< typename Element >
class ElementListView : public QListView 
                      , public kernel::Observer_ABC
                      , public kernel::ElementObserver_ABC< Element >
{

public:
    //! @name Constructors/Destructor
    //@{
             ElementListView( QWidget* parent, kernel::Controllers& controllers );
    virtual ~ElementListView();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ElementListView( const ElementListView& );            //!< Copy constructor
    ElementListView& operator=( const ElementListView& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Element& element );
    virtual void NotifyUpdated( const Element& element );
    virtual void NotifyDeleted( const Element& element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ElementListView constructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
template< typename Element >
ElementListView< Element >::ElementListView( QWidget* parent, kernel::Controllers& controllers )
    : QListView( parent )
    , controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ExerciseList destructor
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
template< typename Element >
ElementListView< Element >::~ElementListView()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: ElementListView::NotifyCreated
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
template< typename Element >
void ElementListView< Element >::NotifyCreated( const Element& element )
{
    gui::ValuedListItem* item = new gui::ValuedListItem( this );
    item->SetNamed( element );
}

// -----------------------------------------------------------------------------
// Name: ElementListView::NotifyUpdated
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
template< typename Element >
void ElementListView< Element >::NotifyUpdated( const Element& element )
{
    if( gui::ValuedListItem* item = gui::FindItem( &element, firstChild() ) )
        item->SetNamed( element );
}

// -----------------------------------------------------------------------------
// Name: ElementListView::NotifyDeleted
// Created: SBO 2007-01-29
// -----------------------------------------------------------------------------
template< typename Element >
void ElementListView< Element >::NotifyDeleted( const Element& element )
{
    if( gui::ValuedListItem* item = gui::FindItem( &element, firstChild() ) )
        delete item;
}

#endif // __ElementListView_h_
