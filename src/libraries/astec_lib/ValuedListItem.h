// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ValuedListItem_h_
#define __ValuedListItem_h_

#include "RichListItem.h"
#include "ListItemRtti.h"
#include "ActionController.h"
#include "SafePointer.h"

class ValueContainer_ABC;

// =============================================================================
/** @class  ValuedListItem
    @brief  Valued list view item
*/
// Created: APE 2004-04-19
// =============================================================================
class ValuedListItem : public RichListItem
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ValuedListItem( QListView* parent );
             ValuedListItem( QListView* parent, QListViewItem* after );
    explicit ValuedListItem( QListViewItem* parent );
             ValuedListItem( QListViewItem* parent, QListViewItem* after );
    virtual ~ValuedListItem();
    //@}

    //! @name Operations
    //@{
    template< typename T >
    bool Holds( const T& value ) const {
        return IsA< T >() && GetValue< T >() == value;
    }
    template< typename T >
    bool IsA() const;
    template< typename T >
    const T& GetValue() const;
    template< typename T >
    void SetValue( const T& value );
    template< typename T >
    void Set( const T& value, QString label1 = QString::null, QString label2 = QString::null );

    int rtti() const;

    void Select( ActionController& actions );
    void ContextMenu( ActionController& actions, const QPoint& where );
    void Activate( ActionController& actions );
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

// -----------------------------------------------------------------------------
// Name: ValuedListItem::FindItem
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
template< typename T >
ValuedListItem* FindItem( const SafePointer< T >& searched, QListViewItem* root )
{
    return FindItem( (const T*)searched, root );
}


// =============================================================================
/** @function  FindSibling
    @brief  Search for a given item in the children of root
*/
// Created: APE 2004-04-19
// =============================================================================
template< typename T >
ValuedListItem* FindSibling( const T& searched, QListViewItem* child )
{
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
    return FindSibling( searched, root->firstChild() );
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
    virtual void Select( ActionController& actions ) = 0;
    virtual void ContextMenu( ActionController& actions, const QPoint& where ) = 0;
    virtual void Activate( ActionController& actions ) = 0;
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
    const T& GetValue() const {
        return value_;
    };
    void SetValue( const T& value ) {
        value_ = value;
    }
    virtual void Select( ActionController& actions ) {
        actions.Select( *value_ );
    };
    virtual void ContextMenu( ActionController& actions, const QPoint& where ) {
        actions.ContextMenu( *value_, where );
    }
    void Activate( ActionController& actions ) {
        actions.Activate( *value_ );
    }
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
const T& ValuedListItem::GetValue() const
{
    if( ! IsA< T >() )
        throw std::runtime_error( std::string( "Value is not of the requested type : " ) + typeid( container_ ).name() + " does not hold a " + typeid( T ).name() );
    return static_cast< ValueContainer< T >*>( container_ )->GetValue();
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem::SetValue
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
template< typename T >
void ValuedListItem::SetValue( const T& value )
{
    if( ! IsA< T >() ) {
        delete container_;
        container_ = new ValueContainer< T >( value );
    }
    static_cast< ValueContainer< T >*>( container_ )->SetValue( value );
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem::Set
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
template< typename T >
void ValuedListItem::Set( const T& value, QString label1 /*= QString::null*/, QString label2 /*= QString::null*/ )
{
    SetValue( value );
    setText( 0, label1 );
    setText( 1, label2 );
}

#endif // __ValuedListItem_h_
