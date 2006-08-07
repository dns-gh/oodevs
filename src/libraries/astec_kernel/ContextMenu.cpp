// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_kernel_pch.h"
#include "ContextMenu.h"
#include <qpopupmenu.h>

// -----------------------------------------------------------------------------
// Name: ContextMenu constructor
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
ContextMenu::ContextMenu()
    : menu_( new QPopupMenu() )
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
void ContextMenu::AddCategory( const std::string& text, int position /*= -1*/ )
{
    if( position < 0 || position >= categories_.size() )
        categories_.push_back( text );
    else
        categories_.insert( categories_.begin() + position, text );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::Clear
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
void ContextMenu::Clear()
{
    menu_->clear();
    insertedCategories_.clear();
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
// Name: ContextMenu::SetItemParameter
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
void ContextMenu::SetItemParameter( int id, int parameter )
{
    menu_->setItemParameter( id, parameter );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertCategory
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
int ContextMenu::InsertCategory( const std::string& category )
{
    int result = 0;
    for( CIT_Categories itCategory = categories_.begin(); itCategory != categories_.end() && *itCategory != category; ++itCategory)
    {
        const std::string& category = *itCategory;
        while( result < insertedCategories_.size() && insertedCategories_.at( result ) == category )
            ++result;
    }
    insertedCategories_.insert( insertedCategories_.begin() + result, category );
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
int ContextMenu::InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member, const QKeySequence& accel /*= 0*/, int id /*= -1*/ )
{
    return menu_->insertItem( text, receiver, member, accel, id, InsertCategory( category ) );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertItem
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
int ContextMenu::InsertItem( const std::string& category, const QString& text, int id /*= -1*/ )
{
    return menu_->insertItem( text, id, InsertCategory( category ) );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertItem
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
int ContextMenu::InsertItem( const std::string& category, const QString& text, QPopupMenu* popup, int id /*= -1*/ )
{
    return menu_->insertItem( text, popup, id, InsertCategory( category ) );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertItem
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
int ContextMenu::InsertItem( const std::string& category, QWidget* widget, int id /*= -1*/ )
{
    return menu_->insertItem( widget, id, InsertCategory( category ) );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::Popup
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
void ContextMenu::Popup( const QPoint& where )
{
    if( !insertedCategories_.empty() )
    {
        InsertSeparators();
        menu_->popup( where );
    }
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertSeparators
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
void ContextMenu::InsertSeparators()
{
    std::string previous;
    for( IT_Categories it = insertedCategories_.begin(); it != insertedCategories_.end(); ++it )
    {
        const std::string current = *it;
        if( !previous.empty() && previous != current )
        {
            const unsigned index = it - insertedCategories_.begin();
            menu_->insertSeparator( index );
            it = insertedCategories_.insert( it, "__separator__" );
            ++it;
        }
        previous = current;
    };
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::operator QWidget*
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
ContextMenu::operator QWidget*() const
{
    return menu_;
}
