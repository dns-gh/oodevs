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
#include "clients_gui/ChangeSuperiorDialog.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Drawing_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Pathfind_ABC.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Tools.h"
#include "gaming/Drawing.h"
#include "gaming/Pathfind.h"
#include "gaming/TacticalLine_ABC.h"

namespace
{
    class ConfirmationBox : public QMessageBox
    {
    private:
        typedef std::function< void( int ) > T_Callback;
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
    , currentEntity_( controllers )
    , profile_ ( profile )
    , confirmation_( new ConfirmationBox( tr( "Confirmation" ), boost::bind( &DrawingsBuilder::OnConfirmDeletion, this, _1 ) ) )
{
    controllers_.Register( *this );
}

DrawingsBuilder::~DrawingsBuilder()
{
    controllers_.Unregister( *this );
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
    currentEntity_ = 0;
    if( entity.GetTypeName() == kernel::Drawing_ABC::typeName_ ||
        entity.GetTypeName() == kernel::Pathfind_ABC::typeName_ ||
        ( entity.GetTypeName() == kernel::TacticalLine_ABC::typeName_ && CanBeOrdered( entity, profile_ ) ) )
    {
        currentEntity_ = &entity;
        OnDelete();
    }
}

void DrawingsBuilder::ChangeSuperior( const kernel::Entity_ABC& entity )
{
    changeSuperiorDialog_->Show( const_cast< kernel::Entity_ABC& >( entity ),
                                 tr( "Change superior" ),
                                 gui::ChangeSuperiorDialog::eTacticalSuperior );
}

void DrawingsBuilder::CreateCityOrDistrict( kernel::Entity_ABC* /*parent*/ )
{
    // NOTHING
}

void DrawingsBuilder::DeleteBlocks( const std::vector< const kernel::UrbanObject_ABC* >& /*urbanObjects*/ )
{
    // NOTHING
}

void DrawingsBuilder::SetChangeSuperiorDialog( const std::shared_ptr< gui::ChangeSuperiorDialog >& changeSuperiorDialog )
{
    changeSuperiorDialog_ = changeSuperiorDialog;
}

void DrawingsBuilder::NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    currentEntity_ = 0;
    auto type = entity.GetTypeName();
    if( type == kernel::Drawing_ABC::typeName_ ||
        ( type == kernel::TacticalLine_ABC::typeName_ && CanBeOrdered( entity, profile_ ) ) )
    {
        currentEntity_= &entity;
        menu.InsertItem( "Command", tr( "Delete" ), this, SLOT( OnDelete() ), false, 5 );
        menu.InsertItem( "Command", tr( "Change superior"), this, SLOT( OnChangeSuperior() ), false, 6 );
    }
}

void DrawingsBuilder::OnDelete()
{
    if( currentEntity_ )
    {
        confirmation_->setText( tr( "Delete '%1'?" ).arg( currentEntity_->GetName() ) );
        confirmation_->adjustSize();
        confirmation_->show();
    }
}

// -----------------------------------------------------------------------------
// Name: DrawingsBuilder::OnChangeSuperior
// Created: LDC 2014-11-06
// -----------------------------------------------------------------------------
void DrawingsBuilder::OnChangeSuperior()
{
    if( currentEntity_ )
        ChangeSuperior( *currentEntity_ );
}

void DrawingsBuilder::OnConfirmDeletion( int result )
{
    if( result == QMessageBox::Yes && currentEntity_ )
    {
        kernel::Entity_ABC* entity = currentEntity_.ConstCast();
        if( kernel::Drawing_ABC* drawing = dynamic_cast< kernel::Drawing_ABC* >( entity ) )
            drawing->NotifyDestruction();
        else if( kernel::TacticalLine_ABC* line = dynamic_cast< kernel::TacticalLine_ABC* >( entity ) )
            line->NotifyDestruction();
        else if( kernel::Pathfind_ABC* pathfind = dynamic_cast< kernel::Pathfind_ABC* >( entity ) )
            pathfind->NotifyDestruction();
        
        currentEntity_ = 0;
        controllers_.actions_.DeselectAll();
    }
}
