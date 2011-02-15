// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ItemFactory_ABC_h_
#define __ItemFactory_ABC_h_

#include <boost/noncopyable.hpp>

namespace gui
{
    class ValuedListItem;
    class RichLabel;

// =============================================================================
/** @class  ItemFactory_ABC
    @brief  ItemFactory_ABC
*/
// Created: AGE 2006-05-11
// =============================================================================
class ItemFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ItemFactory_ABC() {};
    virtual ~ItemFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual ValuedListItem* CreateItem( QListView * parent ) = 0;
    virtual ValuedListItem* CreateItem( QListViewItem * parent ) = 0;
    virtual ValuedListItem* CreateItem( QListView * parent, QListViewItem * after ) = 0;
    virtual ValuedListItem* CreateItem( QListViewItem * parent, QListViewItem * after ) = 0;

    virtual RichLabel*      CreateLabel( QWidget* parent = 0, const char* name = 0 ) = 0;
    virtual RichLabel*      CreateLabel( const QString& text, QWidget* parent = 0, const char* name = 0 ) = 0;
    virtual RichLabel*      CreateLabel( const QString& text, bool required, QWidget* parent = 0, const char* name = 0 ) = 0;
    //@}
};

}

#endif // __ItemFactory_ABC_h_
