// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Tab_Container.h"

// -----------------------------------------------------------------------------
// Name: ADN_Tab_Container constructor
// Created: ABR 2012-01-24
// -----------------------------------------------------------------------------
ADN_Tab_Container::ADN_Tab_Container( const char* szClassName, ADN_Data_Container& data )
    : ADN_GUI_Container( szClassName, data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Tab_Container destructor
// Created: ABR 2012-01-24
// -----------------------------------------------------------------------------
ADN_Tab_Container::~ADN_Tab_Container()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Tab_Container::Build
// Created: ABR 2012-01-24
// -----------------------------------------------------------------------------
void ADN_Tab_Container::Build()
{
    ADN_GUI_Container::Build();
    assert( pMainWidget_ == 0 );

    // Tab management
    QTabWidget* pTabWidget = new QTabWidget();
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        it->second.second->GetMainWidget()->reparent( pMainWidget_, QPoint( 0, 0 ) );
        pTabWidget->addTab( it->second.second->GetMainWidget(), it->second.first );
    }

    // Main widget
    pMainWidget_ = new QWidget();
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_ );
    pMainLayout->setSpacing( 10 );
    pMainLayout->setMargin( 10 );
    pMainLayout->addWidget( pTabWidget );
}
