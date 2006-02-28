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
             ListDisplayer( QWidget* parent, ConcreteDisplayer& displayer )
                 : T_Parent( parent, *this )
                 , displayer_( displayer )
             {
                setSorting( -1 );
                setResizeMode( QListView::LastColumn );
                setAllColumnsShowFocus( true );
             };
    virtual ~ListDisplayer() {};
    //@}

    //! @name Operations
    //@{
    ListDisplayer& AddColumn( const char* column ) {
        addColumn( tr( column ) );
        itemDisplayer_.AddColumn( column );
        return *this;
    }
    template< typename Element >
    ValuedListItem* DisplayList( Iterator< const Element& > it ) {
        if( it.HasMoreElements() )
            show();
        return T_Parent::Display( it, this, (ValuedListItem*)( firstChild() ) );
    }
    template< typename Element >
    ValuedListItem* DisplayList( Iterator< const Element& > it, QListViewItem* parent ) {
        if( it.HasMoreElements() )
            show();
        return T_Parent::Display( it, parent, (ValuedListItem*)( parent->firstChild() ) );
    }

    template< typename Iterator >
    ValuedListItem* DisplayList( const Iterator& from, const Iterator& to ) {
        if( from != to )
            show();
        return T_Parent::Display( from, to, this, (ValuedListItem*)( firstChild() ) );
    };

    template< typename Iterator >
    ValuedListItem* DisplayList( const Iterator& from, const Iterator& to, QListViewItem* parent ) {
        if( from != to )
            show();
        return T_Parent::Display( from, to, (ValuedListItem*)( parent->firstChild() ) );
    };

    template< typename T >
    void Display( const T& element, ValuedListItem* item ) {
        displayer_.Display( element, itemDisplayer_( item ), item );
    }
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ListDisplayer( const ListDisplayer& );            //!< Copy constructor
    ListDisplayer& operator=( const ListDisplayer& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ConcreteDisplayer& displayer_;
    ListItemDisplayer itemDisplayer_;
    //@}
};

#endif // __ListDisplayer_h_
