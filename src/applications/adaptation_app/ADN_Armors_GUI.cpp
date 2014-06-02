// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Armors_GUI.h"
#include "moc_ADN_Armors_GUI.cpp"
#include "ADN_Armors_Data.h"
#include "ADN_Categories_AttritionEffect_Table.h"
#include "ADN_ComboBoxItem.h"
#include "ADN_GuiBuilder.h"
#include "ADN_ListView_Categories_Armor.h"

// -----------------------------------------------------------------------------
// Name: ADN_Armors_GUI constructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_Armors_GUI::ADN_Armors_GUI( ADN_Armors_Data& data )
    : ADN_GUI_ABC( eCategories )
    , data_( data )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Armors_GUI destructor
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
ADN_Armors_GUI::~ADN_Armors_GUI()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Armors_GUI::Build
// Created: ABR 2013-07-11
// -----------------------------------------------------------------------------
void ADN_Armors_GUI::Build()
{
    // -------------------------------------------------------------------------
    // Creations
    // -------------------------------------------------------------------------
    assert( pMainWidget_ == 0 );
    ADN_GuiBuilder builder( strClassName_ );
    builder.PushSubName( "armors" );
    T_ConnectorVector vInfosConnectors( eNbrArmorGuiElements, static_cast< ADN_Connector_ABC* >( 0 ) );

    Q3VGroupBox* pGroup = new Q3VGroupBox( tr( "Armor classes" ) );
    // Armors list view
    pListView_ = builder.AddWidget< ADN_ListView_Categories_Armor >( "list", pGroup );
    connect( pListView_, SIGNAL( UsersListRequested( const ADN_NavigationInfos::UsedBy& ) ), &ADN_Workspace::GetWorkspace(), SLOT( OnUsersListRequested( const ADN_NavigationInfos::UsedBy& ) ) );
    static_cast< ADN_Connector_Vector_ABC* >( &pListView_->GetConnector() )->Connect( &data_.GetArmorsInfos() );
    connect( pListView_, SIGNAL( ItemSelected( void* ) ), this, SLOT( OnItemSelected( void* ) ) );

    // Armor info
    Q3VGroupBox* pArmorInfoGroup = new Q3VGroupBox( tr( "Armor class" ), pGroup );

    QWidget* pHolder = builder.AddFieldHolder( pArmorInfoGroup );

    // Name field
    builder.AddLocalizedField( data_.GetArmorsInfos(), pHolder, "name", tr( "Name" ), vInfosConnectors[ eArmorName ] );

    pComboType_ = builder.AddEnumField( pHolder, "type", tr( "Type" ), vInfosConnectors[ eArmorType ] );
    connect( pComboType_, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged( int ) ) );

    Q3GroupBox* pArmorNeutralizationGroup = new Q3GroupBox( 3, Qt::Horizontal, tr( "Neutralization" ), pArmorInfoGroup );
    builder.AddField< ADN_TimeField >( pArmorNeutralizationGroup, "average-time", tr( "Average time" ), vInfosConnectors[ eNeutralizationAverage ] );
    builder.AddField< ADN_TimeField >( pArmorNeutralizationGroup, "variance", tr( "Variance" ), vInfosConnectors[ eNeutralizationVariance ] );

    pArmorBreakdownGroup_ = new Q3GroupBox( 3, Qt::Horizontal, tr( "Breakdowns" ), pArmorInfoGroup );
    builder.AddField< ADN_EditLine_Double >( pArmorBreakdownGroup_, "maintenance-support-needed", tr( "Maintenance support needed" ), vInfosConnectors[ eBreakdownEVA ], tr( "%" ), ePercentage );
    builder.AddField< ADN_EditLine_Double >( pArmorBreakdownGroup_, "on-site-fixable", tr( "On site fixable" ), vInfosConnectors[ eBreakdownNEVA ], tr( "%" ), ePercentage );

    pAttritionEffectGroup_ = new Q3VGroupBox( tr( "Attrition effects on humans" ), pGroup );

    new ADN_Categories_AttritionEffect_Table( builder.GetChildName( "attrition-table" ), vInfosConnectors[ eAttritionEffects ], pAttritionEffectGroup_ );

    builder.PopSubName(); //! armors

    // set auto connectors
    pListView_->SetItemConnectors( vInfosConnectors );

    // -------------------------------------------------------------------------
    // Layouts
    // -------------------------------------------------------------------------
    pMainWidget_ = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout( pMainWidget_ );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );
    layout->addWidget( pGroup );
}

// -----------------------------------------------------------------------------
// Name: ADN_Armors_GUI::OnTypeChanged
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
void ADN_Armors_GUI::OnTypeChanged( int index )
{
    if( pComboType_->GetEnumIndexFromGUI( index ) == eProtectionType_Human )
    {
        pArmorBreakdownGroup_->hide();
        pAttritionEffectGroup_->hide();
    }
    else
    {
        static_cast< ADN_ListView_Categories_Armor* >( pListView_ )->CreateDefaultAttritionHumanEffect();
        pArmorBreakdownGroup_->show();
        pAttritionEffectGroup_->show();
    }
}

void ADN_Armors_GUI::OnItemSelected( void* )
{
    for( auto i = 0; i < pComboType_->count(); ++i )
        if( pComboType_->GetEnumIndexFromGUI( i ) == eProtectionType_Crowd )
            pComboType_->removeItem( i );
}
