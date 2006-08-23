// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "CampPrototype_ABC.h"
#include "moc_CampPrototype_ABC.cpp"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/AutomatType.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: CampPrototype_ABC::CampPrototype_ABC
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CampPrototype_ABC::CampPrototype_ABC( QWidget* parent, Controllers& controllers )
    : ObjectPrototypeAttributes_ABC( parent, tr( "Camp" ) )
    , controllers_( controllers )
    , selected_( controllers )
{
    new QLabel( tr( "TC2:" ), this );
    tc2s_ = new ValuedComboBox< const Agent_ABC* >( this );
    controllers_.Register( *this );
}
    
// -----------------------------------------------------------------------------
// Name: CampPrototype_ABC::~CampPrototype_ABC
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
CampPrototype_ABC::~CampPrototype_ABC()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: CampPrototype_ABC::CheckValidity
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
bool CampPrototype_ABC::CheckValidity() const
{
    return tc2s_->GetValue();
}

// -----------------------------------------------------------------------------
// Name: CampPrototype_ABC::NotifyCreated
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CampPrototype_ABC::NotifyCreated( const Agent_ABC& agent )
{
    if( tc2s_->GetItemIndex( &agent ) != -1 )
        return;
    if( agent.GetAutomatType() && agent.GetAutomatType()->IsTC2() )
        tc2s_->AddItem( agent.GetName(), &agent );
}

// -----------------------------------------------------------------------------
// Name: CampPrototype_ABC::NotifyDeleted
// Created: SBO 2006-04-19
// -----------------------------------------------------------------------------
void CampPrototype_ABC::NotifyDeleted( const Agent_ABC& agent )
{
    tc2s_->RemoveItem( &agent );
}

// -----------------------------------------------------------------------------
// Name: CampPrototype_ABC::NotifyContextMenu
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void CampPrototype_ABC::NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu )
{
    if( isVisible() && agent.GetAutomatType() && agent.GetAutomatType()->IsTC2() )
    {
        selected_ = &agent;
        menu.InsertItem( "Parametre", tr( "TC2 du camp" ), this, SLOT( SetSelected() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: CampPrototype_ABC::SetSelected
// Created: AGE 2006-04-21
// -----------------------------------------------------------------------------
void CampPrototype_ABC::SetSelected()
{
    if( selected_ )
        tc2s_->SetCurrentItem( selected_ );
}
