// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "LogisticPrototype_ABC.h"
#include "moc_LogisticPrototype_ABC.cpp"

#include "LogisticBase.h"
#include "LongNameHelper.h"
#include "SubObjectName.h"

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::LogisticPrototype_ABC
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
LogisticPrototype_ABC::LogisticPrototype_ABC( QWidget* parent, Controllers& controllers )
    : ObjectAttributePrototype_ABC( parent, "LogisticPrototypeABC",tr( "Camp" ) )
    , controllers_( controllers )
    , selected_   ( controllers )
{
    SubObjectName subObject( "LogisticPrototypeABC" );
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    layout->setMargin( 5 );
    layout->addWidget( new QLabel( tr( "Logistic unit:" ) ) );
    logSuperiors_ = new ValuedComboBox< const Entity_ABC* >( "logSuperiors", 0 );
    layout->addWidget( logSuperiors_ );
    connect( logSuperiors_, SIGNAL( activated( int ) ), this, SLOT( SelectionChanged() ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::~LogisticPrototype_ABC
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
LogisticPrototype_ABC::~LogisticPrototype_ABC()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::CheckValidity
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
bool LogisticPrototype_ABC::CheckValidity( const kernel::Team_ABC& team ) const
{
    if( !logSuperiors_->count() )
        return false;
    const kernel::Entity_ABC* superior = logSuperiors_->GetValue();
    bool result = superior && ( superior->Get< kernel::TacticalHierarchies >().GetTop().GetId() == team.GetId() );
    if( !result )
        const_cast< LogisticPrototype_ABC* >( this )->Warn();
    return result;
}

namespace
{
    QString GetDisplayName( const Entity_ABC& entity )
    {
        std::string longName = longname::GetEntityLongName( entity );
        return longName.empty() ? entity.GetName() : longName.c_str();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::NotifyCreated
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void LogisticPrototype_ABC::NotifyCreated( const Automat_ABC& automat )
{
    if( logSuperiors_->GetItemIndex( &automat ) != -1 ||
        !automat.Get< LogisticBase >().IsBase() )
        return;
    logSuperiors_->AddItem( GetDisplayName( automat ), &automat );
    if( !selected_ )
        selected_ = &automat;
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::NotifyCreated
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void LogisticPrototype_ABC::NotifyCreated( const Formation_ABC& formation )
{
    if( logSuperiors_->GetItemIndex( &formation ) != -1 ||
        !formation.Get< LogisticBase >().IsBase() )
        return;
    logSuperiors_->AddItem( GetDisplayName( formation ), &formation );
    if( !selected_ )
        selected_ = &formation;
}

void LogisticPrototype_ABC::NotifyUpdated( const Automat_ABC& automat )
{
    UpdateDisplayName( automat );
}

void LogisticPrototype_ABC::NotifyUpdated( const Formation_ABC& formation )
{
    UpdateDisplayName( formation );
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::NotifyDeleted
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void LogisticPrototype_ABC::NotifyDeleted( const Automat_ABC& automat )
{
    logSuperiors_->RemoveItem( &automat );
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::NotifyDeleted
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void LogisticPrototype_ABC::NotifyDeleted( const Formation_ABC& formation )
{
    logSuperiors_->RemoveItem( &formation );
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::NotifyUpdated
// Created: JSR 2011-09-28
// -----------------------------------------------------------------------------
void LogisticPrototype_ABC::UpdateDisplayName( const kernel::Entity_ABC& entity )
{
    int pos = logSuperiors_->GetItemIndex( &entity );
    if( pos != -1 )
    {
        QString toDisplay = GetDisplayName( entity );
        if( logSuperiors_->text( pos ) != toDisplay )
            logSuperiors_->ChangeItem( toDisplay, &entity );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::NotifyContextMenu
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void LogisticPrototype_ABC::NotifyContextMenu( const Automat_ABC& automat, ContextMenu& menu )
{
    if( isVisible() && automat.Get< LogisticBase >().IsBase() )
    {
        selected_ = &automat;
        menu.InsertItem( "Parameter", tr( "Camp's logistic unit" ), this, SLOT( SetSelected() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::NotifyContextMenu
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void LogisticPrototype_ABC::NotifyContextMenu( const Formation_ABC& formation, ContextMenu& menu )
{
    if( isVisible() && formation.Get< LogisticBase >().IsBase() )
    {
        selected_ = &formation;
        menu.InsertItem( "Parameter", tr( "Camp's logistic unit" ), this, SLOT( SetSelected() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::SetSelected
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void LogisticPrototype_ABC::SetSelected()
{
    if( selected_ )
        logSuperiors_->SetCurrentItem( selected_ );
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::SelectionChanged
// Created: SBO 2006-09-15
// -----------------------------------------------------------------------------
void LogisticPrototype_ABC::SelectionChanged()
{
    selected_ = logSuperiors_->GetValue();
}
