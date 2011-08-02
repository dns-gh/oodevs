// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MT_ValuedListViewItem_h_
#define __MT_ValuedListViewItem_h_

#include "MT_ListViewItem.h"

// =============================================================================
/** @class  MT_ValuedListViewItem
    @brief  MT_ValuedListViewItem
    @par    Using example
    @code
    MT_ValuedListViewItem;
    @endcode
*/
// Created: APE 2004-04-19
// =============================================================================
template< class T, int N = 0 >
class MT_ValuedListViewItem : public MT_ListViewItem
{

public:
    //! @name Constructors/Destructor
    //@{
    MT_ValuedListViewItem( T value, Q3ListView * parent );
    MT_ValuedListViewItem( T value, Q3ListView * parent, Q3ListViewItem * after );
    MT_ValuedListViewItem( T value, Q3ListView * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    MT_ValuedListViewItem( T value, Q3ListView * parent, Q3ListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );

    MT_ValuedListViewItem( T value, Q3ListViewItem * parent );
    MT_ValuedListViewItem( T value, Q3ListViewItem * parent, Q3ListViewItem * after );
    MT_ValuedListViewItem( T value, Q3ListViewItem * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    MT_ValuedListViewItem( T value, Q3ListViewItem * parent, Q3ListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    virtual ~MT_ValuedListViewItem();
    //@}

    //! @name Operations
    //@{
    T& GetValue();
    int rtti() const;
    //@}

protected:
    //! @name Member data
    //@{
    T value_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: MT_ValuedListViewItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedListViewItem<T, N>::MT_ValuedListViewItem( T value, Q3ListView * parent )
    : MT_ListViewItem( parent )
    , value_         ( value )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedListViewItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedListViewItem<T, N>::MT_ValuedListViewItem( T value, Q3ListView * parent, Q3ListViewItem * after )
    : MT_ListViewItem( parent, after )
    , value_         ( value )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedListViewItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedListViewItem<T, N>::MT_ValuedListViewItem( T value, Q3ListView * parent, QString label1, QString label2 /* = QString::null*/, QString label3 /* = QString::null*/, QString label4 /* = QString::null*/, QString label5 /* = QString::null*/, QString label6 /* = QString::null*/, QString label7 /* = QString::null*/, QString label8 /* = QString::null*/ )
    : MT_ListViewItem( parent, label1, label2, label3, label4, label5, label6, label7, label8 )
    , value_         ( value )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedListViewItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedListViewItem<T, N>::MT_ValuedListViewItem( T value, Q3ListView * parent, Q3ListViewItem * after, QString label1, QString label2 /* = QString::null*/, QString label3 /* = QString::null*/, QString label4 /* = QString::null*/, QString label5 /* = QString::null*/, QString label6 /* = QString::null*/, QString label7 /* = QString::null*/, QString label8 /* = QString::null*/ )
    : MT_ListViewItem( parent, after, label1, label2, label3, label4, label5, label6, label7, label8 )
    , value_         ( value )
{
}

// -----------------------------------------------------------------------------
// Name: MT_ValuedListViewItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedListViewItem<T, N>::MT_ValuedListViewItem( T value, Q3ListViewItem * parent )
    : MT_ListViewItem( parent )
    , value_         ( value )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedListViewItem constructor

// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedListViewItem<T, N>::MT_ValuedListViewItem( T value, Q3ListViewItem * parent, Q3ListViewItem * after )
    : MT_ListViewItem( parent, after )
    , value_         ( value )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedListViewItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedListViewItem<T, N>::MT_ValuedListViewItem( T value, Q3ListViewItem * parent, QString label1, QString label2 /* = QString::null*/, QString label3 /* = QString::null*/, QString label4 /* = QString::null*/, QString label5 /* = QString::null*/, QString label6 /* = QString::null*/, QString label7 /* = QString::null*/, QString label8 /* = QString::null*/ )
    : MT_ListViewItem( parent, label1, label2, label3, label4, label5, label6, label7, label8 )
    , value_         ( value )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedListViewItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedListViewItem<T, N>::MT_ValuedListViewItem( T value, Q3ListViewItem * parent, Q3ListViewItem * after, QString label1, QString label2 /* = QString::null*/, QString label3 /* = QString::null*/, QString label4 /* = QString::null*/, QString label5 /* = QString::null*/, QString label6 /* = QString::null*/, QString label7 /* = QString::null*/, QString label8 /* = QString::null*/ )
    : MT_ListViewItem( parent, after, label1, label2, label3, label4, label5, label6, label7, label8 )
    , value_         ( value )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedListViewItem destructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedListViewItem<T, N>::~MT_ValuedListViewItem()
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedListViewItem::GetValue
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
T& MT_ValuedListViewItem<T, N>::GetValue()
{
    return value_;
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedListViewItem::rtti
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
int MT_ValuedListViewItem<T, N>::rtti() const
{
    return N;
}



#endif // __MT_ValuedListViewItem_h_
