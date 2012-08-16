// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UserProfileControls_ABC.h"
#include "icons.h"
#include "ControlsChecker_ABC.h"
#include "ProfileConsistencyDialog.h"
#include "clients_gui/ValuedListItem.h"
#include "clients_kernel/Tools.h"
#include "preparation/ProfileHierarchies_ABC.h"
#include "preparation/UserProfile.h"
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
            const bool isReadable = ( status == eControl );
            if( CanWrite( entity ) )
            {
                const bool isWriteable = isReadable && !supervisor_ ;
                profile_->SetReadable( *entity, isReadable && !isWriteable );
                profile_->SetWriteable( *entity, isWriteable );
            }
            else
            {
                profile_->SetReadable( *entity, isReadable );
                profile_->SetWriteable( *entity, false );
            }
        }
    UpdateFilter();
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::Display
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::Display( UserProfile& profile )
{
    Initialize( profile.GetWriteProfilesCount() != 0 );
    listView_->setDisabled( false );
    profile_ = &profile;
    ValuedListItem* child = static_cast< ValuedListItem* >( listView_->firstChild() );
    while( child )
    {
        UpdateColor( child );
        ReadRights( child, IsControlled( child ) );
        child = static_cast< ValuedListItem* >( child->nextSibling() );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::ReadRights
// Created: LGY 2011-12-14
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::ReadRights( gui::ValuedListItem* item, bool control )
{
    UpdateColor( item );
    if( control )
    {
        SetItem( item, eControl );
        SelectParent( item );
        listView_->ensureItemVisible( item );
        ValuedListItem* child = static_cast< ValuedListItem* >( item->firstChild() );
        while( child )
        {            
            ReadRights( child, control );
            child = static_cast< ValuedListItem* >( child->nextSibling() );
        }
    }
    else
    {
        ValuedListItem* child = static_cast< ValuedListItem* >( item->firstChild() );
        while( child )
        {
            ReadRights( child, IsControlled( child ) );
            child = static_cast< ValuedListItem* >( child->nextSibling() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::IsControlled
// Created: LGY 2011-12-14
// -----------------------------------------------------------------------------
bool UserProfileControls_ABC::IsControlled( gui::ValuedListItem* item ) const
{
    const Entity_ABC* entity = item->GetValue< const Entity_ABC >();
    if( !entity || !CanWrite( entity ) )
        return false;
    return supervisor_ ? profile_->IsReadable( *entity ) : profile_->IsWriteable( *entity );
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::CanWrite
// Created: LDC 2012-03-01
// -----------------------------------------------------------------------------
bool UserProfileControls_ABC::CanWrite( const kernel::Entity_ABC* /*entity*/ ) const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::UpdateRights
// Created: LGY 2011-12-14
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::UpdateRights( gui::ValuedListItem* item, bool control )
{
    if( control )
    {
        SelectChild( item );
        SelectParent( item );
    }
    else
    {
        DeselectParent( item );
        DeselectChild( item );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::SelectParent
// Created: LGY 2011-12-28
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::SelectParent( gui::ValuedListItem* item )
{
    if( gui::ValuedListItem* parent = static_cast< ValuedListItem* >( item->parent() ) )
    {
        bool allSelect = true;
        ValuedListItem* value = static_cast< ValuedListItem* >( parent->firstChild() );
        while( value )
        {
            if( value->text( 2 ).toInt() != eControl )
                allSelect = false;
            value = static_cast< ValuedListItem* >( value->nextSibling() );
        }
        if( allSelect && parent->text( 2 ).toInt() != eControl )
            SetItem( parent, eControl );
        SelectParent( parent );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::SelectChild
// Created: LGY 2011-12-14
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::SelectChild( gui::ValuedListItem* item )
{
    SetItem( item, eControl );

    ValuedListItem* value = static_cast< ValuedListItem* >( item->firstChild() );
    while( value )
    {
        SelectChild( value );
        value = static_cast< ValuedListItem* >( value->nextSibling() );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::DeselectParents
// Created: LGY 2011-12-14
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::DeselectParent( gui::ValuedListItem* item )
{
    SetItem( item, eNothing );
    if( gui::ValuedListItem* parent = static_cast< ValuedListItem* >( item->parent() ) )
        if( parent->text( 2 ).toInt() == eControl )
            DeselectParent( parent );
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::DeselectChild
// Created: LGY 2011-12-28
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::DeselectChild( gui::ValuedListItem* item )
{
    ValuedListItem* value = static_cast< ValuedListItem* >( item->firstChild() );
    while( value )
    {
        SetItem( value, eNothing );
        DeselectChild( value );
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
    bool control = status == eControl;
    if( column == 1 )
        control = !control;
    if( control )
        Check( static_cast< ValuedListItem* >( item ), control );
    else
    {
        UpdateRights( static_cast< ValuedListItem* >( item ), control );
        Commit();
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::Initialize
// Created: LGY 2011-09-12
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::Initialize( bool collapse )
{
    for( Q3ListViewItemIterator it( listView_ ); it.current(); ++it )
    {
        (*it)->setPixmap( 1, QPixmap() );
        (*it)->setText( 2, QString::number( eNothing ) );
        static_cast< ValuedListItem* >( *it )->SetFontColor( QColor( 0, 0, 0 ) );
        if( collapse )
            listView_->setOpen( *it, false );
    }
    profile_ = 0;
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
    else
        item->setPixmap( 1, check_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::Update
// Created: LGY 2011-09-15
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::Update( bool supervisor, UserProfile& profile )
{
    supervisor_ = supervisor;
    profile_ = &profile;
    listView_->setColumnText( 1, supervisor ? tools::translate( "UserProfileControls", "View" ) :
                                              tools::translate( "UserProfileControls", "Control" ) );
    for( Q3ListViewItemIterator it( listView_ ); it.current(); ++it )
    {
        ValuedListItem* item = static_cast< ValuedListItem* >( *it );
        item->SetFontColor( QColor( 0, 0, 0 ) );
        if( !supervisor_ )
            UpdateColor( item );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::Check
// Created: LGY 2011-09-15
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::Check( ValuedListItem* item, bool control )
{
    T_Errors errors;
    CheckErrors( item, errors );
    if( !errors.empty() && !supervisor_ )
    {
        ProfileConsistencyDialog* dialog = new ProfileConsistencyDialog( listView_ );
        if( dialog->Exec( errors ) == QDialog::Accepted )
        {
            UpdateRights( item, control );
            Commit();
        }
    }
    else
    {
        UpdateRights( item, control );
        Commit();
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::CheckErrors
// Created: LGY 2011-12-13
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::CheckErrors( gui::ValuedListItem* item, T_Errors& errors )
{
    CheckErrors( *item->GetValue< const Entity_ABC >(), errors );
    ValuedListItem* value = static_cast< ValuedListItem* >( item->firstChild() );
    while( value )
    {
        CheckErrors( value, errors );
        value = static_cast< ValuedListItem* >( value->nextSibling() );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::CheckErrors
// Created: LGY 2011-09-15
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::CheckErrors( const kernel::Entity_ABC& entity, T_Errors& errors )
{
    BOOST_FOREACH( const std::string& result, checker_.Find( entity ) )
        if( profile_->GetLogin().toAscii().constData() != result )
            errors.push_back( std::make_pair( result, &entity ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::UpdateColor
// Created: LGY 2011-12-14
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::UpdateColor( gui::ValuedListItem* item )
{
    if( const Entity_ABC* entity = item->GetValue< const Entity_ABC >() )
        if( entity->Retrieve< ProfileHierarchies_ABC >() )
            if( !supervisor_ && profile_ && checker_.IsControlledByLowLevel( profile_->GetLogin().toAscii().constData(), *entity ) )
                item->SetFontColor( QColor( 255, 10, 10 ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::HideAssignedAutomats
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::HideAssignedAutomats()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::ShowAssignedAutomats
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::ShowAssignedAutomats()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::RemoveFilter
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::RemoveFilter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileControls_ABC::UpdateFilter
// Created: LGY 2011-12-13
// -----------------------------------------------------------------------------
void UserProfileControls_ABC::UpdateFilter()
{
    // NOTHING
}
