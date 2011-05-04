// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include <qtable.h>

namespace gui
{

// -----------------------------------------------------------------------------
// Name: ValuedComboBox constructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< typename T >
ValuedComboBox<T>::ValuedComboBox( QWidget* parent, const char* name )
    : QComboBox( false, parent, name )
    , sorting_( false )
{
    if( parent->inherits( "QTable" ) ) // $$$$ SBO 2006-10-30: emulate QComboTableItem
        QObject::connect( this, SIGNAL( activated( int ) ), (QTable*)parent, SLOT( doValueChanged() ) );
}


// -----------------------------------------------------------------------------
// Name: ValuedComboBox destructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< typename T >
ValuedComboBox<T>::~ValuedComboBox()
{
    // NOTHING
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
// Name: ValuedComboBox::AddItem
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
//template< typename T >
//void ValuedComboBox<T>::AddItem( const QPixmap& pixmap, const T& value )
//{
//    insertItem( pixmap, values_.size() );
//    values_.push_back( value );
//}


// -----------------------------------------------------------------------------
// Name: ValuedComboBox::AddItem
/** @param  pixmap 
    @param  strLabel 
    @param  value 
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
//template< typename T >
//void ValuedComboBox<T>::AddItem( const QPixmap& pixmap, const QString& strLabel, const T& value )
//{
//    if( !sorting_ )
//    {
//        insertItem( pixmap, strLabel, values_.size() );
//        values_.push_back( value );
//    }
//    else
//    {
//        // sorted insertion
//        int nInsertPos = 0;
//        for( int i = 0; i < count(); ++i )
//        {
//            if( strLabel.compare( text( i ) ) < 0 )
//                break;
//            ++nInsertPos;
//        }
//        insertItem( pixmap, strLabel, nInsertPos );
//        values_.insert( values_.begin() + nInsertPos, value );
//    }
//}


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
// Name: ValuedComboBox::ChangeItem
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
//template< typename T >
//void ValuedComboBox<T>::ChangeItem( const QPixmap& pixmap, const T& value )
//{
//    int n = GetItemIndex( value );
//    if( n == -1 )
//        return;
//    changeItem( pixmap, n );
//}


// -----------------------------------------------------------------------------
// Name: ValuedComboBox::ChangeItem
/** @param  pixmap 
    @param  strLabel 
    @param  value 
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
//template< typename T >
//void ValuedComboBox<T>::ChangeItem( const QPixmap& pixmap, const QString& strLabel, const T& value )
//{
//    int n = GetItemIndex( value );
//    if( n == -1 )
//        return;
//    changeItem( pixmap, strLabel, n );
//}


// -----------------------------------------------------------------------------
// Name: ValuedComboBox::RemoveItem
/** @param  value 
*/
// Created: APE 2004-06-23
// -----------------------------------------------------------------------------
template< typename T >
void ValuedComboBox<T>::RemoveItem( const T& value )
{
    IT_ValueVector it = std::find( values_.begin(), values_.end(), value );
    if( it == values_.end() )
        return;
    removeItem( it - values_.begin() );
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
    IT_ValueVector it = std::find( values_.begin(), values_.end(), value );
    if( it == values_.end() )
        return -1;
    return it - values_.begin();
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
        throw std::runtime_error( __FUNCTION__ );
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
    return values_.size();
}

// -----------------------------------------------------------------------------
// Name: ValuedComboBox::sorting
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
template< typename T >
bool ValuedComboBox<T>::sorting()
{
    return sorting_;
}

// -----------------------------------------------------------------------------
// Name: ValuedComboBox::setSorting
// Created: SBO 2005-09-22
// -----------------------------------------------------------------------------
template< typename T >
void ValuedComboBox<T>::setSorting( bool sorting )
{
    sorting_ = sorting;
}

}