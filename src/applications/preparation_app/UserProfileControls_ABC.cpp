// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UserProfileControls_ABC.h"
#include "icons.h"
#include "ControlsChecker_ABC.h"
#include "preparation/Tools.h"
#include "preparation/UserProfile.h"
#include "clients_gui/ValuedListItem.h"
#include <boost/foreach.hpp>

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC constructor
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
UserProfileControls_ABC::UserProfileControls_ABC( Q3ListView* listView, ControlsChecker_ABC& checker )
    : listView_  ( listView )
    , checker_   ( checker )
    , profile_   ( 0 )
    , check_     ( MAKE_PIXMAP( check ) )
    , check_grey_( MAKE_PIXMAP( check_grey ) )
    , supervisor_( false )
{
    listView_->header()->show();
    listView_->addColumn( tools::translate( "UserProfileControls", "Control" ), 60 );
    listView_->addColumn( "hidden", 0 );
    listView_->header()->setResizeEnabled( false );
    listView_->hideColumn( 2 );
    listView_->setDisabled( true );
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC destructor
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
UserProfileControls_ABC::~UserProfileControls_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::Commit
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::Commit()
{
    if( !profile_ )
        return;
    for( Q3ListViewItemIterator it( listView_ ); it.current(); ++it )
        if( const ValuedListItem* item = static_cast< const ValuedListItem* >( *it ) )
        {
            const Entity_ABC* entity = item->GetValue< const Entity_ABC >();
            const Status status = Status( item->text( 2 ).toInt() );
            const bool isWriteable = ( status == eControl ) ? ( supervisor_ ? false : true ) : false;
            const bool isReadable = ( status == eControl ) ? true : profile_->IsReadable( *entity );
            profile_->SetReadable( *entity, isReadable );
            profile_->SetWriteable( *entity, isWriteable );
            ValueChanged( entity, isReadable, isWriteable );
        }
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::Display
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::Display( UserProfile& profile )
{
    Clear();
    listView_->setDisabled( false );
    profile_ = &profile;
    ValuedListItem* value = static_cast< ValuedListItem* >( listView_->firstChild() );
    while( value )
    {
        SetStatus( value, false );
        value = static_cast< ValuedListItem* >( value->nextSibling() );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::OnItemClicked
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::OnItemClicked( Q3ListViewItem* item, const QPoint&, int column )
{
    if( column == 0 || !item )
        return;
    const Status status = Status( item->text( 2 ).toInt() );
    if( status == eControlInherited )
        return;
    bool control = status == eControl;
    if( column == 1 )
        control = !control;
    if( control )
        Check( static_cast< ValuedListItem* >( item ), control );
    else
    {
        SetStatus( static_cast< ValuedListItem* >( item ), control, false );
        Commit();
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::Clear
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::Clear()
{
    for( Q3ListViewItemIterator it( listView_ ); it.current(); ++it )
    {
        (*it)->setPixmap( 1, QPixmap() );
        listView_->setOpen( *it, false );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::SetItem
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::SetItem( Q3ListViewItem* item, Status status )
{
    item->setText( 2, QString::number( status ) );
    if( status == eNothing )
        item->setPixmap( 1, QPixmap() );
    else if( status == eControl )
        item->setPixmap( 1, check_ );
    else if( status == eControlInherited )
        item->setPixmap( 1, check_grey_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::SetStatus
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::SetStatus( gui::ValuedListItem* item, bool inheritsControllable )
{
    const Entity_ABC* entity = item->GetValue< const Entity_ABC >();
    if( !entity )
        return;
    const bool isControl = supervisor_ ? profile_->IsReadable( *entity ) : profile_->IsWriteable( *entity );
    SetStatus( item, isControl, inheritsControllable );
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::SetStatus
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::SetStatus( gui::ValuedListItem* item, bool isControl, bool inheritsControllable )
{
    SetItem( item, MakeStatus( isControl, inheritsControllable ) );
    if( isControl )
        listView_->ensureItemVisible( item );

    ValuedListItem* value = static_cast< ValuedListItem* >( item->firstChild() );
    while( value )
    {
        SetStatus( value, isControl || inheritsControllable );
        value = static_cast< ValuedListItem* >( value->nextSibling() );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::MakeStatus
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
UserProfileControls_ABC::Status UserProfileControls_ABC::MakeStatus( bool control, bool inheritedControl )
{
    Status status = eNothing;
    if( control )
        status = eControl;
    else if( inheritedControl )
        status = eControlInherited;
    return status;
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::Update
// Created: LGY 2011-09-15
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::Update( bool supervisor )
{
    supervisor_ = supervisor;
    listView_->setColumnText( 1, supervisor ? tools::translate( "UserProfileControls", "View" ) :
                                              tools::translate( "UserProfileControls", "Control" ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::Check
// Created: LGY 2011-09-15
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::Check( ValuedListItem* item, bool control )
{
   T_Errors errors = GetErrors( item );
    if( !errors.empty() && !supervisor_ )
    {
        QMessageBox msgBox( listView_ );
        msgBox.setWindowTitle( tools::translate( "UserProfileControls", "Profile" ) );
        msgBox.setText( ConvertErrors( errors ) );
        msgBox.setInformativeText( tools::translate( "UserProfileControls", "Do you want to proceed ?" ) );
        msgBox.setStandardButtons( QMessageBox::Yes | QMessageBox::No );
        msgBox.setDefaultButton( QMessageBox::No );
        msgBox.setIcon( QMessageBox::NoIcon );
        if( msgBox.exec() == QMessageBox::Yes )
        {
            BOOST_FOREACH(  const T_Error& error, errors )
                checker_.Update( *profile_, *error.second );
            SetStatus( item, control, false );
            Commit();
        }
    }
    else
    {
        SetStatus( item, control, false );
        Commit();
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::GetErrors
// Created: LGY 2011-09-15
// -----------------------------------------------------------------------------
UserProfileControls_ABC::T_Errors UserProfileControls_ABC::GetErrors( gui::ValuedListItem* item )
{
    T_Errors errors;
    CheckErrors( *item->GetValue< const Entity_ABC >(), errors );
    if( !errors.empty() )
        return errors;

    ValuedListItem* value = static_cast< ValuedListItem* >( item->firstChild() );
    while( value )
    {
        CheckErrors( *value->GetValue< const Entity_ABC >(), errors );
        value = static_cast< ValuedListItem* >( value->nextSibling() );
    }
    return errors;
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::CheckErrors
// Created: LGY 2011-09-15
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::CheckErrors( const kernel::Entity_ABC& entity, T_Errors& errors )
{
    std::string login = checker_.CheckControl( *profile_, entity );
    if( login != "" )
        errors.push_back( std::make_pair( login, &entity ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::ConvertErrors
// Created: LGY 2011-09-15
// -----------------------------------------------------------------------------
QString UserProfileControls_ABC::ConvertErrors( T_Errors& errors ) const
{
    QString results;
    BOOST_FOREACH( const T_Error& error, errors )
        results += QString( tools::translate( "UserProfileControls", "Unit '%1' is already controlled by '%2' profile.\n" ) )
            .arg( error.second->GetName().ascii() ).arg( error.first.c_str() );
    return results;
}
