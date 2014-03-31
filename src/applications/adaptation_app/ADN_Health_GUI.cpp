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
#include "moc_ADN_Health_GUI.cpp"
#include "ADN_Health_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_Resources_GUI.h"
#include "ADN_Tr.h"
#include "ADN_Table.h"
#include "ADN_TimeField.h"
#include "ADN_AvailabilityWarningTable.h"

namespace
{

class ADN_WoundTable : public ADN_Table
{
public:
    explicit ADN_WoundTable( const QString& objectName, void* data, QWidget* pParent = 0 )
        : ADN_Table( objectName, pParent )
    {
        setSortingEnabled( true );
        dataModel_.setColumnCount( eNbrDoctorSkills + 2 );
        dataModel_.setRowCount( 5 );

        QStringList horizontalHeaders;
        int n = 0;
        for( n = 0; n < eNbrDoctorSkills; ++n )
            horizontalHeaders << ADN_Tr::ConvertFromDoctorSkills( (E_DoctorSkills)n ).c_str();
        horizontalHeaders << tools::translate( "ADN_WoundTable", "Psyop" ) << tools::translate( "ADN_WoundTable", "Contamination" );
        dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
        horizontalHeader()->setResizeMode( QHeaderView::Stretch );

        QStringList verticalHeaders;
        verticalHeaders << tools::translate( "ADN_WoundTable", "Treatment duration" ) 
                        << tools::translate( "ADN_WoundTable", "Convalescence duration" ) 
                        << tools::translate( "ADN_WoundTable", "Seriousness distribution (%)" )
                        << tools::translate( "ADN_WoundTable", "Life span" )
                        << tools::translate( "ADN_WoundTable", "Return after treatment" );
        dataModel_.setVerticalHeaderLabels( verticalHeaders );
        delegate_.AddDelayEditOnRow( 0 );
        delegate_.AddDelayEditOnRow( 1 );
        delegate_.AddDoubleSpinBoxOnRow( 2, 0, 100 );
        delegate_.AddDelayEditOnRow( 3, 100u );
        delegate_.AddCheckBoxOnRow( 4 );

        ADN_Health_Data* health = static_cast< ADN_Health_Data* >( data );
        if( !health )
            return;
        for( n = 0 ; n < eNbrDoctorSkills; ++n )
        {
            ADN_Health_Data::WoundInfo& wound = health->wounds[n];
            AddItem( 0, n, data, &wound.treatTime_, ADN_StandardItem::eDelay, Qt::ItemIsEditable );
            AddItem( 1, n, data, &wound.restingTime_, ADN_StandardItem::eDelay, Qt::ItemIsEditable );
            AddItem( 2, n, data, &wound.rPercentage_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
            AddItem( 3, n, data, &wound.lifeExpectancy_, ADN_StandardItem::eDelay, Qt::ItemIsEditable );
            auto item = AddItem( 4, n, data, &wound.goBackToWar_, ADN_StandardItem::eBool );
            item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable );
        }
        AddItem( 0, n, data, &health->shockTreatTime_, ADN_StandardItem::eDelay, Qt::ItemIsEditable );
        AddItem( 1, n, data, &health->shockRestingTime_, ADN_StandardItem::eDelay, Qt::ItemIsEditable );
        AddItem( 2, n, data, &health->rShockPercentage_, ADN_StandardItem::eDouble, Qt::ItemIsEditable );
        AddItem( 3, n, data, QString() );
        auto item = AddItem( 4, n, data, &health->shockGoBackToWar_, ADN_StandardItem::eBool );
        item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable );

        ++n;
        AddItem( 0, n, data, &health->contaminationTreatTime_, ADN_StandardItem::eDelay, Qt::ItemIsEditable );
        AddItem( 1, n, data, &health->contaminationRestingTime_, ADN_StandardItem::eDelay, Qt::ItemIsEditable );
        AddItem( 2, n, data, QString() );
        AddItem( 3, n, data, QString() );
        item = AddItem( 4, n, data, &health->contaminatedGoBackToWar_, ADN_StandardItem::eBool );
        item->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable );
    }

protected slots:
    virtual void OnItemChanged( QStandardItem* item )
    {
        delegate_.OnItemChanged( item );
        ADN_Workspace::GetWorkspace().GetResources().GetGui().UpdateGraph();
    }
};

}

// -----------------------------------------------------------------------------
// Name: ADN_Health_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Health_GUI::ADN_Health_GUI( ADN_Health_Data& data )
    : ADN_GUI_ABC( eLogistic )
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
    ADN_GuiBuilder builder( strClassName_ );
    builder.PushSubName( "health-tab" );

    // Health
    QWidget* pHolder = builder.AddFieldHolder( 0 );
    builder.AddField< ADN_TimeField >( pHolder, "diagnostic-duration", tr( "Diagnostic duration" ), data_.diagnosticTime_ );
    builder.AddField< ADN_TimeField >( pHolder, "triage-duration", tr( "Triage duration" ), data_.sortingTime_ );
    builder.AddField< ADN_EditLine_Double >( pHolder, "diagnosis-life-span-factor", tr( "Diagnosis life span factor" ), data_.diagnosisLifeExpectancyFactor_, 0, eGreaterZero );
    builder.AddStretcher( pHolder, Qt::Vertical );

    // wounds
    ADN_WoundTable* woundTable = new ADN_WoundTable( builder.GetChildName( "wounds-table" ), &data_ );

    // Warning tables
    Q3HGroupBox* pEvacuationBox = new Q3HGroupBox( tr( "Ambulances (evacuation) availability warnings" ) );
    new ADN_AvailabilityWarningTable( builder.GetChildName( "evacuation-table" ), data_.vChangeOverWarnings_, pEvacuationBox );

    Q3HGroupBox* pCollectionBox = new Q3HGroupBox( tr( "Ambulances (collection) availability warnings" ) );
    new ADN_AvailabilityWarningTable( builder.GetChildName( "collect-table" ), data_.vCollectingWarnings_, pCollectionBox );

    Q3HGroupBox* pDoctorsBox = new Q3HGroupBox( tr( "Doctors availability warnings" ) );
    new ADN_AvailabilityWarningTable( builder.GetChildName( "doctors-table" ), data_.vDoctorsWarnings_, pDoctorsBox );

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
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent );
    builder.PopSubName(); //  "health-tab"
}
