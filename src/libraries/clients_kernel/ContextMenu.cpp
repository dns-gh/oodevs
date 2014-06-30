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
#include <boost/assign.hpp>

using namespace kernel;

namespace
{

const std::vector< std::string > baseCategories_ = boost::assign::list_of
    ( "Interface" )
    ( "Order" )
    ( "Command" )
    ( "Helpers" )
    ( "Creation" )
    ( "Target" )
    ( "Parameter" );

}  // namespace

// -----------------------------------------------------------------------------
// Name: ContextMenu constructor
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
ContextMenu::ContextMenu()
    : Q3PopupMenu() //: QMenu()
{
    setSeparatorsCollapsible( true );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu constructor
// Created: ABR 2011-12-29
// -----------------------------------------------------------------------------
ContextMenu::ContextMenu( QWidget* parent )
    : Q3PopupMenu( parent ) //: QMenu( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ContextMenu destructor
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
ContextMenu::~ContextMenu()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::Clear
// Created: AGE 2006-08-04
// -----------------------------------------------------------------------------
void ContextMenu::Clear()
{
    for( CIT_Menus menu = menus_.begin(); menu != menus_.end(); ++menu )
        for( CIT_SubMenus subMenu = menu->second.begin(); subMenu != menu->second.end(); ++subMenu )
        {
            if( QAction* const* vAction = boost::get< QAction* >( &subMenu->second ) )
            {
                QAction* action = *vAction;
                delete action;
                action = 0;
            }
            else if( ContextMenu* const* vMenu = boost::get< ContextMenu* >( &subMenu->second ) )
            {
                ContextMenu* menu = *vMenu;
                menu->Clear();
                delete menu;
                menu = 0;
            }
        }
    menus_.clear();
    this->clear();
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::ConnectSubActions
// Created: ABR 2012-01-06
// -----------------------------------------------------------------------------
void ContextMenu::ConnectSubActions( const QObject* receiver, const char* member, int depth /*= 1*/ )
{
    if( depth == 0 )
        return;
    for( CIT_Menus menu = menus_.begin(); menu != menus_.end(); ++menu )
        for( CIT_SubMenus subMenu = menu->second.begin(); subMenu != menu->second.end(); ++subMenu )
        {
            if( QAction* const* vAction = boost::get< QAction* >( &subMenu->second ) )
                QObject::connect( *vAction, SIGNAL( triggered() ), receiver, member );
            else if( ContextMenu* const* vMenu = boost::get< ContextMenu* >( &subMenu->second ) )
                ConnectSubActions( receiver, member, depth - 1 );
        }
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::FillCategory
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
void ContextMenu::InternalFillMenu( const ContextMenu::CIT_Menus& currentMenu )
{
    QAction* separatorAction = this->addSeparator();
    if( currentMenu->first.separatorText_ )
        separatorAction->setText( currentMenu->first.name_.c_str() );
    for( CIT_SubMenus subMenu = currentMenu->second.begin(); subMenu != currentMenu->second.end(); ++subMenu )
    {
        if( QAction* const* newAction = boost::get< QAction* >( &subMenu->second ) )
            this->addAction( *newAction );
        else if( ContextMenu* const* newMenu = boost::get< ContextMenu* >( &subMenu->second ) )
            this->addMenu( ( *newMenu )->FillMenu() );
        else
            throw MASA_EXCEPTION( "Invalid boost variant value" );
    }
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::FillMenu
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
ContextMenu* ContextMenu::FillMenu()
{
    // Add base categories first
    for( auto it = baseCategories_.begin(); it != baseCategories_.end(); ++it )
        for( CIT_Menus menu = menus_.begin(); menu != menus_.end(); ++menu )
            if( menu->first.name_ == *it )
            {
                InternalFillMenu( menu );
                break;
            }
    // Add on the fly categories after
    for( CIT_Menus menu = menus_.begin(); menu != menus_.end(); ++menu )
        if( std::find( baseCategories_.begin(), baseCategories_.end(), menu->first.name_ ) == baseCategories_.end() )
            InternalFillMenu( menu );
    return this;
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::GetCategoryCreateIFN
// Created: ABR 2012-01-03
// -----------------------------------------------------------------------------
ContextMenu::T_SubMenus& ContextMenu::GetCategoryCreateIFN( const std::string& text, bool separatorText /*= false*/ )
{
    Category category( text, separatorText );
    CIT_Menus itMenu = menus_.find( category );
    if( itMenu == menus_.end() )
        menus_[ category ] = T_SubMenus();
    return menus_[ category ];
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertVariant
// Created: ABR 2012-01-06
// -----------------------------------------------------------------------------
ContextMenu::T_MenuVariant ContextMenu::InsertVariant( const std::string& category, T_MenuVariant& variant, bool separatorText /*= false*/, int index /*= -1*/ )
{
    ContextMenu::T_MenuVariant result;
    if( QAction* const* newAction = boost::get< QAction* >( &variant ) )
        result = InsertAction( category, *newAction, separatorText, index );
    else if( ContextMenu* const* newMenu = boost::get< ContextMenu* >( &variant ) )
        result = SubMenu( category, *newMenu, separatorText, index );
    else
        throw MASA_EXCEPTION( "Invalid boost variant value" );
    return result;
}

namespace
{

std::pair< bool, QString > GetVariantTitle( ContextMenu::T_MenuVariant variant )
{
    const QString name = boost::get< QAction* >( &variant )
        ? boost::get< QAction* >( variant )->text()
        : boost::get< ContextMenu* >( variant )->title();
    return std::make_pair( boost::get< QAction* >( &variant ) != 0, name );
}

} // namespace

ContextMenu::T_MenuVariant ContextMenu::InsertMenuVariant(
        const std::string& category, T_MenuVariant variant, bool separatorText,
        int index )
{
    T_SubMenus& subMenus = GetCategoryCreateIFN( category, separatorText );

    // Deduplicate entries by (type, title)
    const auto title = GetVariantTitle( variant );
    for( auto it = subMenus.begin(); it != subMenus.end(); ++it )
    {
        if( GetVariantTitle( it->second ) == title )
        {
            // Delete current instance and return the other one
            if( auto action = boost::get< QAction* >( &variant ) )
                delete *action;
            else
                delete boost::get< ContextMenu* >( variant );
            return it->second;
        }
    }
    const int pos = index == -1 ? static_cast< int >( subMenus.size() ) : index;
    subMenus.insert( std::make_pair( pos, variant ) );
    return variant;
}


// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertAction
// Created: ABR 2012-01-05
// -----------------------------------------------------------------------------
QAction* ContextMenu::InsertAction( const std::string& category, QAction* action, bool separatorText /* = false */, int index /* = -1*/ )
{
    const auto v = InsertMenuVariant( category, action, separatorText, index );
    return boost::get< QAction* >( v );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertItem
// Created: AGE 2006-08-07
// Modified: ABR 2012-01-03
// -----------------------------------------------------------------------------
QAction* ContextMenu::InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member, bool separatorText /* = false */, int index /* = -1*/ )
{
    QAction* action = new QAction( text, this );
    const auto v = InsertMenuVariant( category, action, separatorText, index );
    const auto a = boost::get< QAction* >( v );
    if( a == action )
        QObject::connect( action, SIGNAL( triggered() ), receiver, member );
    return a;
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertItem
// Created: AGE 2006-08-04
// Modified: ABR 2012-01-03
// -----------------------------------------------------------------------------
QAction* ContextMenu::InsertItem( const std::string& category, const QString& text, const QObject* receiver, const char* member, const QKeySequence& accel /* = 0*/, int index /* = -1*/ )
{
    QAction* action = new QAction( text, this );
    action->setShortcut( accel );
    const auto v = InsertMenuVariant( category, action, false, index );
    const auto a = boost::get< QAction* >( v );
    if( a == action )
        QObject::connect( action, SIGNAL( triggered() ), receiver, member );
    return a;
    return action;
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertItem
// Created: AGE 2006-08-04
// Modified: ABR 2012-01-03
// -----------------------------------------------------------------------------
QAction* ContextMenu::InsertItem( const std::string& category, const QString& text, int index /* = -1*/ )
{
    QAction* action = new QAction( text, this );
    const auto v = InsertMenuVariant( category, action, false, index );
    return boost::get< QAction* >( v );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::InsertItem
// Created: AGE 2006-08-04
// Modified: ABR 2012-01-03
// -----------------------------------------------------------------------------
QAction* ContextMenu::InsertItem( const std::string& category, const QString& text, ContextMenu* subMenu, int index /* = -1*/ )
{
    subMenu->setTitle( text );
    return SubMenu( category, subMenu, false, index )->menuAction();
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::SubMenu
// Created: SBO 2006-08-10
// Modified: ABR 2012-01-03
// -----------------------------------------------------------------------------
ContextMenu* ContextMenu::SubMenu( const std::string& category, const QString& text, bool textSeparator /* = false */, int index /* = -1*/  )
{
    ContextMenu* newMenu = new ContextMenu( this );
    newMenu->setTitle( text );
    return SubMenu( category, newMenu, textSeparator, index );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::SubMenu
// Created: ABR 2012-01-10
// -----------------------------------------------------------------------------
ContextMenu* ContextMenu::SubMenu( const std::string& category, ContextMenu* newMenu, bool textSeparator /*= false*/, int index /*= -1*/ )
{
    const auto v = InsertMenuVariant( category, newMenu, textSeparator, index );
    return boost::get< ContextMenu* >( v );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::Popup
// Created: AGE 2006-08-04
// Modified: ABR 2012-01-03
// -----------------------------------------------------------------------------
void ContextMenu::Popup( const QPoint& where )
{
    FillMenu();
    this->popup( where );
}

// -----------------------------------------------------------------------------
// Name: ContextMenu::operator QWidget*
// Created: AGE 2006-08-04
// Modified: ABR 2012-01-03
// -----------------------------------------------------------------------------
ContextMenu::operator QWidget*() const
{
    return const_cast< ContextMenu* >( this );
}
