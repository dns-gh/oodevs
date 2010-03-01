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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Equipement_GUI.cpp $
// $Author: Nld $
// $Modtime: 20/07/05 14:29 $
// $Revision: 25 $
// $Workfile: ADN_Equipement_GUI.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Equipement_GUI.h"
#include "moc_ADN_Equipement_GUI.cpp"

#include <qlabel.h>
#include <qwidget.h>
#include <qtabwidget.h>
#include <qlayout.h>
#include <qgroupbox.h>
#include <qdialog.h>

#include "ADN_MainWindow.h"
#include "ADN_TableItem_Edit.h"
#include "ADN_Tools.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Enums.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Equipement_GenericListView.h"
#include "ADN_Equipement_AttritionTable.h"
#include "ADN_Connector_ListView_ABC.h"
#include "ADN_EditLine.h"
#include "ADN_ComboBox_Enum.h"
#include "ADN_Equipement_AmmoListView.h"
#include "ADN_Equipement_Postures_GUI.h"
#include "ADN_GroupBox.h"
#include "ENT/ENT_Tr.h"
#include "ADN_Tr.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_TimeField.h"
#include "ADN_ComboBox_Equipment_Nature.h"

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI constructor
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
ADN_Equipement_GUI::ADN_Equipement_GUI( ADN_Equipement_Data& data )
: ADN_GUI_ABC            ( "ADN_Equipement_GUI" )
, data_                  ( data )
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI destructor
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
ADN_Equipement_GUI::~ADN_Equipement_GUI()
{
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::Build
// Created: APE 2004-12-13
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::Build()
{
    if( pMainWidget_ != 0 )
        return;

    pMainWidget_ = new QWidget( 0 );
    QTabWidget* pTabWidget = new QTabWidget( pMainWidget_ );

    this->BuildAmmunition( pTabWidget );
    this->BuildGeneric( eDotationFamily_Carburant, pTabWidget );
    this->BuildGeneric( eDotationFamily_Mine, pTabWidget );
    this->BuildGeneric( eDotationFamily_Explosif, pTabWidget );
    this->BuildGeneric( eDotationFamily_Barbele, pTabWidget );
    this->BuildGeneric( eDotationFamily_Ration, pTabWidget );
    this->BuildGeneric( eDotationFamily_Piece, pTabWidget );

    QGridLayout* pMainLayout = new QGridLayout( pMainWidget_, 1, 1, 10, 10 );
    pMainLayout->addWidget( pTabWidget, 0, 0 );
}

#include "ADN_Validator.h"

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::BuildGeneric
// Created: APE 2004-12-28
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::BuildGeneric( E_DotationFamily nType, QTabWidget* pParent )
{
    ADN_GuiBuilder builder;
    QWidget* pPage = new QWidget( pParent );
    pParent->addTab( pPage, ENT_Tr::ConvertFromDotationFamily( nType, ENT_Tr_ABC::eToTr ).c_str() );

    ADN_Equipement_GenericListView* pListView = new ADN_Equipement_GenericListView( nType, pPage );
    pListView->GetConnector().Connect( & data_.GetDotation( nType ).categories_ );
    T_ConnectorVector vConnectors( eNbrGenericGuiElements, (ADN_Connector_ABC*)0 );

    QGroupBox* pGroupBox = new QGroupBox( 1, Qt::Horizontal, ENT_Tr::ConvertFromDotationFamily( nType, ENT_Tr_ABC::eToTr ).c_str(), pPage );

    QWidget* pHolder = builder.AddFieldHolder( pGroupBox );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "Name" ), vConnectors[eName] );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "CodeEMAT6" ), vConnectors[eGenEMAT6Code] );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "CodeEMAT8" ), vConnectors[eGenEMAT8Code] );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "CodeLFRIL" ), vConnectors[eGenLFRILCode] );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "CodeNNO" ), vConnectors[eGenNNOCode] );
    builder.AddField<ADN_ComboBox_Equipment_Nature>( pHolder, tr( "Nature" ), vConnectors[eGenNature] );

    QGroupBox* pPackagingGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Packaging" ), pGroupBox ); 
    builder.AddField<ADN_EditLine_Double>( pPackagingGroup, tr( "Nbr per package" ), vConnectors[ePackageNbr], 0, eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pPackagingGroup, tr( "Package weight" ), vConnectors[ePackageWeight], tr( "T" ), eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pPackagingGroup, tr( "Package volume" ), vConnectors[ePackageVolume], tr( "m3" ), eGreaterZero );

    pListView->SetItemConnectors(vConnectors);

    // Layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pPage, 10, 10 );
    pMainLayout->addWidget( pListView, 1 );
    pMainLayout->addWidget( pGroupBox, 4 );
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::BuildAmmunition
// Created: APE 2004-12-28
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::BuildAmmunition( QTabWidget* pParent )
{
    ADN_GuiBuilder builder;

    QWidget* pPage = new QWidget( pParent );
    pParent->addTab( pPage, ENT_Tr::ConvertFromDotationFamily( eDotationFamily_Munition, ENT_Tr_ABC::eToTr ).c_str() );

    pAmmoListView_ = new ADN_Equipement_AmmoListView( pPage );
    pAmmoListView_->GetConnector().Connect( & data_.GetDotation( eDotationFamily_Munition ).categories_ );
    T_ConnectorVector vConnectors( eNbrAmmoGuiElements, (ADN_Connector_ABC*)0 );

    QGroupBox* pGroupBox = new QGroupBox( 1, Qt::Horizontal, ENT_Tr::ConvertFromDotationFamily( eDotationFamily_Munition, ENT_Tr_ABC::eToTr ).c_str(), pPage );

    QWidget* pHolder = builder.AddFieldHolder( pGroupBox );

    builder.AddField<ADN_EditLine_String>( pHolder, tr( "Name" ), vConnectors[eAmmoName] );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "CodeEMAT6" ), vConnectors[eEMAT6Code] );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "CodeEMAT8" ), vConnectors[eEMAT8Code] );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "CodeLFRIL" ), vConnectors[eLFRILCode] );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "CodeNNO" ), vConnectors[eNNOCode] );
    builder.AddEnumField<E_MunitionType>( pHolder, tr( "Type" ), vConnectors[eType], ADN_Tr::ConvertFromMunitionType );
    builder.AddField<ADN_ComboBox_Equipment_Nature>( pHolder, tr( "Nature" ), vConnectors[eNature] );

    QGroupBox* pPackagingGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Packaging" ), pGroupBox ); 
    builder.AddField<ADN_EditLine_Double>( pPackagingGroup, tr( "Nbr per package" ), vConnectors[eAmmoPackageNbr], 0, eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pPackagingGroup, tr( "Package weight" ), vConnectors[eAmmoPackageWeight], tr( "T" ), eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pPackagingGroup, tr( "Package volume" ), vConnectors[eAmmoPackageVolume], tr( "m3" ), eGreaterZero );

    builder.AddField<ADN_CheckBox>( pHolder, tr( "For indirect fire" ), vConnectors[eTrancheD] );


    // Direct fire properties
    ADN_GroupBox* pDirectGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Attritions" ), pGroupBox );
    vConnectors[eDirect] = &pDirectGroup->GetConnector();

    pAttritionTable_ = new ADN_Equipement_AttritionTable( pDirectGroup );
    vConnectors[eAttritions] = &pAttritionTable_->GetConnector();

    // Indirect fire properties
    ADN_GroupBox* pIndirectGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Indirect fire" ), pGroupBox );
    vConnectors[eIndirect] = &pIndirectGroup->GetConnector();

    pHolder = builder.AddFieldHolder( pIndirectGroup );

    ADN_ComboBox_Enum<E_TypeMunitionTirIndirect>* pIndirectTypeCombo_ = builder.AddEnumField<E_TypeMunitionTirIndirect>( pHolder, tr( "Type" ), vConnectors[eIndirectType], ADN_Tr::ConvertFromTypeMunitionTirIndirect );
    pIndirectGroup->connect( pIndirectTypeCombo_, SIGNAL(activated(int)), this, SLOT( IndirectTypeComboActivated(int) ) );

    builder.AddField<ADN_EditLine_Int>( pHolder, tr( "Intervention" ), vConnectors[eIntervention], 0, eGreaterEqualZero );
    builder.AddField<ADN_EditLine_Double>( pHolder, tr( "X Dispersion" ), vConnectors[eDispersionX], tr( "m" ), eGreaterZero );
    builder.AddField<ADN_EditLine_Double>( pHolder, tr( "Y Dispersion" ), vConnectors[eDispersionY], tr( "m" ), eGreaterZero );

    // Explosive parameters
    pExplosiveParametersGroup_ = new QGroupBox( 1, Qt::Horizontal, tr( "Explosive ammo parameters" ), pIndirectGroup );
    pExplosiveParametersGroup_->hide();

    ADN_Equipement_Postures_GUI* pStance = new ADN_Equipement_Postures_GUI( tr( "Stance" ), pExplosiveParametersGroup_ );
    vConnectors[eModifStances] = &pStance->GetConnector();

    pHolder = builder.AddFieldHolder( pExplosiveParametersGroup_ );
    builder.AddField<ADN_EditLine_Double>( pHolder, tr( "Neutralization ratio" ), vConnectors[eNeutralizationRatio] );
    builder.SetValidator( new ADN_DoubleValidator( 1, INT_MAX, 2, this ) );

    // Flare parameters
    pFlareParametersGroup_ = new QGroupBox( 3, Qt::Horizontal, tr( "Flare/Smoke ammo parameters" ), pIndirectGroup );
    pFlareParametersGroup_->hide();

    builder.AddField<ADN_TimeField>( pFlareParametersGroup_, tr( "Activation duration" ), vConnectors[eDeployTime] );
    builder.AddField<ADN_TimeField>( pFlareParametersGroup_, tr( "Span" ), vConnectors[eLifetime] );

    // Mine parameters
    pMineParametersGroup_ = new QGroupBox( 3, Qt::Horizontal, tr( "Mine ammo parameters" ), pIndirectGroup );
    pMineParametersGroup_->hide();

    builder.AddField<ADN_EditLine_Int>( pMineParametersGroup_, tr( "Mines quantity" ), vConnectors[eMineNumber] );

    // Illumination 
    ADN_GroupBox* pIlluminationGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Illumination capacity" ), pGroupBox );
    vConnectors[eIlluminating] = &pIlluminationGroup->GetConnector();
    builder.AddField<ADN_EditLine_Double>( pIlluminationGroup, tr( "Range" ), vConnectors[eRange] );
    builder.AddField<ADN_CheckBox>( pIlluminationGroup, tr( "Must Maintain illumination" ), vConnectors[eMaintainIllumination] );

    ADN_GroupBox* pGuidanceGroup = new ADN_GroupBox( 1, Qt::Horizontal, tr( "Guidance" ), pGroupBox );
    vConnectors[eGuided] = &pGuidanceGroup->GetConnector();
    builder.AddField<ADN_CheckBox>( pGuidanceGroup, tr( "Must Maintain guidance" ), vConnectors[eMaintainGuidance] );
    builder.AddField<ADN_EditLine_Double>( pGuidanceGroup, tr( "Illumination range needed" ), vConnectors[eGuidanceRange] );

    //Connect
    pAmmoListView_->SetItemConnectors(vConnectors);

    // Layout
    QHBoxLayout* pMainLayout = new QHBoxLayout( pPage, 10, 10 );
    pMainLayout->addWidget( pAmmoListView_, 1 );
    pMainLayout->addWidget( pGroupBox, 4 );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::IndirectTypeComboActivated
