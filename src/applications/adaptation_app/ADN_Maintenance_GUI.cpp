// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Maintenance_GUI.h"
#include "ADN_Maintenance_Data.h"
#include "ADN_GuiBuilder.h"
#include "ADN_EditLine.h"
#include "ADN_Table.h"
#include "ADN_AvailabilityWarningTable.h"
#include "ENT/ENT_Tr.h"

namespace
{
    class ADN_WorkingSchemeTable : public ADN_Table
    {
    public:
        ADN_WorkingSchemeTable( const QString& objectName, QWidget* pParent = 0 )
            : ADN_Table( objectName, pParent )
        {
            setSortingEnabled( false );

            delegate_.AddSpinBoxOnRow( 0, 1, std::numeric_limits< int >::max(), 1 );
            delegate_.AddDelayEditOnRow( 1 );
        }
        virtual ~ADN_WorkingSchemeTable()
        {
            // NOTHING
        }

        void Initialize( ADN_Maintenance_Data& data )
        {
            dataModel_.setColumnCount( static_cast< int >( data.vWorkingSchemes_.size() ) );
            QStringList horizontalHeaders;
            for( ADN_Maintenance_Data::IT_WorkingSchemeInfo_Vector it = data.vWorkingSchemes_.begin(); it != data.vWorkingSchemes_.end(); ++it )
                horizontalHeaders << tools::translate( "ADN_WorkingSchemeTable", "Shift " ) + QString::number( ( **it ).nIdx_.GetData() );
            dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
            horizontalHeader()->setResizeMode( QHeaderView::Stretch );

            dataModel_.setRowCount( 2 );
            QStringList verticalHeaders;
            verticalHeaders << tools::translate( "ADN_WorkingSchemeTable", "Work duration (h)" )
                            << tools::translate( "ADN_WorkingSchemeTable", "Warning after" );
            dataModel_.setVerticalHeaderLabels( verticalHeaders );

            int n = 0;
            for( ADN_Maintenance_Data::IT_WorkingSchemeInfo_Vector it = data.vWorkingSchemes_.begin(); it != data.vWorkingSchemes_.end(); ++it, ++n )
            {
                ADN_Maintenance_Data::WorkingSchemeInfo& modifiers = **it;
                AddItem( 0, n, &modifiers, &modifiers.nWorkTime_, ADN_StandardItem::eInt, Qt::ItemIsEditable );
                AddItem( 1, n, &modifiers, &modifiers.warningDelay_, ADN_StandardItem::eDelay, Qt::ItemIsEditable );
            }
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_GUI constructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Maintenance_GUI::ADN_Maintenance_GUI( ADN_Maintenance_Data& data )
    : ADN_GUI_ABC( eLogistic )
    , data_      ( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_GUI destructor
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
ADN_Maintenance_GUI::~ADN_Maintenance_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Maintenance_GUI::Build
// Created: APE 2005-03-21
// -----------------------------------------------------------------------------
void ADN_Maintenance_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );

    // Maintenance
    ADN_GuiBuilder builder( strClassName_ );
    builder.PushSubName( "maintenance-tab" );
    Q3GroupBox* workingShemeBox = new Q3HGroupBox( tr( "Shifts durations" ) );
    workingShemeBox->setFixedHeight( 200 );
    ADN_WorkingSchemeTable* pWorkingSchemeTable = new ADN_WorkingSchemeTable( builder.GetChildName( "durations-table" ), workingShemeBox );
    pWorkingSchemeTable->Initialize( data_ );

    //vRepairerWarnings_
    Q3HGroupBox* pRepairerGroup = new Q3HGroupBox( tr( "Repairers availability warnings" ) );
    new ADN_AvailabilityWarningTable( builder.GetChildName( "repairers-table" ), data_.vRepairerWarnings_, pRepairerGroup );

    //vHaulerWarnings_
    Q3HGroupBox* pHaulerGroup = new Q3HGroupBox( tr( "Tow trucks availability warnings" ) );
    new ADN_AvailabilityWarningTable( builder.GetChildName( "haulers-table" ), data_.vHaulerWarnings_, pHaulerGroup );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Bottom layout
    QHBoxLayout* pBottomLayout = new QHBoxLayout();
    pBottomLayout->setMargin( 0 );
    pBottomLayout->setSpacing( 10 );
    pBottomLayout->addWidget( pRepairerGroup );
    pBottomLayout->addWidget( pHaulerGroup );

    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( workingShemeBox );
    pContentLayout->addLayout( pBottomLayout );
    pContentLayout->addStretch( 1 );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent );
}

