// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Automata_GUI.h"
#include "ADN_MainWindow.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Automata_Data.h"
#include "ADN_Automata_ListView.h"
#include "ADN_Automata_SubUnitsTable.h"
#include "ADN_ComboBox_Vector.h"
#include "ADN_Tr.h"
#include "ADN_TimeField.h"
#include "ADN_AutomatLog_ListView.h"
#include "ADN_AutomatLogCategory_ListView.h"
#include "ADN_GoToButton.h"

// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI constructor
// Created: APE 2004-12-10
// -----------------------------------------------------------------------------
ADN_Automata_GUI::ADN_Automata_GUI( ADN_Automata_Data& data )
    : ADN_GUI_ABC( eAutomata )
    , data_         ( data )
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
    ADN_GuiBuilder builder( strClassName_ );
    T_ConnectorVector vInfosConnectors( eNbrGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    // Info holder
    QWidget* pInfoHolder = builder.AddFieldHolder( 0 );
    // Name
    builder.AddLocalizedField( data_.GetAutomata(), pInfoHolder, "name", tools::translate( "ADN_Automata_GUI", "Name" ), vInfosConnectors[eName] );
    // Automaton type
    builder.AddEnumField( pInfoHolder, "type", tools::translate( "ADN_Automata_GUI", "Type" ), vInfosConnectors[eAgentType] );
    // Model
    ADN_GoToButton* goToButton = new ADN_GoToButton( ::eModels, eEntityType_Automat );
    goToButton->SetLinkedCombo( builder.AddField< ADN_ComboBox_Vector >( pInfoHolder, "doctrine-model", tools::translate( "ADN_Automata_GUI", "Doctrine model" ), vInfosConnectors[eModel], 0, eNone, goToButton ) );
    // Unit
    builder.AddField< ADN_ComboBox_Vector >( pInfoHolder, "command-post", tools::translate( "ADN_Automata_GUI", "Command post" ), vInfosConnectors[eUnit] );
    // Feedback time
    builder.AddCheckableField<ADN_TimeField>( pInfoHolder, "force-ratio-feedback-time", tools::translate( "ADN_Automata_GUI", "Force ratio feedback time" ), vInfosConnectors[eHasFeedbackTime], vInfosConnectors[eFeedbackTime] );

    // Sub units
    QGroupBox* pSubUnitsGroup = new gui::RichGroupBox( "sub-units", tools::translate( "ADN_Automata_GUI", "Sub-units" ) );
    QVBoxLayout* pSubUnitsLayout = new QVBoxLayout( pSubUnitsGroup );
    ADN_Automata_SubUnitsTable* pSubUnitsTable = new ADN_Automata_SubUnitsTable( builder.GetChildName( "sub-units-table" ), vInfosConnectors[eSubUnit], pSubUnitsGroup );
    pSubUnitsTable->SetGoToOnDoubleClick( ::eUnits );
    pSubUnitsLayout->addWidget( pSubUnitsTable, 1 );

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
    QWidget* pSearchListView = builder.AddSearchListView< ADN_Automata_ListView >( this, data_.GetAutomata(), vInfosConnectors );

    // Main widget
    pMainWidget_ = CreateScrollArea( builder.GetName(), *pContent, pSearchListView );
}

