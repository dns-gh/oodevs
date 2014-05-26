// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "DrawingsBuilder.h"
#include "moc_DrawingsBuilder.cpp"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Drawing_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Tools.h"
#include "gaming/Drawing.h"
#include "gaming/TacticalLine_ABC.h"

namespace
{
    class ConfirmationBox : public QMessageBox
    {
    private:
        typedef boost::function1< void, int > T_Callback;
    public:
        ConfirmationBox( const QString& title, T_Callback callback )
            : QMessageBox( title, "", QMessageBox::Warning, QMessageBox::Yes, QMessageBox::No | QMessageBox::Default, Qt::NoButton )
            , callback_( callback )
        {
            setIcon( QMessageBox::Information );
            hide();
        }

        virtual void done( int result )
        {
            callback_( result );
            hide();
        }
    private:
        T_Callback callback_;
    };
}

DrawingsBuilder::DrawingsBuilder( kernel::Controllers& controllers, const kernel::Profile_ABC& profile )
    : controllers_( controllers )
    , toDelete_( controllers )
    , profile_ ( profile )
    , confirmation_( new ConfirmationBox( tr( "Confirmation" ), boost::bind( &DrawingsBuilder::OnConfirmDeletion, this, _1 ) ) )
{
    controllers_.Register( *this );
}

DrawingsBuilder::~DrawingsBuilder()
{
    controllers_.Unregister( *this );
}

void DrawingsBuilder::OnRename( kernel::Entity_ABC& entity, const QString& newName )
{
    if( entity.GetTypeName() == kernel::Drawing_ABC::typeName_ )
        if( Drawing* drawing = static_cast< Drawing* >( &entity ) )
            drawing->Rename( newName );
    if( entity.GetTypeName() == kernel::TacticalLine_ABC::typeName_ )
        if( ::TacticalLine_ABC* line = static_cast< ::TacticalLine_ABC* >( &entity ) )
            line->Rename( newName );
}

void DrawingsBuilder::CreateCommunication()
{
    // NOTHING
}

void DrawingsBuilder::CreateTeam()
{
    // NOTHING
}

void DrawingsBuilder::ClearSelection()
{
    // NOTHING
}

void DrawingsBuilder::CreateFormation( int /*level*/ )
{
    // NOTHING
}

kernel::Automat_ABC* DrawingsBuilder::ReplaceAutomat( kernel::Entity_ABC& /*original*/, const kernel::AutomatType& /*type*/ )
{
    return 0;
}

namespace
{
    bool CanBeOrdered( const kernel::Entity_ABC& entity, const kernel::Profile_ABC& profile )
    {
        const kernel::Entity_ABC* superior = entity.Get< kernel::TacticalHierarchies >().GetSuperior();
        return superior && profile.CanBeOrdered( *superior );
    }
}

void DrawingsBuilder::DeleteEntity( const kernel::Entity_ABC& entity )
{
    toDelete_ = 0;
    if( entity.GetTypeName() == kernel::Drawing_ABC::typeName_ ||
        ( entity.GetTypeName() == kernel::TacticalLine_ABC::typeName_ && CanBeOrdered( entity, profile_ ) ) )
    {
        toDelete_ = &entity;
        OnDelete();
    }
}

void DrawingsBuilder::CreateCityOrDistrict( kernel::Entity_ABC* /*parent*/ )
{
    // NOTHING
}

void DrawingsBuilder::DeleteBlocks( const std::vector< const kernel::UrbanObject_ABC* >& /*urbanObjects*/ )
{
    // NOTHING
}

void DrawingsBuilder::NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    toDelete_ = 0;
    if( entity.GetTypeName() == kernel::Drawing_ABC::typeName_ ||
        ( entity.GetTypeName() == kernel::TacticalLine_ABC::typeName_ && CanBeOrdered( entity, profile_ ) ) )
    {
        toDelete_= &entity;
        menu.InsertItem( "Command", tr( "Delete" ), this, SLOT( OnDelete() ), false, 5 );
    }
}

void DrawingsBuilder::OnDelete()
{
    if( toDelete_ )
    {
        confirmation_->setText( tr( "Delete '%1'?" ).arg( toDelete_->GetName() ) );
        confirmation_->adjustSize();
        confirmation_->show();
    }
}

void DrawingsBuilder::OnConfirmDeletion( int result )
{
    if( result == QMessageBox::Yes && toDelete_ )
    {
        if( toDelete_->GetTypeName() == kernel::Drawing_ABC::typeName_ )
            if( kernel::Drawing_ABC* drawing = static_cast< kernel::Drawing_ABC* >( toDelete_.ConstCast() ) )
                drawing->NotifyDestruction();

        if( toDelete_->GetTypeName() == kernel::TacticalLine_ABC::typeName_ )
            if( kernel::TacticalLine_ABC* line = static_cast< kernel::TacticalLine_ABC* >( toDelete_.ConstCast() ) )
                line->NotifyDestruction();

        toDelete_ = 0;
        controllers_.actions_.DeselectAll();
    }
}
