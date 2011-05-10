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
#include "clients_kernel/ActionController.h"
#include "clients_kernel/SafePointer.h"

#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Intelligence_ABC.h"

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
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
    bool Holds( T* value ) const {
        return IsA< T >() && GetValue< T >() == value;
    }
    template< typename T >
    bool IsA() const;
    template< typename T >
    T* GetValue() const;
    template< typename T >
    void SetValue( T* value );
    template< typename T >
    void Set( T* value, QString label1 = QString::null, QString label2 = QString::null );
    template< typename T >
    void Set( T* value, const std::string& label1 );
    template< typename T >
    void SetNamed( const T& value );
    void SetToolTip( const QString& text );

    int rtti() const;
    QString GetToolTip() const;

    void Select( kernel::ActionController& actions );
    void ContextMenu( kernel::ActionController& actions, const QPoint& where );
    void Activate( kernel::ActionController& actions );
    //@}

protected:
    //! @name Member data
    //@{
    ValueContainer_ABC* container_;
    QString toolTip_;
    //@}
};

// =============================================================================
/** @function  FindItem
    @brief  Search for a given item in a list
*/
// Created: APE 2004-04-19
// =============================================================================
template< typename T >
ValuedListItem* FindItem( T* searched, QListViewItem* root )
{
    if( ! root )
        return 0;

    QListViewItemIterator it( root );
    while( it.current() )
    {
        ValuedListItem* item = static_cast< ValuedListItem* >( it.current() );
        if( item->Holds( searched ) )
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
ValuedListItem* FindItem( const kernel::SafePointer< T >& searched, QListViewItem* root )
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
ValuedListItem* FindSibling( T* searched, QListViewItem* child )
{
    while( child )
    {
        ValuedListItem* item = static_cast< ValuedListItem* >( child );
        if( item->Holds( searched ) )
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
ValuedListItem* FindChild( T* searched, QListViewItem* root )
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
    virtual const type_info& typeinfo() const = 0;
    virtual void Select( kernel::ActionController& actions ) = 0;
    virtual void ContextMenu( kernel::ActionController& actions, const QPoint& where ) = 0;
    virtual void Activate( kernel::ActionController& actions ) = 0;
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
    ValueContainer( T* value ) : value_( value ) {};
    virtual const type_info& typeinfo() const {
        return typeid( T );
    }
    T* GetValue() const {
        return value_;
    };
    void SetValue( T* value ) {
        value_ = value;
    }
    virtual void Select( kernel::ActionController& actions ) {
        actions.Select( *value_ );
    };
    virtual void ContextMenu( kernel::ActionController& actions, const QPoint& where ) {
        actions.ContextMenu( *value_, where );
    }
    void Activate( kernel::ActionController& actions ) {
        actions.Activate( *value_ );
    }
private:
    T* value_;
};

template< >
class ValueContainer< const kernel::Entity_ABC > : public ValueContainer_ABC
{
public:
    ValueContainer( const kernel::Entity_ABC* value ) : value_( value ) {};
    virtual const type_info& typeinfo() const {
        return typeid( const kernel::Entity_ABC );
    }
    kernel::Entity_ABC const* GetValue() const {
        return value_;
    };
    void SetValue( kernel::Entity_ABC const* value ) {
        value_ = value;
    }
    virtual void Select( kernel::ActionController& actions );
    virtual void ContextMenu( kernel::ActionController& actions, const QPoint& where );
    void Activate( kernel::ActionController& actions );
private:
    const kernel::Entity_ABC* value_;
};

template< >
class ValueContainer< const kernel::Object_ABC > : public ValueContainer< const kernel::Entity_ABC >
{
public:
    ValueContainer( kernel::Object_ABC const* value )
        : ValueContainer< const kernel::Entity_ABC >( value ), value_( value ) {};
    virtual const type_info& typeinfo() const {
        return typeid( const kernel::Object_ABC );
    }
    kernel::Object_ABC const* GetValue() const {
        return value_;
    };
    void SetValue( kernel::Object_ABC const* value ) {
        value_ = value;
        ValueContainer< const kernel::Entity_ABC >::SetValue( value );
    }
private:
    const kernel::Object_ABC* value_;
};

template< >
class ValueContainer< const kernel::Population_ABC > : public ValueContainer< const kernel::Entity_ABC >
{
public:
    ValueContainer( kernel::Population_ABC const* value )
        : ValueContainer< const kernel::Entity_ABC >( value ), value_( value ) {};
    virtual const type_info& typeinfo() const {
        return typeid( const kernel::Population_ABC );
    }
    kernel::Population_ABC const* GetValue() const {
        return value_;
    };
    void SetValue( kernel::Population_ABC const* value ) {
        value_ = value;
        ValueContainer< const kernel::Entity_ABC >::SetValue( value );
    }
private:
    const kernel::Population_ABC* value_;
};

template< >
class ValueContainer< const kernel::Intelligence_ABC > : public ValueContainer< const kernel::Entity_ABC >
{
public:
    ValueContainer( kernel::Intelligence_ABC const* value )
        : ValueContainer< const kernel::Entity_ABC >( value ), value_( value ) {};
    virtual const type_info& typeinfo() const {
        return typeid( const kernel::Intelligence_ABC );
    }
    kernel::Intelligence_ABC const* GetValue() const {
        return value_;
    };
    void SetValue( kernel::Intelligence_ABC const* value ) {
        value_ = value;
        ValueContainer< const kernel::Entity_ABC >::SetValue( value );
    }
private:
    const kernel::Intelligence_ABC* value_;
};

template< >
class ValueContainer< const kernel::Inhabitant_ABC > : public ValueContainer< const kernel::Entity_ABC >
{
public:
    ValueContainer( kernel::Inhabitant_ABC const* value )
        : ValueContainer< const kernel::Entity_ABC >( value ), value_( value ) {};
    virtual const type_info& typeinfo() const {
        return typeid( const kernel::Inhabitant_ABC );
    }
    kernel::Inhabitant_ABC const* GetValue() const {
        return value_;
    };
    void SetValue( kernel::Inhabitant_ABC const* value ) {
        value_ = value;
        ValueContainer< const kernel::Entity_ABC >::SetValue( value );
    }
private:
    const kernel::Inhabitant_ABC* value_;
};


// -----------------------------------------------------------------------------
// Name: ValuedListItem::IsA
// Created: AGE 2005-09-15
// -----------------------------------------------------------------------------
template< typename T >
bool ValuedListItem::IsA() const
{
    return container_ && container_->typeinfo() == typeid( T );
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem::GetValue
// Created: AGE 2005-09-15
// -----------------------------------------------------------------------------
template< typename T >
T* ValuedListItem::GetValue() const
{
    if( ! IsA< T >() )
        throw std::runtime_error( std::string( "Value is not of the requested type : " ) + typeid( container_ ).name() + " does not hold a " + typeid( T ).name() );
    return static_cast< ValueContainer< T >* >( container_ )->GetValue();
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem::SetValue
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
template< typename T >
void ValuedListItem::SetValue( T* value )
{
    if( ! IsA< T >() ) {
        delete container_;
        container_ = new ValueContainer< T >( value );
    }
    else
        static_cast< ValueContainer< T >* >( container_ )->SetValue( value );
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem::Set
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
template< typename T >
void ValuedListItem::Set( T* value, QString label1 /*= QString::null*/, QString label2 /*= QString::null*/ )
{
    SetValue( value );
    setText( 0, label1 );
    setText( 1, label2 );
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem::Set
// Created: AGE 2008-02-28
// -----------------------------------------------------------------------------
template< typename T >
void ValuedListItem::Set( T* value, const std::string& label1 )
{
    SetValue( value );
    setText( 0, label1.c_str() );
}

// -----------------------------------------------------------------------------
// Name: ValuedListItem::SetNamed
// Created: AGE 2006-08-23
// -----------------------------------------------------------------------------
template< typename T >
void ValuedListItem::SetNamed( const T& value )
{
    Set( &value, value.GetName() );
}

}

#endif // __ValuedListItem_h_