namespace
{
    class ADN_CompositionTable : public ADN_Table
    {
    public:
        //! @name Constructors/Destructor
        //@{
        ADN_CompositionTable( const QString& objectName, QWidget* pParent = 0 )
            : ADN_Table( objectName, pParent )
        {
            dataModel_.setColumnCount( 3 );
            setSortingEnabled( false );
            setShowGrid( true );
            setSelectionMode( QAbstractItemView::NoSelection );
            QStringList horizontalHeaders;
            horizontalHeaders << tools::translate( "ADN_Automata_GUI", "Automata [ officers/warrant officers/soldiers ]" )
                              << tools::translate( "ADN_Automata_GUI", "Unit [ officers/warrant officers/soldiers ]" )
                              << tools::translate( "ADN_Automata_GUI", "Equipment [ officers/warrant officers/soldiers ]" );
            dataModel_.setHorizontalHeaderLabels( horizontalHeaders );
            horizontalHeader()->setResizeMode( QHeaderView::Stretch );
            horizontalHeader()->setResizeMode( 0, QHeaderView::ResizeToContents );
            verticalHeader()->setVisible( false );
            setAlternatingRowColors( false );
        }
        virtual ~ADN_CompositionTable() {}
        //@}
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI::CreateAutomataCompositionsTable
// Created: APE 2005-03-31
// -----------------------------------------------------------------------------
QWidget* ADN_Automata_GUI::CreateAutomataCompositionsTable()
{
    ADN_Table* pTable = new ADN_CompositionTable( std::string( std::string( strClassName_ ) + "automata-compositions-consistency-table" ).c_str() );
    pTable->AddBoldGridCol( 1 );
    pTable->AddBoldGridCol( 2 );
    // Fill the table.
    int nRow = 0;
    ADN_Automata_Data::T_AutomatonInfosVector automats = data_.vAutomata_;
    std::sort( automats.begin(), automats.end(), ADN_Tools::NameSort<ADN_Automata_Data::AutomatonInfos>() );
    for( auto it = automats.begin(); it != automats.end(); ++it )
    {
        ADN_Automata_Data::AutomatonInfos& automaton = **it;
        if( automaton.vSubUnits_.empty() )
            continue;
        pTable->setNumRows( std::max( pTable->numRows(), nRow + 1 ) );
        pTable->AddBoldGridRow( nRow );
        int nSubRow = 0;
        unsigned int nAutoOfficer = 0;
        unsigned int nAutoNCOfficer = 0;
        int  nAutoTroops = 0;

        for( auto it2 = automaton.vSubUnits_.begin(); nSubRow == 0 || it2 != automaton.vSubUnits_.end(); )
        {
            ADN_Automata_Data::UnitInfos* pUnit = ( nSubRow == 0 ) ? automaton.ptrUnit_.GetData() : *it2;
            if( !pUnit || !pUnit->GetCrossedElement() )
            {
                ++it2;
                continue;
            }
            if( nSubRow != 0  && pUnit == automaton.ptrUnit_.GetData() )
            {
                ++it2;
                continue;
            }
            ADN_Units_Data::UnitInfos& unit = *pUnit->GetCrossedElement();
            pTable->setNumRows( std::max( pTable->numRows(), nRow + nSubRow + 1 ) );

            int nSubSubRow = 0;
            int nTroops = 0;
            for( auto it3 = unit.vComposantes_.begin(); it3 != unit.vComposantes_.end(); ++it3, ++nSubSubRow )
            {
                ADN_Units_Data::ComposanteInfos* composantes = *it3;
                pTable->setNumRows( std::max( pTable->numRows(), nRow + nSubRow + nSubSubRow + 1 ) );
                QString strText = tools::translate( "ADN_Automata_GUI", "(%1..%2) x %3 x %4 [ %5 ]" );
                strText = strText.arg( pUnit->min_.GetData() )
                    .arg( pUnit->max_.GetData() )
                    .arg( composantes->nNb_.GetData() )
                    .arg( composantes->GetCrossedElement()->strName_.GetData().c_str() )
                    .arg( composantes->nNbrHumanInCrew_.GetData() );
                pTable->AddItem( nRow + nSubRow + nSubSubRow, 2, composantes, strText );
                nTroops += composantes->nNb_.GetData() * composantes->nNbrHumanInCrew_.GetData();
            }
            int nRowSpan = std::max( 1, static_cast< int >( unit.vComposantes_.size() ) );
            QString strText = tools::translate( "ADN_Automata_GUI", "(%1..%2) x %3 [ %4/%5/%6 ]" );
            strText = strText.arg( pUnit->min_.GetData() )
                .arg( pUnit->max_.GetData() )
                .arg( unit.strName_.GetData().c_str() )
                .arg( unit.nNbOfficer_.GetData() )
                .arg( unit.nNbNCOfficer_.GetData() )
                .arg( nTroops - unit.nNbOfficer_.GetData() - unit.nNbNCOfficer_.GetData() );
            uint nNbUnit = pUnit->min_.GetData();
            nAutoOfficer   += nNbUnit * unit.nNbOfficer_.GetData();
            nAutoNCOfficer += nNbUnit * unit.nNbNCOfficer_.GetData();
            nAutoTroops    += nNbUnit * ( nTroops - unit.nNbOfficer_.GetData() - unit.nNbNCOfficer_.GetData() );
            if( nSubRow == 0 )
                strText = strText + " ( " + tools::translate( "ADN_Automata_GUI", "CP" ) +" )";
            else
                ++it2;
            pTable->AddItem( nRow + nSubRow, 1, nRowSpan, 1, pUnit, strText );
            nSubRow += nRowSpan;
        }

        int nRowSpan = std::max( 1, nSubRow );
        QString strText;
        strText = tools::translate( "ADN_Automata_GUI", "%1 [ %2/%3/%4 ]" );
        strText = strText.arg( automaton.strName_.GetData().c_str() )
                         .arg( nAutoOfficer )
                         .arg( nAutoNCOfficer )
                         .arg( nAutoTroops );
        pTable->AddItem( nRow, 0, nRowSpan, 1, &automaton, strText );
        nRow += nRowSpan;
    }
    automats.clear(); // force clear here so ADN_Type_Vector_ABC won't delete each automat
    return pTable;
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI::CreateAutomataLogTable
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
QWidget* ADN_Automata_GUI::CreateAutomataLogTable()
{
    return new ADN_AutomatLog_ListView( data_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI::CreateAutomataLogTablePerDotation
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
QWidget* ADN_Automata_GUI::CreateAutomataLogTablePerDotation()
{
    return new ADN_AutomatLogCategory_ListView( data_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_GUI::RegisterTable
// Created: APE 2005-03-31
// -----------------------------------------------------------------------------
void ADN_Automata_GUI::RegisterTable( ADN_MainWindow& mainWindow )
{
    mainWindow.AddTable( tools::translate( "ADN_Automata_GUI", "Automata compositions" ), boost::bind( &ADN_Automata_GUI::CreateAutomataCompositionsTable, this ) );
    mainWindow.AddListView( tools::translate( "ADN_Automata_GUI", "Supplies per automaton" ), boost::bind( &ADN_Automata_GUI::CreateAutomataLogTable, this ) );
    mainWindow.AddListView( tools::translate( "ADN_Automata_GUI", "Supplies per type" ), boost::bind( &ADN_Automata_GUI::CreateAutomataLogTablePerDotation, this ) );
}
