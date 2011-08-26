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
    QAction* separatorAction = menu_->addSeparator();
    categories_.push_back( text );
    separators_[ text ] = separatorAction;
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
    separators_.clear();
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
QAction* ContextMenu::InsertCategory( const std::string& category )
{
    T_OrderedCategories::iterator it = std::find( categories_.begin(), categories_.end(), category );
    if( it == categories_.end() )
        AddCategory( category );
    return separators_[ category ];
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertItem
// Created: AGE 2006-08-07
// -----------------------------------------------------------------------------
QAction* ContextMenu::InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member )
{
    QAction* actionCategory = InsertCategory( category );
    QList<QAction*> actions = menu_->actions();
    for( QList<QAction*>::iterator it = actions.begin(); it != actions.end(); ++it )
        if( *it == actionCategory )
        {
            QAction* action;
            ++it;
            if( it == actions.end() )
                action = menu_->addAction( text, receiver, member );
            else
            {
                action = new QAction(text, menu_);
                QObject::connect( action, SIGNAL(triggered()), receiver, member );
                menu_->insertAction( *it, action );
            }
            return action;
        }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertItem
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
QAction* ContextMenu::InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member, const QKeySequence& accel /* = 0*/, int id /* = -1*/ )
{
    QAction* actionCategory = InsertCategory( category );
    QList<QAction*> actions = menu_->actions();
    for( QList<QAction*>::iterator it = actions.begin(); it != actions.end(); ++it )
        if( *it == actionCategory )
        {
            QAction* action;
            ++it;
            if( it == actions.end() )
                action = menu_->addAction( text, receiver, member );
            else
            {
                action = new QAction(text, menu_);
                QObject::connect( action, SIGNAL(triggered()), receiver, member );
                menu_->insertAction( *it, action );
            }
            action->setShortcut( accel );
            return action;
        }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertItem
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
QAction* ContextMenu::InsertItem( const std::string& category, const QString& text, int id /* = -1*/ )
{
    QAction* actionCategory = InsertCategory( category );
    QList<QAction*> actions = menu_->actions();
    for( QList<QAction*>::iterator it = actions.begin(); it != actions.end(); ++it )
        if( *it == actionCategory )
        {
            QAction* action;
            ++it;
            if( it == actions.end() )
                action = menu_->addAction( text );
            else
            {
                action = new QAction(text, menu_);
                menu_->insertAction( *it, action );
            }
            return action;
        }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertItem
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
QAction* ContextMenu::InsertItem( const std::string& category, const QString& text, Q3PopupMenu* popup, int id /* = -1*/ )
{
    QAction* actionCategory = InsertCategory( category );
    popup->setTitle( text );
    QList<QAction*> actions = menu_->actions();
    for( QList<QAction*>::iterator it = actions.begin(); it != actions.end(); ++it )
        if( *it == actionCategory )
        {
            ++it;
            if( it == actions.end() )
                menu_->addMenu( popup );
            else
                menu_->insertMenu( *it, popup );
            return popup->menuAction();
        }
    return popup->menuAction();
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
        menus_[category] = new Q3PopupMenu( menu_ );
    Q3PopupMenu* result = menus_[category];
    InsertItem( category, text, result );
    return result;
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