// Created: APE 2005-01-04
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::IndirectTypeComboActivated( int nIndex )
{
    pExplosiveParametersGroup_->hide();
    pFlareParametersGroup_->hide();
    pMineParametersGroup_->hide();

    if( (E_TypeMunitionTirIndirect) nIndex == eTypeMunitionTirIndirect_Explosif 
        || (E_TypeMunitionTirIndirect) nIndex == eTypeMunitionTirIndirect_Aced 
        || (E_TypeMunitionTirIndirect) nIndex == eTypeMunitionTirIndirect_Grenade )
        pExplosiveParametersGroup_->show();
    else if(  (E_TypeMunitionTirIndirect) nIndex == eTypeMunitionTirIndirect_Eclairant
            ||(E_TypeMunitionTirIndirect) nIndex == eTypeMunitionTirIndirect_Fumigene )
        pFlareParametersGroup_->show();
    else if( (E_TypeMunitionTirIndirect) nIndex == eTypeMunitionTirIndirect_Mine )
        pMineParametersGroup_->show();
}



// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::CreatePKTable
// Created: APE 2005-03-30
// -----------------------------------------------------------------------------
ADN_Table* ADN_Equipement_GUI::CreatePKTable()
{
    ADN_Categories_Data::T_ArmorInfos_Vector& armorInfos = ADN_Workspace::GetWorkspace().GetCategories().GetData().GetArmorsInfos();
    
    ADN_GuiBuilder builder;
    ADN_Table* pTable = builder.CreateTable( 0 );

    // Setup the header.
    pTable->setNumCols( 5 );
    pTable->horizontalHeader()->setLabel( 0, tr( "Ammunition" ) );
    pTable->horizontalHeader()->setLabel( 1, tr( "Target armor" ) );
    pTable->horizontalHeader()->setLabel( 2, tr( "On site fixable" ) );
    pTable->horizontalHeader()->setLabel( 3, tr( "Maintenance support needed" ) );
    pTable->horizontalHeader()->setLabel( 4, tr( "Destroyed" ) );
    pTable->horizontalHeader()->show();

    pTable->setNumRows( 1 );
    builder.AddTableCell( pTable, 0, 0, tr( "Ammunition" ) );
    builder.AddTableCell( pTable, 0, 1, tr( "Target armor" ) );
    builder.AddTableCell( pTable, 0, 2, tr( "On site fixable" ) );
    builder.AddTableCell( pTable, 0, 3, tr( "Maintenance support needed" ) );
    builder.AddTableCell( pTable, 0, 4, tr( "Destroyed" ) );
    pTable->hideRow( 0 );
    pTable->AddBoldGridRow( 0 );
    pTable->setSorting( false );

    // Fill the table.
    ADN_Equipement_Data::DotationInfos& ammo = data_.GetDotation( eDotationFamily_Munition );

    int nRowSize = (int)armorInfos.size();
    int nRow = 1;
    for( ADN_Equipement_Data::IT_CategoryInfos_Vector it = ammo.categories_.begin(); it != ammo.categories_.end(); ++it )
    {
        ADN_Equipement_Data::AmmoCategoryInfo& ammoCategory = (ADN_Equipement_Data::AmmoCategoryInfo&)(**it);
        if( ! ammoCategory.bDirect_.GetData() )
            continue;

        pTable->setNumRows( nRow + nRowSize );
        pTable->AddBoldGridRow( nRow );
        builder.AddTableCell<ADN_TableItem_String>( pTable, *it, nRow, 0, armorInfos.size(), 1, ammoCategory.strName_, eNone, QTableItem::Never );
        int nSubRow = 0;
        for( ADN_Equipement_Data::IT_AttritionInfos_Vector it2 = ammoCategory.attritions_.begin(); it2 != ammoCategory.attritions_.end(); ++it2, ++nSubRow )
        {
            ADN_TableItem_Double* pCell = 0;
            builder.AddTableCell<ADN_TableItem_String>( pTable, *it, nRow + nSubRow, 1, (*it2)->ptrArmor_.GetData()->strName_, eNone, QTableItem::Never );

            pCell = builder.AddTableCell<ADN_TableItem_Double>( pTable, *it, nRow + nSubRow, 2, (*it2)->rRepairNoEvac_, ePercentage );
            pCell->UseColor( true );

            pCell = builder.AddTableCell<ADN_TableItem_Double>( pTable, *it, nRow + nSubRow, 3, (*it2)->rRepairWithEvac_, ePercentage );
            pCell->UseColor( true );

            pCell = builder.AddTableCell<ADN_TableItem_Double>( pTable, *it, nRow + nSubRow, 4, (*it2)->rDestroy_, ePercentage );
            pCell->UseColor( true );
        }
        nRow += nRowSize;
    }

    pTable->AdjustColumns( 100 );
    return pTable;
}


// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::RegisterTable
// Created: APE 2005-03-30
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::RegisterTable( ADN_MainWindow& mainWindow )
{
    mainWindow.AddTable( tr( "PKs" ), new ADN_Callback<ADN_Table*,ADN_Equipement_GUI>( this, &ADN_Equipement_GUI::CreatePKTable ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Equipement_GUI::ExportPKs
// Created: APE 2005-04-21
// -----------------------------------------------------------------------------
void ADN_Equipement_GUI::ExportPKs( ADN_HtmlBuilder& builder, ADN_Equipement_Data::AmmoCategoryInfo& infos )
{
    if( ! infos.bDirect_.GetData() )
        return;

    pAmmoListView_->SetCurrentItem( &infos );
    builder.Section( tr( "PKs" ) );
    builder.CreateTableFrom( *pAttritionTable_ );
}
