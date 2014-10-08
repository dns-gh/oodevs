// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "DisasterPrototype.h"
#include "gaming/Simulation.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/RichCheckBox.h"
#include "actions/ParameterList.h"

// -----------------------------------------------------------------------------
// Name: DisasterPrototype constructor
// Created: LGY 2012-12-12
// -----------------------------------------------------------------------------
DisasterPrototype::DisasterPrototype( QWidget* parent, const tools::GeneralConfig& config,
                                      kernel::Controllers& controllers, actions::parameters::ParameterList*& attributesList )
    : gui::DisasterPrototype_ABC( parent, config )
    , controllers_   ( controllers )
    , attributesList_( attributesList )
    , initialized_  ( false )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype destructor
// Created: LGY 2012-12-12
// -----------------------------------------------------------------------------
DisasterPrototype::~DisasterPrototype()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype::Commit
// Created: LGY 2012-12-12
// -----------------------------------------------------------------------------
void DisasterPrototype::Commit( const kernel::Team_ABC& )
{
    actions::parameters::ParameterList& list = attributesList_->AddList( "Disaster" );
    list.AddIdentifier( "AttributeId", sword::ObjectMagicAction::disaster );
    list.AddString( "model", propagationFiles_->currentText().toStdString() );
    if( checkbox_->isChecked() )
        list.AddString( "date", date_->dateTime().toString( "yyyyMMddThhmmss" ).toStdString() );
    else
        list.AddString( "date", "" );
}

// -----------------------------------------------------------------------------
// Name: DisasterPrototype::NotifyUpdated
// Created: LGY 2012-12-12
// -----------------------------------------------------------------------------
void DisasterPrototype::NotifyUpdated( const Simulation& simulation )
{
    if( simulation.IsInitialized() && !initialized_ )
    {
        date_->setDateTime( simulation.GetDateTime() );
        initialized_ = true;
    }
}
