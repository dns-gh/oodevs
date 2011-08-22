// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "ContextMenu.h"
#pragma warning( push, 0 )
#include <Qt3Support/q3popupmenu.h>
#pragma warning( pop )

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ContextMenu constructor
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
ContextMenu::ContextMenu()
    : menu_( new Q3PopupMenu() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ContextMenu destructor
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
ContextMenu::~ContextMenu()
{
    delete menu_;
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::AddCategory
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
void ContextMenu::AddCategory( const std::string& text )
{
    categories_.push_back( text );
    menus_[ text ] = 0;
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::Clear
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
void ContextMenu::Clear()
{
    menu_->clear();
    categories_.clear();
    menus_.clear();
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::SetItemEnabled
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
void ContextMenu::SetItemEnabled( int id, bool enable )
{
    menu_->setItemEnabled( id, enable );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::SetChecked
// Created: SBO 2007-04-24
// -----------------------------------------------------------------------------
void ContextMenu::SetChecked( int id, bool checked )
{
    menu_->setItemChecked( id, checked );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::SetItemParameter
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
void ContextMenu::SetItemParameter( int id, int parameter )
{
    menu_->setItemParameter( id, parameter );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::SetPixmap
// Created: SBO 2008-10-20
// -----------------------------------------------------------------------------
void ContextMenu::SetPixmap( int id, const QPixmap& pixmap )
{
    menu_->changeItem( id, pixmap, menu_->text( id ) );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertCategory
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
int ContextMenu::InsertCategory( const std::string& category )
{
    T_OrderedCategories::iterator it = std::find( categories_.begin(), categories_.end(), category );
    if( it == categories_.end() )
        AddCategory( category );
    int result = 0;
    for( T_OrderedCategories::const_iterator it = categories_.begin(); it != categories_.end(); ++it )
    {
        T_CategoryMenus::const_iterator itMenu = menus_.find( *it );
        if( itMenu != menus_.end() )
        {
            result += itMenu->second == 0 ? 0 : itMenu->second->count();
            if( *it == category )
                break;
        }
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertItem
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
int ContextMenu::InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member )
{
    return menu_->insertItem( text, receiver, member, 0, -1, InsertCategory( category ) );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertItem
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
int ContextMenu::InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member, const QKeySequence& accel /* = 0*/, int id /* = -1*/ )
{
    return menu_->insertItem( text, receiver, member, accel, id, InsertCategory( category ) );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertItem
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
int ContextMenu::InsertItem( const std::string& category, const QString& text, int id /* = -1*/ )
{
    return menu_->insertItem( text, id, InsertCategory( category ) );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertItem
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
int ContextMenu::InsertItem( const std::string& category, const QString& text, Q3PopupMenu* popup, int id /* = -1*/ )
{
    InsertCategory( category );
    popup->setTitle( text );
    menu_->insertMenu( 0, popup );
    return -1;
    //return menu_->insertItem( text, popup, id, InsertCategory( category ) );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::SubMenu
// Created: SBO 2006-08-10
// -----------------------------------------------------------------------------
Q3PopupMenu* ContextMenu::SubMenu( const std::string& category, const QString& text )
{
    T_OrderedCategories::iterator it = std::find( categories_.begin(), categories_.end(), category );
    if( it == categories_.end() )
        AddCategory( category );
    T_CategoryMenus::iterator itMenu = menus_.find( category );
    if( itMenu == menus_.end() )
        return 0;
    if( itMenu->second == 0 )
        itMenu->second = new Q3PopupMenu( menu_ );
    InsertItem( category, text, itMenu->second );
    return itMenu->second;
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::Popup
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
void ContextMenu::Popup( const QPoint& where )
{
    if( menu_ && menu_->count() > 0 )
        menu_->popup( where );
}
/*
// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertSeparators
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
void ContextMenu::InsertSeparators()
{

}
*/
// -----------------------------------------------------------------------------
// Name: ContextMenu::operator QWidget*
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
ContextMenu::operator QWidget*() const
{
    return menu_;
}
