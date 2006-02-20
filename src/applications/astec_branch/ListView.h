// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListView_h_
#define __ListView_h_

#include "Iterator.h"

class ValuedListItem;

// =============================================================================
/** @class  ListView
    @brief  ListView
*/
// Created: AGE 2006-02-20
// =============================================================================
template< typename ConcreteList >
class ListView : public QListView
{

public:
    //! @name Constructors/Destructor
    //@{
             ListView( QWidget* parent, ConcreteList& list )
                 : QListView( parent )
                 , list_( list ) {};
    virtual ~ListView()
        {};
    //@}

    //! @name Operations
    //@{
    template< typename Element, typename Parent >
    ValuedListItem* Display( Iterator< const Element& >& it, Parent* parent, ValuedListItem* currentItem = 0 )
    {
        ValuedListItem* previousItem = currentItem;
        while( it.HasMoreElements() )
        {
            const Element& element = it.NextElement();
            if( ! currentItem  ) 
                currentItem = new ValuedListItem( &element, parent, previousItem );
            list_.Display( element, currentItem );
            previousItem = currentItem;
            currentItem = (ValuedListItem*)( currentItem->nextSibling() );
        }
        return currentItem;
    };

    template< typename Iterator >
    ValuedListItem* Display( Iterator from, Iterator to, ValuedListItem* item = 0 )
    {
    };

    void DeleteTail( ValuedListItem* item )
    {
        while( item )
        {
            ValuedListItem* next = (ValuedListItem*) item->nextSibling();
            delete item;
            item = next;
        }
    };
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ListView( const ListView& );            //!< Copy constructor
    ListView& operator=( const ListView& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ConcreteList& list_;
    //@}
};

#endif // __ListView_h_
