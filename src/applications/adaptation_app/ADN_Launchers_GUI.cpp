//*****************************************************************************
//
// $Created: JDY 03-07-11 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Launchers_GUI.cpp $
// $Author: Ape $
// $Modtime: 21/04/05 11:57 $
// $Revision: 10 $
// $Workfile: ADN_Launchers_GUI.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Launchers_GUI.h"
#include "ADN_App.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Workspace.h"
#include "ADN_CommonGfx.h"
#include "ADN_Launchers_Data.h"
#include "ADN_ListView_Launchers.h"
#include "ADN_Launchers_ModifPhs_GUI.h"
#include "ADN_GroupBox.h"
#include "ADN_HtmlBuilder.h"


//-----------------------------------------------------------------------------
// Name: ADN_Launchers_GUI constructor
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
ADN_Launchers_GUI::ADN_Launchers_GUI( ADN_Launchers_Data& data )
: ADN_GUI_ABC( "ADN_Launchers_GUI" )
, data_      ( data )
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Launchers_GUI destructor
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
ADN_Launchers_GUI::~ADN_Launchers_GUI()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Launchers_GUI::Build
// Created: JDY 03-07-11
//-----------------------------------------------------------------------------
void ADN_Launchers_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;

    // Create the main widget.
    pMainWidget_ = new QWidget( 0, "frame launchers" );

    // Launcher listview
    pLaunchers_ = new ADN_ListView_Launchers( pMainWidget_ );
    pLaunchers_->GetConnector().Connect(&data_.GetLaunchersInfos());
    T_ConnectorVector vConnectors( eNbrGuiElements, (ADN_Connector_ABC*)0 );

    // Launcher data
    Q3GroupBox* pGroup = new Q3GroupBox( 1, Qt::Horizontal, tr( "Launcher" ), pMainWidget_ );

    QWidget* pHolder = builder.AddFieldHolder( pGroup );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "Name" ), vConnectors[eName] );

    // launcher modificator
    ADN_GroupBox* pDirectGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Direct fire" ), pGroup );
    vConnectors[eDirect] = &pDirectGroup->GetConnector();

    Q3GroupBox* pGroupModificators = new Q3HGroupBox( tr( "Phs modifiers" ), pDirectGroup );

    // modificators headers
    QLabel* pLabel = new QLabel( tr( "Shooter's\nstance" ), pGroupModificators );
    pLabel->setAlignment( Qt::AlignVCenter );
    Q3VBox* pBox = new Q3VBox( pGroupModificators );
    pLabel = new QLabel( tr( "Target's stance" ), pBox );
    pLabel->setAlignment( Qt::AlignHCenter );

    // modificators tab
    pModifPhs_ = new ADN_Launchers_ModifPhs_GUI( pBox );
    vConnectors[ePhModifiers] = &pModifPhs_->GetConnector();

    ADN_CheckBox* pCheckBox = new ADN_CheckBox( tr( "Indirect fire" ), pGroup );
    vConnectors[eIndirect] = &pCheckBox->GetConnector();

    builder.AddStretcher( pGroup, Qt::Vertical );

    // Connect the gui to the data.
    pLaunchers_->SetItemConnectors(vConnectors);

    // Layout
    Q3HBoxLayout* pMainLayout = new Q3HBoxLayout( pMainWidget_, 10, 10 );
    pMainLayout->addWidget( pLaunchers_, 1 );
    pMainLayout->addWidget( pGroup, 4 );
}


// -----------------------------------------------------------------------------
// Name: ADN_Launchers_GUI::ExportPHModifiers
// Created: APE 2005-04-21
// -----------------------------------------------------------------------------
void ADN_Launchers_GUI::ExportPHModifiers( ADN_HtmlBuilder& builder, ADN_Launchers_Data::LauncherInfos& infos )
{
    if( ! infos.bDirect_.GetData() )
        return;

    pLaunchers_->SetCurrentItem( &infos );
    builder.Section( tr( "Phs modificators" ) );
    builder.Stream() << tr( "Shooter's stance / Target's stance" ).ascii();
    builder.CreateTableFrom( *pModifPhs_ );
}
