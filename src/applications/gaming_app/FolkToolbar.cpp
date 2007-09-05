// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "FolkToolbar.h"
#include "moc_FolkToolbar.cpp"
#include "gaming/FolkModel.h"
#include "clients_kernel/Controllers.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FolkToolbar constructor
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
FolkToolbar::FolkToolbar( QMainWindow* pParent, Controllers& controllers, FolkModel& model )
    : QToolBar( pParent, "folk toolbar" )
    , controllers_( controllers )
    , model_( model )
{
    setLabel( tr( "Folk control" ) );

    container_ = new QComboBox( this );
    profile_   = new QComboBox( this );
    activity_  = new QComboBox( this );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: FolkToolbar destructor
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
FolkToolbar::~FolkToolbar()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: FolkToolbar::SomethingChanged
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
void FolkToolbar::SomethingChanged()
{
    model_.SwitchTo( container_->currentText(), profile_->currentText(), activity_->currentText() );
}

// -----------------------------------------------------------------------------
// Name: FolkToolbar::Fill
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
void FolkToolbar::Fill( QComboBox* box, const std::vector< QString >& names )
{
    box->clear();
    box->insertItem( tr( "any" ) );
    for( std::vector< QString >::const_iterator it = names.begin(); it != names.end(); ++it )
        box->insertItem( *it );
    box->setCurrentItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: FolkToolbar::NotifyUpdated
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
void FolkToolbar::NotifyUpdated( const FolkModel& model )
{
    disconnect( this );
    Fill( container_, model.Containers() );
    Fill( profile_,   model.Profiles() );
    Fill( activity_,  model.Activities() );

    connect( container_, SIGNAL( activated( int ) ), SLOT( SomethingChanged() ) );
    connect( profile_,   SIGNAL( activated( int ) ), SLOT( SomethingChanged() ) );
    connect( activity_,  SIGNAL( activated( int ) ), SLOT( SomethingChanged() ) );
    show();
}
