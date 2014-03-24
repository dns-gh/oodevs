// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "ObjectNameManager.h"

namespace gui
{

// -----------------------------------------------------------------------------
// Name: ValuedComboBox constructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< typename T >
ValuedComboBox<T>::ValuedComboBox( const QString& objectName, QWidget* parent, const char* name )
    : QComboBox( false, parent, name )
    , sorting_( false )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: ValuedComboBox destructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< typename T >
ValuedComboBox<T>::~ValuedComboBox()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}

// -----------------------------------------------------------------------------
// Name: ValuedComboBox::AddItem
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< typename T >
void ValuedComboBox<T>::AddItem( const QString& label, const T& value )
{
    if( !sorting_ )
    {
        insertItem( label, (int)values_.size() );
        values_.push_back( value );
    }
    else
    {
        // sorted insertion
        int nInsertPos = 0;
        for( int i = 0; i < count(); ++i )
        {
            if( label.compare( text( i ) ) < 0 )
                break;
            ++nInsertPos;
        }
        insertItem( label, nInsertPos );
        values_.insert( values_.begin() + nInsertPos, value );
    }
    if( currentItem() == -1 )
        SetCurrentItem( value );
}

// -----------------------------------------------------------------------------
// Name: ValuedComboBox::AddItem
// Created: AGE 2008-02-28
// -----------------------------------------------------------------------------
template< typename T >
void ValuedComboBox< T >::AddItem( const std::string& label, const T& value )
{
    AddItem( QString( label.c_str() ), value );
}

// -----------------------------------------------------------------------------
// Name: ValuedComboBox::ChangeItem
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
template< typename T >
void ValuedComboBox<T>::ChangeItem( const QString& label, const T& value )
{
    int n = GetItemIndex( value );
    if( n == -1 )
        return;
    changeItem( label, n );
}

// -----------------------------------------------------------------------------
// Name: ValuedComboBox::RemoveItem
/** @param  value
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
template< typename T >
void ValuedComboBox<T>::RemoveItem( const T& value )
{
    auto it = std::find( values_.begin(), values_.end(), value );
    if( it == values_.end() )
        return;
    removeItem( static_cast< int >( it - values_.begin() ) );
    values_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: ValuedComboBox::SetCurrentItem
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
template< typename T >
void ValuedComboBox<T>::SetCurrentItem( const T& value )
{
    int n = GetItemIndex( value );
    if( n == -1 )
        return;
    setCurrentItem( n );
}

// -----------------------------------------------------------------------------
// Name: ValuedComboBox::GetItemIndex
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
template< typename T >
int ValuedComboBox<T>::GetItemIndex( const T& value )
{
    auto it = std::find( values_.begin(), values_.end(), value );
    if( it == values_.end() )
        return -1;
    return static_cast< int >( it - values_.begin() );
}

// -----------------------------------------------------------------------------
// Name: ValuedComboBox::GetValue
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< typename T >
T ValuedComboBox<T>::GetValue()
{
    int nIndex = currentItem();
    if( nIndex == -1 || nIndex > int( values_.size() ) )
        throw MASA_EXCEPTION( "Invalid index." );
    return values_[nIndex];
}

// -----------------------------------------------------------------------------
// Name: ValuedComboBox::Clear
// Created: APE 2004-10-15
// -----------------------------------------------------------------------------
template< typename T >
void ValuedComboBox<T>::Clear()
{
    values_.clear();
    QComboBox::clear();
}

// -----------------------------------------------------------------------------
// Name: ValuedComboBox::Count
// Created: SBO 2009-05-07
// -----------------------------------------------------------------------------
template< typename T >
unsigned int ValuedComboBox<T>::Count() const
{
    return static_cast< unsigned int >( values_.size() );
}

// -----------------------------------------------------------------------------
// Name: ValuedComboBox::Sorting
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
template< typename T >
bool ValuedComboBox<T>::Sorting()
{
    return sorting_;
}

// -----------------------------------------------------------------------------
// Name: ValuedComboBox::SetSorting
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
template< typename T >
void ValuedComboBox<T>::SetSorting( bool sorting )
{
    sorting_ = sorting;
}

}