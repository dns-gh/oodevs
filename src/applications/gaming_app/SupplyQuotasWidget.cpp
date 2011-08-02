// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "SupplyQuotasWidget.h"
#include "gaming/LogisticLink.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: SupplyQuotasWidget constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyQuotasWidget::SupplyQuotasWidget( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& /*factory*/ )
    : Q3VBox       ( parent )
    , controllers_( controllers )
{
    controllers_.Register( *this );

    Q3HBox* test = new Q3HBox( (Q3VBox*)this );
    /*QLabel* label =*/ new QLabel( tr( "Logistic superior" ), test );
    //label->setMaximumWidth( 150 );
    superior_ = new ValuedComboBox< const Entity_ABC* >( test );
    superior_->setMinimumWidth( 200 );
}

// -----------------------------------------------------------------------------
// Name: SupplyQuotasWidget destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyQuotasWidget::~SupplyQuotasWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SupplyQuotasWidget::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void SupplyQuotasWidget::NotifyUpdated( const LogisticLinks& a )
{
    superior_->Clear();

    tools::Iterator< const LogisticLink& > it = a.CreateSuperiorLinksIterator();
    while( it.HasMoreElements() )
    {
        const LogisticLink& link = it.NextElement();
        superior_->AddItem( link.GetSuperior().GetName(), &link.GetSuperior() );
    }
}
