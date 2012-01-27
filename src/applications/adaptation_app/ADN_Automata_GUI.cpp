// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-12-06 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Automata_GUI.cpp $
// $Author: Ape $
// $Modtime: 7/06/05 10:08 $
// $Revision: 18 $
// $Workfile: ADN_Automata_GUI.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Automata_GUI.h"
#include "moc_ADN_Automata_GUI.cpp"
#include "ADN_MainWindow.h"
#include "ADN_App.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Workspace.h"
#include "ADN_CommonGfx.h"
#include "ADN_Automata_Data.h"
#include "ADN_Connector_Vector_ABC.h"
#include "ADN_Automata_ListView.h"
#include "ADN_Automata_SubUnitsTable.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_GroupBox.h"
#include "ADN_SearchListView.h"
#include "ADN_Tr.h"
#include "ADN_TimeField.h"
#include "ADN_ListView.h"
#include "ADN_AutomatLog_ListView.h"
#include "ADN_AutomatLogCategory_ListView.h"
#include "UnitsFilter.h"

// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI constructor
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
ADN_Automata_GUI::ADN_Automata_GUI( ADN_Automata_Data& data )
    : ADN_GUI_ABC( "ADN_Automata_GUI" )
    , data_         ( data )
    , pAutomataList_( 0 )
    , pFilter_      ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI destructor
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
ADN_Automata_GUI::~ADN_Automata_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI::Build
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
void ADN_Automata_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder;
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    // Name
    builder.AddField<ADN_EditLine_String>( pInfoHolder, tr( "Name" ), vInfosConnectors[eName] );
    // Automaton type
    builder.AddEnumField<E_AgentTypeAutomate>( pInfoHolder, tr( "Type" ), vInfosConnectors[eAgentType], ADN_Tr::ConvertFromAgentTypeAutomate );
    // Model
    builder.AddField< ADN_ComboBox_Vector<ADN_Models_Data::ModelInfos> >( pInfoHolder, tr( "Doctrine model" ), vInfosConnectors[eModel] );
    // Unit
    pFilter_ = builder.AddField< UnitsFilter >( pInfoHolder, tr( "Command post" ), vInfosConnectors[eUnit] );
    // Feedback time
    builder.AddOptionnalField<ADN_TimeField>( pInfoHolder, tr( "Force ratio feedback time" ), vInfosConnectors[eHasFeedbackTime], vInfosConnectors[eFeedbackTime] );

    // Sub units
    QGroupBox* pSubUnitsGroup = new QGroupBox( tr( "Sub-units" ) );
    QVBoxLayout* pSubUnitsLayout = new QVBoxLayout( pSubUnitsGroup );
    ADN_Automata_SubUnitsTable* pSubUnitsTable = new ADN_Automata_SubUnitsTable( pSubUnitsGroup );
    pSubUnitsLayout->addWidget( pSubUnitsTable, 1 );
    vInfosConnectors[eSubUnit] = &pSubUnitsTable->GetConnector();
    connect( pSubUnitsTable, SIGNAL( AddItem( const std::string& ) ), this, SLOT( OnItemAdded( const std::string& ) ) );
    connect( pSubUnitsTable, SIGNAL( RemoveItem( const std::string& ) ), this, SLOT( OnItemRemoved( const std::string& ) ) );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    // Content layout
    QWidget* pContent = new QWidget();
    QVBoxLayout* pContentLayout = new QVBoxLayout( pContent );
    pContentLayout->setMargin( 10 );
    pContentLayout->setSpacing( 10 );
    pContentLayout->setAlignment( Qt::AlignTop );
    pContentLayout->addWidget( pInfoHolder );
    pContentLayout->addWidget( pSubUnitsGroup, 1 );

    // List view
    ADN_SearchListView< ADN_Automata_ListView >* pSearchListView = new ADN_SearchListView< ADN_Automata_ListView >( data_.GetAutomata(), vInfosConnectors );
    connect( this, SIGNAL( ApplyFilterList( const ADN_UsedByInfos& ) ), pSearchListView, SLOT( OnApplyFilterList( const ADN_UsedByInfos& ) ) );
    pAutomataList_ = pSearchListView->GetListView();

    // Main widget
    pMainWidget_ = CreateScrollArea( *pContent, pSearchListView );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI::CreateAutomataCompositionsTable
