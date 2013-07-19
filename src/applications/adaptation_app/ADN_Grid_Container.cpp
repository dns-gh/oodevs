// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Grid_Container.h"

// -----------------------------------------------------------------------------
// Name: ADN_Grid_Container constructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_Grid_Container::ADN_Grid_Container( E_WorkspaceElements workspaceElement, ADN_Data_Container& data )
    : ADN_GUI_Container( workspaceElement, data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Grid_Container destructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_Grid_Container::~ADN_Grid_Container()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Grid_Container::Build
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Grid_Container::Build()
{
    ADN_GUI_Container::Build();
    assert( pMainWidget_ == 0 );
    assert( elements_.size() == positions_.size() );

    QWidget* pContent = new QWidget();
    QGridLayout* pLayout = new QGridLayout( pContent );
    pLayout->setMargin( 10 );
    pLayout->setSpacing( 10 );
    pLayout->setAlignment( Qt::AlignTop );

    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        const GridPosition& pos = positions_[ it->first ];
        pLayout->addWidget( it->second.second->GetMainWidget(), pos.row_, pos.column_, pos.rowSpan_, pos.columnSpan_ );
    }

    pMainWidget_ = CreateScrollArea( strClassName_, *pContent );
}

// -----------------------------------------------------------------------------
// Name: ADN_Grid_Container::AddPosition
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Grid_Container::AddPosition( int index, int row, int column, int rowSpan /*= 1*/, int columnSpan /*= 1*/ )
{
    positions_[ index ] = GridPosition( row, column, rowSpan, columnSpan );
}
