// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipments_Resources_Tables.h"
#include "ADN_Table.h"
#include "ADN_Equipments_ConsumptionsTable.h"
#include "ADN_Equipments_Dotations_GUI.h"
#include "ADN_Equipments_Resources_Table.h"

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_Tables constructor
// Created: ABR 2012-11-30
// -----------------------------------------------------------------------------
ADN_Equipments_Resources_Tables::ADN_Equipments_Resources_Tables( const QString& objectName, ADN_Connector_ABC*& dotationConnector, ADN_Connector_ABC*& allocationConnector, ADN_Connector_ABC*& consumptionConnector, QWidget* parent /* = 0 */ )
    : QWidget( parent )
{
    setObjectName( objectName );

    categoryList_ = new ADN_Equipments_Resources_Table( objectName + "_CategoriesListView", dotationConnector );
    categoryList_->setFixedWidth( 150 );
    categoryList_->setFixedHeight( 200 );

    allocationTable_ = new ADN_Equipments_Dotations_GUI( objectName + "_AllocationTable", allocationConnector, 0, ADN_Equipments_Dotations_GUI::eColumn_All ^ ADN_Equipments_Dotations_GUI::eColumn_Category );
    allocationTable_->setFixedHeight( 200 );
    allocationTable_->horizontalHeader()->setResizeMode( 3, QHeaderView::ResizeToContents );

    consumptionTable_ = new ADN_Equipments_ConsumptionsTable( objectName + "_ConsumptionTable", consumptionConnector );
    consumptionTable_->setFixedHeight( 200 );

    connect( categoryList_->verticalScrollBar(), SIGNAL( valueChanged( int ) ), allocationTable_->verticalScrollBar(), SLOT( setValue( int ) ) );
    connect( allocationTable_->verticalScrollBar(), SIGNAL( valueChanged( int ) ), consumptionTable_->verticalScrollBar(), SLOT( setValue( int ) ) );
    connect( consumptionTable_->verticalScrollBar(), SIGNAL( valueChanged( int ) ), categoryList_->verticalScrollBar(), SLOT( setValue( int ) ) );


    QGridLayout* mainLayout = new QGridLayout( this );
    mainLayout->setMargin( 0 );
    mainLayout->addWidget( new QLabel( tools::translate( "ADN_Equipments_Resources_Tables", "Allocation" ) ), 0, 1, 1, 1, Qt::AlignCenter );
    mainLayout->addWidget( new QLabel( tools::translate( "ADN_Equipments_Resources_Tables", "Consumptions" ) ), 0, 2, 1, 1, Qt::AlignCenter );
    mainLayout->addWidget( categoryList_, 1, 0 );
    mainLayout->addWidget( allocationTable_, 1, 1 );
    mainLayout->addWidget( consumptionTable_, 1, 2 );

    mainLayout->setColumnStretch( 1, 2 );
    mainLayout->setColumnStretch( 2, 3 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_Tables destructor
// Created: ABR 2012-11-30
// -----------------------------------------------------------------------------
ADN_Equipments_Resources_Tables::~ADN_Equipments_Resources_Tables()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_Tables::GetAllocationTable
// Created: ABR 2012-12-04
// -----------------------------------------------------------------------------
const ADN_Table& ADN_Equipments_Resources_Tables::GetAllocationTable() const
{
    assert( allocationTable_ );
    return *allocationTable_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_Tables::GetConsumptionTable
// Created: ABR 2012-12-04
// -----------------------------------------------------------------------------
const ADN_Table& ADN_Equipments_Resources_Tables::GetConsumptionTable() const
{
    assert( consumptionTable_ );
    return *consumptionTable_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipments_Resources_Tables::ResetScrollBars
// Created: ABR 2012-12-04
// -----------------------------------------------------------------------------
void ADN_Equipments_Resources_Tables::ResetScrollBars()
{
    categoryList_->verticalScrollBar()->setValue( 0 );
    allocationTable_->verticalScrollBar()->setValue( 0 );
    consumptionTable_->verticalScrollBar()->setValue( 0 );
}