// Created: APE 2005-03-31
// -----------------------------------------------------------------------------
ADN_Table* ADN_Automata_GUI::CreateAutomataCompositionsTable()
{
    ADN_GuiBuilder builder;
    ADN_Table* pTable = builder.CreateTable( 0 );

    // Setup the header.
    pTable->setNumCols( 3 );
    pTable->horizontalHeader()->setLabel( 0, tr( "Automata [ officers/warrant officers/soldiers ]" ) );
    pTable->horizontalHeader()->setLabel( 1, tr( "Unit [ officers/warrant officers/soldiers ]" ) );
    pTable->horizontalHeader()->setLabel( 2, tr( "Equipment [ officers/warrant officers/soldiers ]" ) );
    pTable->horizontalHeader()->show();

    pTable->setNumRows( 1 );
    builder.AddTableCell( pTable, 0, 0, tr( "Automata [ officers/warrant officers/soldiers ]" ) );
    builder.AddTableCell( pTable, 0, 1, tr( "Unit [ officers/warrant officers/soldiers ]" ) );
    builder.AddTableCell( pTable, 0, 2, tr( "Equipment [ officers/warrant officers/soldiers ]" ) );
    pTable->hideRow( 0 );
    pTable->AddBoldGridRow( 0 );
    pTable->sortColumn( 0, true, true );
    pTable->setSorting( false );

    // Fill the table.
    int nRow = 1;
    std::sort( data_.vAutomata_.begin(), data_.vAutomata_.end(), ADN_Tools::NameSort<ADN_Automata_Data::AutomatonInfos>() );
    for( ADN_Automata_Data::IT_AutomatonInfosVector it = data_.vAutomata_.begin(); it != data_.vAutomata_.end(); ++it )
    {
        ADN_Automata_Data::AutomatonInfos& automaton = **it;
        pTable->setNumRows( std::max( pTable->numRows(), nRow + 1 ) );
        pTable->AddBoldGridRow( nRow );
        builder.AddTableCell( pTable, nRow, 1 );
        builder.AddTableCell( pTable, nRow, 2 );

        int nSubRow = 0;

        uint nAutoOfficer   = 0;
        uint nAutoNCOfficer = 0;
        int  nAutoTroops    = 0;

        for( ADN_Automata_Data::IT_UnitInfosVector it2 = automaton.vSubUnits_.begin(); nSubRow == 0 || it2 != automaton.vSubUnits_.end(); )
        {
            ADN_Automata_Data::UnitInfos* pUnitInfos = 0;
            ADN_Units_Data::UnitInfos*    pUnit;

            if( nSubRow == 0 )
            {
                pUnit = automaton.ptrUnit_.GetData();
            }
            else
            {
                pUnitInfos = *it2;
                assert( pUnitInfos->ptrUnit_.GetData() != 0 );
                pUnit = pUnitInfos->ptrUnit_.GetData();
            }
            ADN_Units_Data::UnitInfos&    unit      = * pUnit;
            pTable->setNumRows( std::max( pTable->numRows(), nRow + nSubRow + 1 ) );

            int nSubSubRow = 0;
            int nTroops = 0;
            for( ADN_Units_Data::IT_ComposanteInfos_Vector it3 = unit.vComposantes_.begin(); it3 != unit.vComposantes_.end(); ++it3, ++nSubSubRow )
            {
                pTable->setNumRows( std::max( pTable->numRows(), nRow + nSubRow + nSubSubRow + 1 ) );

                QString strText;
                if( pUnitInfos != 0 )
                {
                    strText = tr( "(%1..%2) x %3 x %4 [ %5 ]" );
                    strText = strText.arg( pUnitInfos->min_.GetData() )
                                     .arg( pUnitInfos->max_.GetData() )
                                     .arg( (*it3)->nNb_.GetData() )
                                     .arg( (*it3)->ptrComposante_.GetData()->strName_.GetData().c_str() )
                                     .arg( (*it3)->nNbrHumanInCrew_.GetData() );
                }
                else
                {
                    strText = tr( "%1 x %2 [ %3 ]" );
                    strText = strText.arg( (*it3)->nNb_.GetData() )
                                     .arg( (*it3)->ptrComposante_.GetData()->strName_.GetData().c_str() )
                                     .arg( (*it3)->nNbrHumanInCrew_.GetData() );
                }
                builder.AddTableCell( pTable, nRow + nSubRow + nSubSubRow, 2, strText );
                nTroops += (*it3)->nNb_.GetData() * (*it3)->nNbrHumanInCrew_.GetData();
            }
            int nRowSpan = std::max( 1, (int)unit.vComposantes_.size() );
            QString strText;
            if( pUnitInfos != 0 )
            {
                strText = tr( "(%1..%2) x %3 [ %4/%5/%6 ]" );
                strText = strText.arg( pUnitInfos->min_.GetData() )
                                 .arg( pUnitInfos->max_.GetData() )
                                 .arg( unit.strName_.GetData().c_str() )
                                 .arg( pUnit->nNbOfficer_.GetData() )
                                 .arg( pUnit->nNbNCOfficer_.GetData() )
                                 .arg( nTroops - pUnit->nNbOfficer_.GetData() - pUnit->nNbNCOfficer_.GetData() );
                uint nNbUnit = pUnitInfos->min_.GetData();
                nAutoOfficer   += nNbUnit * pUnit->nNbOfficer_.GetData();
                nAutoNCOfficer += nNbUnit * pUnit->nNbNCOfficer_.GetData();
                nAutoTroops    += nNbUnit * ( nTroops - pUnit->nNbOfficer_.GetData() - pUnit->nNbNCOfficer_.GetData() );
            }
            else
            {
                strText = tr( "%1 [ %2/%3/%4 ]" );
                strText = strText.arg( unit.strName_.GetData().c_str() )
                                 .arg( pUnit->nNbOfficer_.GetData() )
                                 .arg( pUnit->nNbNCOfficer_.GetData() )
                                 .arg( nTroops - pUnit->nNbOfficer_.GetData() - pUnit->nNbNCOfficer_.GetData() );
                nAutoOfficer   += pUnit->nNbOfficer_.GetData();
                nAutoNCOfficer += pUnit->nNbNCOfficer_.GetData();
                nAutoTroops    += nTroops - pUnit->nNbOfficer_.GetData() - pUnit->nNbNCOfficer_.GetData();
            }
            builder.AddTableCell( pTable, nRow + nSubRow, 1, nRowSpan, 1, strText );

            if( nSubRow > 0 )
                ++it2;
            nSubRow += nRowSpan;
        }

        int nRowSpan = std::max( 1, nSubRow );
        QString strText;
        strText = tr( "%1 [ %2/%3/%4 ]" );
        strText = strText.arg( automaton.strName_.GetData().c_str() )
                         .arg( nAutoOfficer )
                         .arg( nAutoNCOfficer )
                         .arg( nAutoTroops );
        builder.AddTableCell( pTable, nRow, 0, nRowSpan, 1, strText );
        nRow += nRowSpan;
    }

    pTable->AdjustColumns( 50 );
    return pTable;
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI::CreateAutomataLogTable
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
ADN_ListView* ADN_Automata_GUI::CreateAutomataLogTable()
{
    return new ADN_AutomatLog_ListView( data_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI::CreateAutomataLogTablePerDotation
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
ADN_ListView* ADN_Automata_GUI::CreateAutomataLogTablePerDotation()
{
    return new ADN_AutomatLogCategory_ListView( data_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI::RegisterTable
// Created: APE 2005-03-31
// -----------------------------------------------------------------------------
void ADN_Automata_GUI::RegisterTable( ADN_MainWindow& mainWindow )
{
    mainWindow.AddTable   ( tr( "Automata compositions" ), new ADN_Callback<ADN_Table*   ,ADN_Automata_GUI>( this, &ADN_Automata_GUI::CreateAutomataCompositionsTable ) );
    mainWindow.AddListView( tr( "Logistic per automat" ) , new ADN_Callback<ADN_ListView*,ADN_Automata_GUI>( this, &ADN_Automata_GUI::CreateAutomataLogTable ) );
    mainWindow.AddListView( tr( "Logistic per resource" ), new ADN_Callback<ADN_ListView*,ADN_Automata_GUI>( this, &ADN_Automata_GUI::CreateAutomataLogTablePerDotation ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI::OnItemAdded
// Created: LGY 2011-11-04
// -----------------------------------------------------------------------------
void ADN_Automata_GUI::OnItemAdded( const std::string& name )
{
    pFilter_->Add( name );
    pAutomataList_->Update();
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI::OnItemRemoved
// Created: LGY 2011-11-04
// -----------------------------------------------------------------------------
void ADN_Automata_GUI::OnItemRemoved( const std::string& name )
{
    pFilter_->Remove( name );
    pAutomataList_->Update();
}
