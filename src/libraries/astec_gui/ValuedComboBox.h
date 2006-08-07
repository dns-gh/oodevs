// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ValuedComboBox_h_
#define __ValuedComboBox_h_

#include <qcombobox.h>

// =============================================================================
/** @class  ValuedComboBox
    @brief  Valued ComboBox
*/
// Created: SBO 2006-04-18
// =============================================================================
template< typename T >
class ValuedComboBox : public QComboBox
{

private:
    //! @name Types
    //@{
    typedef std::vector< T >                  T_ValueVector;
    typedef typename T_ValueVector::iterator IT_ValueVector;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ValuedComboBox( QWidget* parent = 0, const char* name = 0 );
    virtual ~ValuedComboBox();
    //@}

    //! @name Accessors
    //@{
    T    GetValue    ();
    bool sorting     ();
    //@}

    //! @name Operations
    //@{
    void AddItem( const QString& label, const T& value );
//    void AddItem( const QPixmap& pixmap, const T& value );
//    void AddItem( const QPixmap& pixmap, const QString& strLabel, const T& value );

    void ChangeItem( const QString& label, const T& value );
//    void ChangeItem( const QPixmap& pixmap, const T& value );
//    void ChangeItem( const QPixmap& pixmap, const QString& strLabel, const T& value );

    void RemoveItem( const T& value );

    int GetItemIndex( const T& value );
    void SetCurrentItem( const T& value );
	void Clear();

    void setSorting( bool sorting );
    //@}

private:
    //! @name Copy constructor/Assignment
    //@{
    ValuedComboBox( const ValuedComboBox& );
    ValuedComboBox& operator=( const ValuedComboBox& );
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    T_ValueVector values_;
    bool          sorting_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: ValuedComboBox constructor
// Created: APE 2004-04-21
// -----------------------------------------------------------------------------
template< typename T >
ValuedComboBox<T>::ValuedComboBox( QWidget* parent, const char* name )
    : QComboBox( false, parent, name )
    , sorting_( false )
{
    // NOTHING
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
        insertItem( label, values_.size() );
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
    assert( nIndex >= 0 && nIndex < (int)values_.size() );
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

#endif // __ValuedComboBox_h_
