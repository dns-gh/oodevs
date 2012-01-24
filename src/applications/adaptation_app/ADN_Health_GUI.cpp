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
#include "ADN_Equipement_GUI.h"
#include "ADN_Tr.h"
#include "ADN_Table.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_TimeField.h"
#include "ADN_TableItem_TimeField.h"
#include "ADN_AvailabilityWarningTable.h"

namespace
{

class ADN_WoundTable : public ADN_Table
{
public:
    explicit ADN_WoundTable( QWidget* pParent = 0, const char* szName = 0 )
        : ADN_Table( pParent, szName ) {}

protected slots:
    virtual void doValueChanged( int row, int col )
    {
        ADN_Table::doValueChanged( row, col );
        ADN_Workspace::GetWorkspace().GetEquipements().GetGui().UpdateGraph();
    }
};

}

// -----------------------------------------------------------------------------
// Name: ADN_Health_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Health_GUI::ADN_Health_GUI( ADN_Health_Data& data )
    : ADN_GUI_ABC( "ADN_Health_GUI" )
    , data_      ( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Health_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Health_GUI::~ADN_Health_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Health_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Health_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder;

    // Health
    QWidget* pHolder = builder.AddFieldHolder( 0 );
    builder.AddField< ADN_TimeField >( pHolder, tr( "Diagnostic duration" ), data_.diagnosticTime_ );
    builder.AddField< ADN_TimeField >( pHolder, tr( "Triage duration" ), data_.sortingTime_ );
    builder.AddField< ADN_EditLine_Double >( pHolder, tr( "Diagnosis life span factor" ), data_.diagnosisLifeExpectancyFactor_, 0, eGreaterZero );
    builder.AddStretcher( pHolder, Qt::Vertical );

    // wounds
    ADN_Table* woundTable = BuildWoundsTable();

    // Warning tables
    Q3HGroupBox* pEvacuationBox = new Q3HGroupBox( tr( "Ambulances (evacuation) availability warnings" ) );
    ADN_AvailabilityWarningTable* pChangeOverWarningTable = new ADN_AvailabilityWarningTable( pEvacuationBox );
    pChangeOverWarningTable->GetConnector().Connect( & data_.vChangeOverWarnings_ );

    Q3HGroupBox* pCollectionBox = new Q3HGroupBox( tr( "Ambulances (collection) availability warnings" ) );
    ADN_AvailabilityWarningTable* pCollectWarningTable = new ADN_AvailabilityWarningTable( pCollectionBox );
    pCollectWarningTable->GetConnector().Connect( & data_.vCollectingWarnings_ );

    Q3HGroupBox* pDoctorsBox = new Q3HGroupBox( tr( "Doctors availability warnings" ) );
    ADN_AvailabilityWarningTable* pDoctorsWarningTable = new ADN_AvailabilityWarningTable( pDoctorsBox );
    pDoctorsWarningTable->GetConnector().Connect( & data_.vDoctorsWarnings_ );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // TopLayout
    QHBoxLayout* pTopLayout = new QHBoxLayout();
    pTopLayout->setMargin( 10 );
    pTopLayout->setSpacing( 10 );
    pTopLayout->addWidget( pHolder );
    pTopLayout->addWidget( woundTable );

    // BottomLayout
    QHBoxLayout* pBottomLayout = new QHBoxLayout();
    pBottomLayout->setMargin( 10 );
    pBottomLayout->setSpacing( 10 );
    pBottomLayout->addWidget( pEvacuationBox );
    pBottomLayout->addWidget( pCollectionBox );
    pBottomLayout->addWidget( pDoctorsBox );

    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addLayout( pTopLayout );
    pContentLayout->addLayout( pBottomLayout );
    pContentLayout->addStretch( 1 );

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent );
}

// -----------------------------------------------------------------------------
// Name: ADN_Health_GUI::BuildWoundsTable
// Created: ABR 2012-01-24
// -----------------------------------------------------------------------------
ADN_Table* ADN_Health_GUI::BuildWoundsTable()
{
    ADN_GuiBuilder builder;
    ADN_WoundTable* pWoundTable = new ADN_WoundTable();
    pWoundTable->setFixedHeight( 200 );
    pWoundTable->setSorting( true );
    pWoundTable->setSelectionMode( Q3Table::NoSelection );
    pWoundTable->setShowGrid( true );
    pWoundTable->verticalHeader()->hide();
    pWoundTable->setLeftMargin( 0 );
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
    pWoundTable->horizontalHeader()->setLabel( n, tr( "Psyop" ) );
    pWoundTable->setColumnStretchable( n, true );
    pWoundTable->horizontalHeader()->setLabel( n + 1, tr( "Contamination" ) );
    pWoundTable->setColumnStretchable( n + 1, true );

    pWoundTable->verticalHeader()->setLabel( 0, tr( "Treatment duration" ) );
    pWoundTable->verticalHeader()->setLabel( 1, tr( "Convalescence duration" ) );
    pWoundTable->verticalHeader()->setLabel( 2, tr( "Seriousness distribution (%)" ) );
    pWoundTable->verticalHeader()->setLabel( 3, tr( "Life span" ) );

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
    pWoundTable->setItem( 3, n, new Q3TableItem( pWoundTable, Q3TableItem::Never ) );

    builder.AddTableCell< ADN_TableItem_TimeField >( pWoundTable, &data_, 0, n + 1, data_.contaminationTreatTime_ );
    builder.AddTableCell< ADN_TableItem_TimeField >( pWoundTable, &data_, 1, n + 1, data_.contaminationRestingTime_ );
    pWoundTable->setItem( 2, n + 1, new Q3TableItem( pWoundTable, Q3TableItem::Never ) );
    pWoundTable->setItem( 3, n + 1, new Q3TableItem( pWoundTable, Q3TableItem::Never ) );

    return pWoundTable;
}
