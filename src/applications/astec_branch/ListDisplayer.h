// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
    template< typename Element, typename Parent >
    ValuedListItem* Display( Iterator< const Element& > it, Parent* parent ) {
        return T_Parent::Display( it, parent, (ValuedListItem*)( firstChild() ) );
    }

    template< typename Iterator, typename Parent >
    ValuedListItem* Display( Iterator from, const Iterator& to, Parent* parent ) {
        return T_Parent::Display( from, to, parent, (ValuedListItem*)( firstChild() ) );
    };

    template< typename T >
    void Display( const T& element, ValuedListItem* item ) {
        displayer_.Display( element, itemDisplayer_( item ) );
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
