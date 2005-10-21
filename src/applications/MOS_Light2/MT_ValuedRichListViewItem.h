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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_ValuedRichListViewItem.h $
// $Author: Ape $
// $Modtime: 10/09/04 17:52 $
// $Revision: 1 $
// $Workfile: MT_ValuedRichListViewItem.h $
//
// *****************************************************************************

#ifndef __MT_ValuedRichListViewItem_h_
#define __MT_ValuedRichListViewItem_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MT_RichListViewItem.h"


// =============================================================================
/** @class  MT_ValuedRichListViewItem
    @brief  MT_ValuedRichListViewItem
    @par    Using example
    @code
    MT_ValuedRichListViewItem;
    @endcode
*/
// Created: APE 2004-04-19
// =============================================================================
template< class T, int N = 0 >
class MT_ValuedRichListViewItem : public MT_RichListViewItem
{
    MT_COPYNOTALLOWED( MT_ValuedRichListViewItem );

public:
    //! @name Constructors/Destructor
    //@{
    MT_ValuedRichListViewItem( T value, QListView * parent );
    MT_ValuedRichListViewItem( T value, QListView * parent, QListViewItem * after );
    MT_ValuedRichListViewItem( T value, QListView * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    MT_ValuedRichListViewItem( T value, QListView * parent, QListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );

    MT_ValuedRichListViewItem( T value, QListViewItem * parent );
    MT_ValuedRichListViewItem( T value, QListViewItem * parent, QListViewItem * after );
    MT_ValuedRichListViewItem( T value, QListViewItem * parent, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    MT_ValuedRichListViewItem( T value, QListViewItem * parent, QListViewItem * after, QString label1, QString label2 = QString::null, QString label3 = QString::null, QString label4 = QString::null, QString label5 = QString::null, QString label6 = QString::null, QString label7 = QString::null, QString label8 = QString::null );
    virtual ~MT_ValuedRichListViewItem();
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
// Name: MT_ValuedRichListViewItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedRichListViewItem<T, N>::MT_ValuedRichListViewItem( T value, QListView * parent )
    : MT_RichListViewItem( parent )
    , value_         ( value )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedRichListViewItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedRichListViewItem<T, N>::MT_ValuedRichListViewItem( T value, QListView * parent, QListViewItem * after )
    : MT_RichListViewItem( parent, after )
    , value_         ( value )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedRichListViewItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedRichListViewItem<T, N>::MT_ValuedRichListViewItem( T value, QListView * parent, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
    : MT_RichListViewItem( parent, label1, label2, label3, label4, label5, label6, label7, label8 )
    , value_         ( value )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedRichListViewItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedRichListViewItem<T, N>::MT_ValuedRichListViewItem( T value, QListView * parent, QListViewItem * after, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
    : MT_RichListViewItem( parent, after, label1, label2, label3, label4, label5, label6, label7, label8 )
    , value_         ( value )
{
}

// -----------------------------------------------------------------------------
// Name: MT_ValuedRichListViewItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedRichListViewItem<T, N>::MT_ValuedRichListViewItem( T value, QListViewItem * parent )
    : MT_RichListViewItem( parent )
    , value_         ( value )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedRichListViewItem constructor

// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedRichListViewItem<T, N>::MT_ValuedRichListViewItem( T value, QListViewItem * parent, QListViewItem * after )
    : MT_RichListViewItem( parent, after )
    , value_         ( value )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedRichListViewItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedRichListViewItem<T, N>::MT_ValuedRichListViewItem( T value, QListViewItem * parent, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
    : MT_RichListViewItem( parent, label1, label2, label3, label4, label5, label6, label7, label8 )
    , value_         ( value )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedRichListViewItem constructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedRichListViewItem<T, N>::MT_ValuedRichListViewItem( T value, QListViewItem * parent, QListViewItem * after, QString label1, QString label2 /*= QString::null*/, QString label3 /*= QString::null*/, QString label4 /*= QString::null*/, QString label5 /*= QString::null*/, QString label6 /*= QString::null*/, QString label7 /*= QString::null*/, QString label8 /*= QString::null*/ )
    : MT_RichListViewItem( parent, after, label1, label2, label3, label4, label5, label6, label7, label8 )
    , value_         ( value )
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedRichListViewItem destructor
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
MT_ValuedRichListViewItem<T, N>::~MT_ValuedRichListViewItem()
{
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedRichListViewItem::GetValue
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
T& MT_ValuedRichListViewItem<T, N>::GetValue()
{
    return value_;
}


// -----------------------------------------------------------------------------
// Name: MT_ValuedRichListViewItem::rtti
// Created: APE 2004-04-19
// -----------------------------------------------------------------------------
template< class T, int N >
int MT_ValuedRichListViewItem<T, N>::rtti() const
{
    return N;
}


#endif // __MT_ValuedRichListViewItem_h_
