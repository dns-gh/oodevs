// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
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

#include "ADN_pch.h"
#include "ADN_Automata_GUI.h"

#include <qlabel.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qdialog.h>

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
#include "ADN_Tr.h"
#include "ADN_TimeField.h"
#include "ADN_ListView.h"
#include "ADN_AutomatLog_ListView.h"
#include "ADN_AutomatLogCategory_ListView.h"


// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI constructor
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
ADN_Automata_GUI::ADN_Automata_GUI( ADN_Automata_Data& data )
: ADN_GUI_ABC( "ADN_Automata_GUI" )
, data_      ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI destructor
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
ADN_Automata_GUI::~ADN_Automata_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI::Build
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
void ADN_Automata_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the top widget.
    pMainWidget_ = new QWidget( 0, "Weapons main widget" );

    // Create the automata listview.
    ADN_Automata_ListView* pAutomataList = new ADN_Automata_ListView( pMainWidget_ );
    pAutomataList->GetConnector().Connect( &data_.GetAutomata() );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    QGroupBox* pGroup = new QGroupBox( 0, Qt::Horizontal, tr( "Automata" ), pMainWidget_ );

    QWidget* pPropertiesGroup = builder.AddFieldHolder( pGroup );

    // Name
    builder.AddField<ADN_EditLine_String>( pPropertiesGroup, tr( "Name" ), vInfosConnectors[eName] );

    // Automaton type
    builder.AddEnumField<E_AgentTypeAutomate>( pPropertiesGroup, tr( "Type" ), vInfosConnectors[eAgentType], ADN_Tr::ConvertFromAgentTypeAutomate );

    // Model
    builder.AddField< ADN_ComboBox_Vector<ADN_Models_Data::ModelInfos> >( pPropertiesGroup, tr( "Model" ), vInfosConnectors[eModel] );

    // Unit
    builder.AddField< ADN_ComboBox_Vector<ADN_Units_Data::UnitInfos> >( pPropertiesGroup, tr( "Command post" ), vInfosConnectors[eUnit] );

    // Feedback time
    builder.AddOptionnalField<ADN_TimeField>( pPropertiesGroup, tr( "Force ratio feedback time" ), vInfosConnectors[eHasFeedbackTime], vInfosConnectors[eFeedbackTime] );

    QGroupBox* pSubUnitsGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Sub-units" ), pGroup );
    ADN_Automata_SubUnitsTable* pSubUnitsTable = new ADN_Automata_SubUnitsTable( pSubUnitsGroup );
    vInfosConnectors[eSubUnit] = &pSubUnitsTable->GetConnector();

    pAutomataList->SetItemConnectors( vInfosConnectors );

    // Layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pAutomataList, 1 );
    pMainLayout->addWidget( pGroup, 3 );

    QVBoxLayout* pGroupLayout = new QVBoxLayout( pGroup->layout(), 5 );
    pGroupLayout->addWidget( pPropertiesGroup, 0, 0 );
    pGroupLayout->addWidget( pSubUnitsGroup, 1, 0 );
    builder.AddStretcher( pGroupLayout, Qt::Vertical );
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
    pTable->horizontalHeader()->setLabel( 0, tr( "Automata [ off/sic/troop ]" ) );
    pTable->horizontalHeader()->setLabel( 1, tr( "Unit [ off/sic/troop ]" ) );
    pTable->horizontalHeader()->setLabel( 2, tr( "Composante [ off/sic/troop ]" ) );
    pTable->horizontalHeader()->show();

    pTable->setNumRows( 1 );
    builder.AddTableCell( pTable, 0, 0, tr( "Automata [ off/sic/troop ]" ) );
    builder.AddTableCell( pTable, 0, 1, tr( "Unit [ off/sic/troop ]" ) );
    builder.AddTableCell( pTable, 0, 2, tr( "Composante [ off/sic/troop ]" ) );
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
                    strText = tr( "%1 x %2 x %3 [ %4 ]" );
                    strText = strText.arg( pUnitInfos->strNbrRegExp_.GetData().c_str() )
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
                strText = tr( "%1 x %2 [ %3/%4/%5 ]" );
                strText = strText.arg( pUnitInfos->strNbrRegExp_.GetData().c_str(), unit.strName_.GetData().c_str() )
                                 .arg( pUnit->nNbOfficer_.GetData() )
                                 .arg( pUnit->nNbNCOfficer_.GetData() )
                                 .arg( nTroops - pUnit->nNbOfficer_.GetData() - pUnit->nNbNCOfficer_.GetData() );
                uint nNbUnit = QString( pUnitInfos->strNbrRegExp_.GetData().c_str() ).toUInt();
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
    mainWindow.AddListView( tr( "Logistic per dotation" ), new ADN_Callback<ADN_ListView*,ADN_Automata_GUI>( this, &ADN_Automata_GUI::CreateAutomataLogTablePerDotation ) );
}
