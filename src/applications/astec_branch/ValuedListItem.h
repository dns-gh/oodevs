// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-19 $
// $Archive: /tools/MT/build/libraries/MT_Qt/src/ValuedListItem.h $
// $Author: Ape $
// $Modtime: 12/05/04 9:58 $
// $Revision: 2 $
// $Workfile: ValuedListItem.h $
//
// *****************************************************************************

#ifndef __ValuedListItem_h_
#define __ValuedListItem_h_

#include <qlistview.h>
#include "ListItemRtti.h"

class ValueContainer_ABC;

// =============================================================================
/** @class  ValuedListItem
    @brief  Valued list view item
*/
// Created: APE 2004-04-19
// =============================================================================
class ValuedListItem : public QListViewItem
{

public:
    //! @name Constructors/Destructor
    //@{
    ValuedListItem( QListView * parent );
    ValuedListItem( QListView * parent, QListViewItem * after );
    ValuedListItem( QListView * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    ValuedListItem( QListView * parent, QListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );

    ValuedListItem( QListViewItem * parent );
    ValuedListItem( QListViewItem * parent, QListViewItem * after );
    ValuedListItem( QListViewItem * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    ValuedListItem( QListViewItem * parent, QListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );

    template< typename T >
    ValuedListItem( const T& value, QListView * parent );
    template< typename T >
    ValuedListItem( const T& value, QListView * parent, QListViewItem * after );
    template< typename T >
    ValuedListItem( const T& value, QListView * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    template< typename T >
    ValuedListItem( const T& value, QListView * parent, QListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );

    template< typename T >
    ValuedListItem( const T& value, QListViewItem * parent );
    template< typename T >
    ValuedListItem( const T& value, QListViewItem * parent, QListViewItem * after );
    template< typename T >
    ValuedListItem( const T& value, QListViewItem * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    template< typename T >
    ValuedListItem( const T& value, QListViewItem * parent, QListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    virtual ~ValuedListItem();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    bool IsA() const;
    template< typename T >
    const T& GetValue();

    int rtti() const {
        return 1000;
    }
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ValuedListItem( const ValuedListItem& );            //!< Copy constructor
    ValuedListItem& operator=( const ValuedListItem& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    ValueContainer_ABC* container_;
    //@}
};

// =============================================================================
/** @function  FindItem
    @brief  Search for a given item in a list
*/
// Created: APE 2004-04-19
// =============================================================================
template< typename T >
ValuedListItem* FindItem( const T& searched, QListViewItem* root )
{
    if( ! root )
        return 0;

    QListViewItemIterator it( root );
    while( it.current() )
    {
        ValuedListItem* item = static_cast< ValuedListItem* >( it.current() );
        if( item->IsA< T >() && item->GetValue< T >() == searched )
            return item;
        ++it;
    }
    return 0;
}

// =============================================================================
/** @function  FindChild
    @brief  Search for a given item in the children of root
*/
// Created: APE 2004-04-19
// =============================================================================
template< typename T >
ValuedListItem* FindChild( const T& searched, QListViewItem* root )
{
    if( ! root )
        return 0;
    QListViewItem* child = root->firstChild();
    while( child )
    {
        ValuedListItem* item = static_cast< ValuedListItem* >( child );
        if( item->IsA< T >() && item->GetValue< T >() == searched )
            return item;
        child = child->nextSibling();
    }
    return 0;
}

// =============================================================================
/** @class  ValueContainer_ABC
    @brief  Value container definition
*/
// Created: APE 2004-04-19
// =============================================================================
class ValueContainer_ABC
{
public:
    virtual int rtti() const = 0;
};

// =============================================================================
/** @class  ValueContainer
    @brief  Value container implementation
*/
// Created: APE 2004-04-19
// =============================================================================
template< typename T >
class ValueContainer : public ValueContainer_ABC
{
public:
    ValueContainer( const T& value ) : value_( value ) {};
    virtual int rtti() const {
        return ListItemRtti< T >::rtti;
    }
    const T& GetValue() {
        return value_;
    };
private:
    T value_;
};

// -----------------------------------------------------------------------------
// Name: ValuedListItem::IsA
// Created: AGE 2005-09-15
// -----------------------------------------------------------------------------
template< typename T >
bool ValuedListItem::IsA() const
{
    return container_ && container_->rtti() == ListItemRtti< T >::rtti;
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem::GetValue
// Created: AGE 2005-09-15
// -----------------------------------------------------------------------------
template< typename T >
const T& ValuedListItem::GetValue()
{
    if( ! IsA< T >() )
        throw std::runtime_error( std::string( "Value is not of the requested type : " ) + typeid( container_ ).name() + " does not hold a " + typeid( T ).name() );
    return static_cast< ValueContainer< T >*>( container_ )->GetValue();
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< typename T >
ValuedListItem::ValuedListItem( const T& value, QListView * parent )
: QListViewItem( parent )
, container_( new ValueContainer< T >( value ) )
{
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< typename T >
ValuedListItem::ValuedListItem( const T& value, QListView * parent, QListViewItem * after )
: QListViewItem( parent, after )
, container_( new ValueContainer< T >( value ) )
{
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< typename T >
ValuedListItem::ValuedListItem( const T& value, QListView * parent, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
: QListViewItem( parent, label1, label2, label3, label4, label5, label6, label7, label8 )
, container_( new ValueContainer< T >( value ) )
{
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< typename T >
ValuedListItem::ValuedListItem( const T& value, QListView * parent, QListViewItem * after, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
: QListViewItem( parent, after, label1, label2, label3, label4, label5, label6, label7, label8 )
, container_( new ValueContainer< T >( value ) )
{
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< typename T >
ValuedListItem::ValuedListItem( const T& value, QListViewItem * parent )
: QListViewItem( parent )
, container_( new ValueContainer< T >( value ) )
{
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< typename T >
ValuedListItem::ValuedListItem( const T& value, QListViewItem * parent, QListViewItem * after )
: QListViewItem( parent, after )
, container_( new ValueContainer< T >( value ) )
{
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< typename T >
ValuedListItem::ValuedListItem( const T& value, QListViewItem * parent, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
: QListViewItem( parent, label1, label2, label3, label4, label5, label6, label7, label8 )
, container_( new ValueContainer< T >( value ) )
{
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< typename T >
ValuedListItem::ValuedListItem( const T& value, QListViewItem * parent, QListViewItem * after, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
: QListViewItem( parent, after, label1, label2, label3, label4, label5, label6, label7, label8 )
, container_( new ValueContainer< T >( value ) )
{
}


#endif // __ValuedListItem_h_
