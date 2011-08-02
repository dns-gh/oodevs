// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListDisplayer_h_
#define __ListDisplayer_h_

#include "ListView.h"
#include "ListItemDisplayer.h"

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

namespace gui
{

// =============================================================================
/** @class  ListDisplayer
    @brief  ListDisplayer
*/
// Created: AGE 2006-02-22
// =============================================================================
template< typename ConcreteDisplayer >
class ListDisplayer : public ListView< ListDisplayer< ConcreteDisplayer > >
{
    typedef ListView< ListDisplayer< ConcreteDisplayer > > T_Parent;

public:
    //! @name Constructors/Destructor
    //@{
             ListDisplayer( QWidget* parent, ConcreteDisplayer& displayer, ItemFactory_ABC& factory, const char* name = 0 )
                 : T_Parent( parent, *this, factory, name )
                 , displayer_( displayer )
             {
                setSorting( -1 );
                setAllColumnsShowFocus( true );
             };
    virtual ~ListDisplayer() {};
    //@}

    //! @name Operations
    //@{
    ListDisplayer& AddColumn( const QString& column ) {
        addColumn( column );
        itemDisplayer_.AddColumn( column );
        setSorting( columns() - 1, true );
        setResizeMode( Q3ListView::LastColumn );
        return *this;
    }
    template< typename Element >
    ValuedListItem* DisplayList( tools::Iterator< const Element& > it ) {
        if( it.HasMoreElements() )
            show();
        return T_Parent::Display( it, this, (ValuedListItem*)( firstChild() ) );
    }
    template< typename Element >
    ValuedListItem* DisplayList( tools::Iterator< const Element& > it, Q3ListViewItem* parent ) {
        if( it.HasMoreElements() )
            show();
        return T_Parent::Display( it, parent, (ValuedListItem*)( parent->firstChild() ) );
    }
    template< typename Element >
    ValuedListItem* DisplayList( tools::Iterator< const Element& > it, Q3ListView* parent, ValuedListItem* at ) {
        if( it.HasMoreElements() )
            show();
        return T_Parent::Display( it, parent, at );
    }
    template< typename Element >
    ValuedListItem* DisplayList( tools::Iterator< const Element& > it, ValuedListItem* parent, ValuedListItem* at ) {
        if( it.HasMoreElements() )
            show();
        return T_Parent::Display( it, parent, at );
    }

    template< typename Iterator >
    ValuedListItem* DisplayList( const Iterator& from, const Iterator& to ) {
        if( from != to )
            show();
        return T_Parent::Display( from, to, this, (ValuedListItem*)( firstChild() ) );
    };

    template< typename Iterator >
    ValuedListItem* DisplayList( const Iterator& from, const Iterator& to, Q3ListViewItem* parent ) {
        if( from != to )
            show();
        return T_Parent::Display( from, to, parent, (ValuedListItem*)( parent->firstChild() ) );
    };

    template< typename Iterator >
    ValuedListItem* DisplayList( const Iterator& from, const Iterator& to, Q3ListView* parent, ValuedListItem* at ) {
        if( from != to )
            show();
        return T_Parent::Display( from, to, parent, at );
    };

    template< typename T >
    void Display( const T& element, ValuedListItem* item ) {
        displayer_.Display( element, itemDisplayer_( item ), item );
    }
    //@}

    //! @name Operations
    //@{
    kernel::Displayer_ABC& GetItemDisplayer( ValuedListItem* item ) { return itemDisplayer_( item ); };
    //@}

private:
    //! @name Member data
    //@{
    ConcreteDisplayer& displayer_;
    ListItemDisplayer itemDisplayer_;
    //@}
};

}

#endif // __ListDisplayer_h_
