// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Health_GUI.h"

#include "ADN_Health_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_Tr.h"
#include "ADN_Table.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_TimeField.h"
#include "ADN_TableItem_TimeField.h"
#include "ADN_AvailabilityWarningTable.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qvbox.h>


// -----------------------------------------------------------------------------
// Name: ADN_Health_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Health_GUI::ADN_Health_GUI( ADN_Health_Data& data )
: ADN_GUI_ABC( "ADN_Health_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Health_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Health_GUI::~ADN_Health_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Health_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Health_GUI::Build()
{
    if( pMainWidget_ != 0 )
        return;

    ADN_GuiBuilder builder;

    // Create the top widget.
    pMainWidget_ = new QWidget( 0 );
    QGroupBox* pGroup = new QGroupBox( 4, Qt::Horizontal, tr( "Health" ), pMainWidget_ );

    QVBox* pVBox = new QVBox( pGroup ); 

    QWidget* pHolder = builder.AddFieldHolder( pVBox );

    builder.AddField< ADN_TimeField >( pHolder, tr( "Diagnostic time" ), data_.diagnosticTime_ );
    builder.AddField< ADN_TimeField >( pHolder, tr( "Sorting time" ), data_.sortingTime_ );
    
    // wounds
    QHBox* pWoundsGroup = new QHBox( pVBox );
    pWoundsGroup->setSpacing( 5 );

    ADN_Table* pWoundTable = builder.CreateTable( pWoundsGroup );
    pWoundTable->setNumCols( eNbrDoctorSkills + 2 );
    pWoundTable->setNumRows( 4 );
    pWoundTable->verticalHeader()->show();
    pWoundTable->setLeftMargin( 5 );
    pWoundTable->setSorting( false );

    int n = 0;
    for( ; n < eNbrDoctorSkills; ++n )
    {
        pWoundTable->horizontalHeader()->setLabel( n, ADN_Tr::ConvertFromDoctorSkills( (E_DoctorSkills)n ).c_str() );
        pWoundTable->setColumnStretchable( n, true );
    }
    pWoundTable->horizontalHeader()->setLabel( n, tr( "Shock" ) );
    pWoundTable->setColumnStretchable( n, true );
    pWoundTable->horizontalHeader()->setLabel( n + 1, tr( "Contamination" ) );
    pWoundTable->setColumnStretchable( n + 1, true );

    pWoundTable->verticalHeader()->setLabel( 0, tr( "Treating time" ) );
    pWoundTable->verticalHeader()->setLabel( 1, tr( "Resting time" ) );
    pWoundTable->verticalHeader()->setLabel( 2, tr( "Repartition (%)" ) );
    pWoundTable->verticalHeader()->setLabel( 3, tr( "Life expectency" ) );

    for( n = 0; n < eNbrDoctorSkills; ++n )
    {
        ADN_Health_Data::WoundInfo& wound = data_.wounds[n];
        builder.AddTableCell< ADN_TableItem_TimeField >( pWoundTable, &wound, 0, n, wound.treatTime_ );
        builder.AddTableCell< ADN_TableItem_TimeField >( pWoundTable, &wound, 1, n, wound.restingTime_ );
        builder.AddTableCell< ADN_TableItem_Double    >( pWoundTable, &wound, 2, n, wound.rPercentage_, ePercentage );
        builder.AddTableCell< ADN_TableItem_TimeField >( pWoundTable, &wound, 3, n, wound.lifeExpectancy_ );
    }
    builder.AddTableCell< ADN_TableItem_TimeField >( pWoundTable, &data_, 0, n, data_.shockTreatTime_ );
    builder.AddTableCell< ADN_TableItem_TimeField >( pWoundTable, &data_, 1, n, data_.shockRestingTime_ );
    builder.AddTableCell< ADN_TableItem_Double    >( pWoundTable, &data_, 2, n, data_.rShockPercentage_, ePercentage );
    pWoundTable->setItem( 3, n, new QTableItem( pWoundTable, QTableItem::Never ) );

    builder.AddTableCell< ADN_TableItem_TimeField >( pWoundTable, &data_, 0, n + 1, data_.contaminationTreatTime_ );
    builder.AddTableCell< ADN_TableItem_TimeField >( pWoundTable, &data_, 1, n + 1, data_.contaminationRestingTime_ );
    pWoundTable->setItem( 2, n + 1, new QTableItem( pWoundTable, QTableItem::Never ) );
    pWoundTable->setItem( 3, n + 1, new QTableItem( pWoundTable, QTableItem::Never ) );

    // Warning tables
    QHGroupBox* pWarningGroup = new QHGroupBox( tr( "Resources availability warnings" ), pGroup );
    QHGroupBox* pAvailabilityGroup = new QHGroupBox( tr( "Changeover" ), pWarningGroup );
    ADN_AvailabilityWarningTable* pChangeOverWarningTable = new ADN_AvailabilityWarningTable( pAvailabilityGroup );
    pChangeOverWarningTable->GetConnector().Connect( & data_.vChangeOverWarnings_ );
    
    pAvailabilityGroup = new QHGroupBox( tr( "Collect" ), pWarningGroup );
    ADN_AvailabilityWarningTable* pCollectWarningTable = new ADN_AvailabilityWarningTable( pAvailabilityGroup );
    pCollectWarningTable->GetConnector().Connect( & data_.vCollectingWarnings_ );

    pAvailabilityGroup = new QHGroupBox( tr( "Doctors" ), pWarningGroup );
    ADN_AvailabilityWarningTable* pDoctorsWarningTable = new ADN_AvailabilityWarningTable( pAvailabilityGroup );
    pDoctorsWarningTable->GetConnector().Connect( & data_.vDoctorsWarnings_ );

    // Layout
    QVBoxLayout* pLayout = new QVBoxLayout( pMainWidget_, 10, 5 );
    pLayout->addWidget( pGroup );
    builder.AddStretcher( pLayout, Qt::Vertical );
}
