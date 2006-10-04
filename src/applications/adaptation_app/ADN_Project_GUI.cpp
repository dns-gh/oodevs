//*****************************************************************************
//
// $Created: JDY 03-06-23 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Project_GUI.cpp $
// $Author: Ape $
// $Modtime: 25/04/05 12:04 $
// $Revision: 10 $
// $Workfile: ADN_Project_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Project_GUI.h"

#include "ADN_App.h"
#include "ADN_GuiBuilder.h"
#include "ADN_CommonGfx.h"
#include "ADN_FileChooser.h"
#include "ADN_Project_Data.h"
#include "ADN_TimeField.h"

#include <qlayout.h>
#include <qgroupbox.h>

#define XML_FILTER "Xml files (*.xml)"


//-----------------------------------------------------------------------------
// Name: ADN_Project_GUI constructor
// Created: JDY 03-06-23
//-----------------------------------------------------------------------------
ADN_Project_GUI::ADN_Project_GUI( ADN_Project_Data& data )
: ADN_GUI_ABC( "ADN_Project_GUI" )
, data_      ( data )
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Project_GUI destructor
// Created: JDY 03-06-23
//-----------------------------------------------------------------------------
ADN_Project_GUI::~ADN_Project_GUI()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Project_GUI::Build
// Created: JDY 03-06-23
//-----------------------------------------------------------------------------
void ADN_Project_GUI::Build()
{
    assert( pMainWidget_ == 0 );
    
    ADN_GuiBuilder builder;

     // Create the top level widget.
    pMainWidget_ = new QWidget( 0, "project main widget" );

    // Project
    QGroupBox* pProjectGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Project" ), pMainWidget_ );

    builder.AddField<ADN_EditLine_String>( pProjectGroup, tr( "Project file" ), data_.szFile_.GetFileName() );
    builder.SetEnabled( false );

    builder.AddField<ADN_EditLine_String>( pProjectGroup, tr( "Working Directory" ), ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory() );
    builder.SetEnabled( false );

    // Files
    QGroupBox* pFilesGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Files" ), pMainWidget_ );

    builder.AddFileField( pFilesGroup, tr( "Terrain" ), data_.dataInfos_.szTerrain_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Decisionnal" ), data_.dataInfos_.szDecisional_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Network" ), data_.dataInfos_.szNetwork_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Sizes" ), data_.dataInfos_.szSizes_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Armors" ), data_.dataInfos_.szArmors_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Objects" ), data_.dataInfos_.szObjects_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Equipements" ), data_.dataInfos_.szEquipements_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Launchers" ), data_.dataInfos_.szLaunchers_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Weapons" ), data_.dataInfos_.szWeapons_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Sensors" ), data_.dataInfos_.szSensors_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Components" ), data_.dataInfos_.szComponents_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Units" ), data_.dataInfos_.szUnits_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Automatas" ), data_.dataInfos_.szAutomata_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "NBC" ), data_.dataInfos_.szNBC_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Weather" ), data_.dataInfos_.szWeather_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Health" ), data_.dataInfos_.szHealth_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "IDs" ), data_.dataInfos_.szIDs_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Human factors" ), data_.dataInfos_.szHumanFactors_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Breakdowns" ), data_.dataInfos_.szBreakdowns_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Knowledge groups" ), data_.dataInfos_.szKnowledgeGroups_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Maintenance" ), data_.dataInfos_.szMaintenance_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Supply" ), data_.dataInfos_.szSupply_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Com" ), data_.dataInfos_.szCom_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "ODB" ), data_.dataInfos_.szODB_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Pathfind" ), data_.dataInfos_.szPathfinder_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Missions" ), data_.dataInfos_.szMissions_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "HLA" ), data_.dataInfos_.szHLA_, XML_FILTER );
    builder.AddFileField( pFilesGroup, tr( "Populations" ), data_.dataInfos_.szPopulation_, XML_FILTER );

    // Network
    QGroupBox* pNetworkGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Network" ), pMainWidget_ );

    builder.AddField<ADN_EditLine_Int>( pNetworkGroup, tr( "Server port" ), data_.netInfos_.nServerPort_ );
    builder.AddField<ADN_EditLine_Int>( pNetworkGroup, tr( "Server magic" ), data_.netInfos_.nServerMagic_ );
    builder.AddField<ADN_CheckBox>( pNetworkGroup, tr( "Network thread active" ), data_.netInfos_.bNetworkThreadActive_ );

    // Pathfinder
    QGroupBox* pPathfinderGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Pathfinder" ), pMainWidget_ );

    builder.AddField<ADN_EditLine_Int>( pPathfinderGroup, tr( "Pathfinder threads" ), data_.pathfinderInfo_.nPathfinderNbr_, 0, eGreaterZero );
    builder.AddField<ADN_EditLine_Int>( pPathfinderGroup, tr( "Distance threshold" ), data_.pathfinderInfo_.nDistanceThreshold_ );
    builder.AddField<ADN_TimeField>( pPathfinderGroup, tr( "Max computation time" ), data_.pathfinderInfo_.maxComputationTime_ );

    // SIM
    QGroupBox* pSimGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Sim" ), pMainWidget_ );
    QWidget* pHolder = builder.AddFieldHolder( pSimGroup );
    builder.AddField<ADN_CheckBox>( pHolder, tr( "Auto start" ), data_.simInfos_.bAutoStart_ );
    builder.AddField<ADN_EditLine_Int>( pHolder, tr( "Time step" ), data_.simInfos_.nTimeStep_, tr( "s" ), eGreaterZero );
    builder.AddField<ADN_EditLine_Int>( pHolder, tr( "Time factor" ), data_.simInfos_.nTimeFactor_, 0, eGreaterZero );
    QGroupBox* pSavesGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Saves" ), pSimGroup );
    builder.AddField<ADN_EditLine_Int>( pSavesGroup, tr( "Interval" ), data_.simInfos_.nAutosaveTime_, tr( "min" ), eGreaterZero );
    builder.AddField<ADN_EditLine_Int>( pSavesGroup, tr( "Max nbr" ), data_.simInfos_.nNbrMax_, 0, eGreaterZero );


    // Layout
    QGridLayout* pMainLayout = new QGridLayout( pMainWidget_, 1, 2, 10, 10 );
    pMainLayout->setAlignment( Qt::AlignTop );
    pMainLayout->addMultiCellWidget( pFilesGroup, 0, 5, 0, 0, Qt::AlignTop );
    pMainLayout->addWidget( pProjectGroup, 0, 1, Qt::AlignTop );
    pMainLayout->addWidget( pPathfinderGroup, 1, 1, Qt::AlignTop );
    pMainLayout->addWidget( pNetworkGroup, 2, 1, Qt::AlignTop );
    pMainLayout->addWidget( pSimGroup, 3, 1, Qt::AlignTop );
    pMainLayout->setRowStretch( 5, 10000 );
    pLayout_ = pMainLayout;
}


// -----------------------------------------------------------------------------
// Name: ADN_Project_GUI::AddHLAWidget
// Created: APE 2005-04-25
// -----------------------------------------------------------------------------
void ADN_Project_GUI::AddHLAWidget( QWidget& hlaWidget )
{
    hlaWidget.reparent( pMainWidget_, QPoint( 0, 0 ) );
    pLayout_->addWidget( &hlaWidget, 4, 1, Qt::AlignTop );
}
