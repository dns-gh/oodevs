// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "StockPrototype_ABC.h"
#include "moc_StockPrototype_ABC.cpp"
#include "RichLabel.h"
#include "RichSpinBox.h"
#include "SubObjectName.h"
#include "clients_kernel/DotationType.h"
#include "Tools.h"
#include "LoadableSpinBox.h"
#include <tools/Iterator.h>
#include <boost/foreach.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: StockPrototype_ABC constructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
StockPrototype_ABC::StockPrototype_ABC( QWidget* parent, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver )
    : ObjectAttributePrototype_ABC( parent, "TrafficabilityPrototypeABC", tools::translate( "gui::StockPrototype_ABC", "Stock parameters" ) )
    , resolver_( resolver )
{
    SubObjectName subObject( "TrafficabilityPrototypeABC" );
    QGridLayout* layout = new QGridLayout( this, 0, 2 );
    QSpinBox* dotationCount = new RichSpinBox( "dotationCount", 0, 0, 100, 1 );
    dotationCount->setValue( 0 );
    dotationCount->connect( dotationCount, SIGNAL( valueChanged(int) ), this, SLOT( dotationCountChanged(int) ) );
    layout->addWidget( dotationCount );
}

// -----------------------------------------------------------------------------
// Name: StockPrototype_ABC destructor
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
StockPrototype_ABC::~StockPrototype_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockPrototype_ABC::FillTypes
// Created: BCI 2011-02-04
// -----------------------------------------------------------------------------
void StockPrototype_ABC::FillTypes()
{
    BOOST_FOREACH( DotationWidget* dotationWidget, dotationWidgets_ )
    {
        dotationWidget->dotationType_->Clear();
        tools::Iterator< const kernel::DotationType& > it( resolver_.CreateIterator() );
        while( it.HasMoreElements() )
        {
            const kernel::DotationType& element = it.NextElement();
            dotationWidget->dotationType_->AddItem( element.GetName(), &element );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: StockPrototype_ABC::showEvent
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
void StockPrototype_ABC::showEvent( QShowEvent* e )
{
    FillTypes();
    ObjectAttributePrototype_ABC::showEvent( e );
}

// -----------------------------------------------------------------------------
// Name: StockPrototype_ABC::CheckValidity
// Created: JCR 2008-06-30
// -----------------------------------------------------------------------------
bool StockPrototype_ABC::CheckValidity( const kernel::Team_ABC& ) const
{
    bool bValid = true;
    BOOST_FOREACH( DotationWidget* dotationWidget, dotationWidgets_ )
    {
        if( !dotationWidget->dotationType_->count() )
        {
            dotationWidget->dotationLabel_->Warn( 3000 );
            bValid = false;
        }
        if( dotationWidget->stock_->value() > dotationWidget->maxStock_->value() )
        {
            dotationWidget->maxStockLabel_->Warn( 3000 );
            bValid = false;
        }

    }
    return bValid;
}

// -----------------------------------------------------------------------------
// Name: StockPrototype_ABC::dotationCountChanged
// Created: BCI 2011-02-04
// -----------------------------------------------------------------------------
void StockPrototype_ABC::dotationCountChanged( int count )
{
    gui::SubObjectName subObject( "StockPrototype_ABC" );
    while( dotationWidgets_.size() < std::size_t( count ) )
    {
        DotationWidget* dotationWidget = new DotationWidget( this );

        dotationWidget->dotationLabel_ = new RichLabel( tools::translate( "gui::StockPrototype_ABC", "Resource type:" ), dotationWidget );
        dotationWidget->dotationType_ = new ValuedComboBox< const kernel::DotationType* >( "dotationType", dotationWidget );

        new QLabel( tools::translate( "gui::StockPrototype_ABC", "Stock:" ), dotationWidget );
        dotationWidget->stock_ = new LoadableSpinBox( "stock", 0, std::numeric_limits< int >::max(), 1, dotationWidget );

        dotationWidget->maxStockLabel_ = new RichLabel( tools::translate( "gui::StockPrototype_ABC", "Max stock:" ), dotationWidget );
        dotationWidget->maxStock_ = new LoadableSpinBox( "maxStock", 0, std::numeric_limits< int >::max(), 1, dotationWidget );

        dotationWidget->show();
        dotationWidgets_.push_back( dotationWidget );
    }

    while( dotationWidgets_.size() > std::size_t( count ) )
    {
        delete dotationWidgets_.back();
        dotationWidgets_.pop_back();
    }

    FillTypes();
}

// -----------------------------------------------------------------------------
// Name: StockPrototype_ABC::SetLoader
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
void StockPrototype_ABC::SetLoader( ObjectPrototypeLoader_ABC* loader )
{
     BOOST_FOREACH( DotationWidget* dotationWidget, dotationWidgets_ )
     {
         dotationWidget->stock_->SetLoader( loader );
         dotationWidget->maxStock_->SetLoader( loader );
     }
}
