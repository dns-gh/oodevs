// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::LogisticPrototype_ABC */

#include "clients_gui_pch.h"
#include "LogisticPrototype_ABC.h"
#include "moc_LogisticPrototype_ABC.cpp"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/AutomatType.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::LogisticPrototype_ABC
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
LogisticPrototype_ABC::LogisticPrototype_ABC( QWidget* parent, Controllers& controllers )
    : ObjectAttributePrototype_ABC( parent, tr( "Camp" ) )
    , controllers_( controllers )
    , selected_   ( controllers )
{
    new QLabel( tr( "Logistic unit:" ), this );
    logSuperiors_ = new ValuedComboBox< const Entity_ABC* >( this );
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
bool LogisticPrototype_ABC::CheckValidity() const
{
    return logSuperiors_->count() && logSuperiors_->GetValue();
}

// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::NotifyCreated
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void LogisticPrototype_ABC::NotifyCreated( const Automat_ABC& automat )
{
    if( logSuperiors_->GetItemIndex( &automat ) != -1 )
        return;

    if( automat.GetLogisticLevel() == kernel::LogisticLevel::none_ )
        return;

    logSuperiors_->AddItem( automat.GetName(), &automat );
    if( !selected_ )
        selected_ = &automat;
}


// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::NotifyCreated
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void LogisticPrototype_ABC::NotifyCreated( const Formation_ABC& formation )
{
    if( logSuperiors_->GetItemIndex( &formation ) != -1 )
        return;

    if( formation.GetLogisticLevel() == kernel::LogisticLevel::none_ )
        return;

    logSuperiors_->AddItem( formation.GetName(), &formation );
    if( !selected_ )
        selected_ = &formation;
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
// Name: LogisticPrototype_ABC::NotifyContextMenu
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void LogisticPrototype_ABC::NotifyContextMenu( const Automat_ABC& agent, ContextMenu& menu )
{
    if( isVisible() && agent.GetLogisticLevel() != kernel::LogisticLevel::none_ )
    {
        selected_ = &agent;
        menu.InsertItem( "Parameter", tr( "Camp's logistic unit" ), this, SLOT( SetSelected() ) );
    }
}


// -----------------------------------------------------------------------------
// Name: LogisticPrototype_ABC::NotifyContextMenu
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void LogisticPrototype_ABC::NotifyContextMenu( const Formation_ABC& formation, ContextMenu& menu )
{
    if( isVisible() && formation.GetLogisticLevel() != kernel::LogisticLevel::none_ )
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
