// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ListView_h_
#define __ListView_h_

#include "tools/Iterator.h"
#include "ValuedListItem.h"
#include "ItemFactory_ABC.h"

namespace gui
{
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
             ListView( QWidget* parent, ConcreteList& list, ItemFactory_ABC& factory, const char* name = 0 )
                 : QListView( parent, name )
                 , list_( list )
                 , factory_( factory )
                 , toSkip_( 0 )
             {
                 setDefaultRenameAction( QListView::Accept );
             }
    virtual ~ListView() {}
    //@}

    //! @name Operations
    //@{
    template< typename Element, typename Parent >
    ValuedListItem* Display( tools::Iterator< const Element& > it, Parent* parent, ValuedListItem* currentItem = 0 )
    {
        if( ! parent )
            throw std::runtime_error( "Missing parent !" );
        ValuedListItem* previousItem = currentItem;
        while( it.HasMoreElements() )
        {
            const Element& element = it.NextElement();
            if( ! currentItem  ) 
                currentItem = CreateItem( parent, previousItem );
            currentItem->SetValue( &element );
            previousItem = currentItem;
            currentItem = (ValuedListItem*)( currentItem->nextSibling() );
            list_.Display( element, previousItem );
        }
        return currentItem ? currentItem : CreateItem( parent, previousItem );
    };
    template< typename Iterator, typename Parent >
    ValuedListItem* Display( Iterator from, const Iterator& to, Parent* parent, ValuedListItem* currentItem = 0 )
    {
        if( ! parent )
            throw std::runtime_error( "Missing parent !" );
        ValuedListItem* previousItem = currentItem;
        while( from != to )
        {
            if( ! currentItem  ) 
                currentItem = CreateItem( parent, previousItem );
            previousItem = currentItem;
            currentItem = (ValuedListItem*)( currentItem->nextSibling() );
            list_.Display( *from, previousItem );
            ++from;
        }
        return currentItem ? currentItem : CreateItem( parent, previousItem );
    };

    void RemoveItem( ValuedListItem* item )
    {
        if( item->nextSibling() )
            delete item;
        else
            toSkip_ = item;
    }

    void DeleteTail( ValuedListItem* item )
    {
        while( item )
        {
            ValuedListItem* next = (ValuedListItem*) item->nextSibling();
            delete item;
            item = next;
        }
    };

    template< typename Parent >
    ValuedListItem* CreateItem( Parent* parent, ValuedListItem* previousItem )
    {
        if( previousItem && previousItem == toSkip_ ) {
            toSkip_ = 0;
            return previousItem;
        }
        return factory_.CreateItem( parent, previousItem );
    }
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
    ItemFactory_ABC& factory_;
    ValuedListItem* toSkip_;
    //@}
};

}

#endif // __ListView_h_
