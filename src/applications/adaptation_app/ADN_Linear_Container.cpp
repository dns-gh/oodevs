// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Linear_Container.h"

// -----------------------------------------------------------------------------
// Name: ADN_Linear_Container constructor
// Created: ABR 2012-01-24
// -----------------------------------------------------------------------------
ADN_Linear_Container::ADN_Linear_Container( Qt::Orientation orientation, bool useScrollBar, const char* szClassName, ADN_Data_Container& data )
    : ADN_GUI_Container( szClassName, data )
    , orientation_ ( orientation )
    , useScrollBar_( useScrollBar)
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Linear_Container destructor
// Created: ABR 2012-01-24
// -----------------------------------------------------------------------------
ADN_Linear_Container::~ADN_Linear_Container()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Linear_Container::Build
// Created: ABR 2012-01-24
// -----------------------------------------------------------------------------
void ADN_Linear_Container::Build()
{
    ADN_GUI_Container::Build();
    assert( pMainWidget_ == 0 );

    QBoxLayout* pLayout = 0;
    if( orientation_ == Qt::Vertical )
        pLayout = new QVBoxLayout();
    else
        pLayout = new QHBoxLayout();

    pLayout->setMargin( 0 );
    pLayout->setSpacing( 0 );
    pLayout->setAlignment( ( orientation_ == Qt::Vertical ) ? Qt::AlignTop : Qt::AlignLeft );

    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        it->second.second->GetMainWidget()->reparent( pMainWidget_, QPoint( 0, 0 ) );
        pLayout->addWidget( it->second.second->GetMainWidget() );
    }

    if( orientation_ == Qt::Vertical )
        pLayout->addStretch( 1 );

    if( useScrollBar_ )
    {
        QWidget* pContent = new QWidget();
        pContent->setLayout( pLayout );
        pMainWidget_ = CreateScrollArea( *pContent );
    }
    else
    {
        pMainWidget_ = new QWidget( 0 );
        pMainWidget_->setLayout( pLayout );
    }
}
