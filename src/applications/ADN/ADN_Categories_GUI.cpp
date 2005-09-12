//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Categories_GUI.cpp $
// $Author: Nld $
// $Modtime: 27/04/05 15:56 $
// $Revision: 14 $
// $Workfile: ADN_Categories_GUI.cpp $
//
//*****************************************************************************
#include "ADN_pch.h"
#include "ADN_Categories_GUI.h"

#include "moc_ADN_Categories_GUI.cpp"

#include "ADN_App.h"
#include "ADN_Workspace.h"
#include "ADN_CommonGfx.h"
#include "ADN_Categories_Data.h"
#include "ADN_ListView_Categories_Armor.h"
#include "ADN_ListView_Categories_Size.h"
#include "ADN_GuiBuilder.h"
#include "ADN_Tr.h"

#include <qframe.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qvgroupbox.h>
#include <qhgroupbox.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qpushbutton.h>
#include <qwhatsthis.h>


//-----------------------------------------------------------------------------
// Name: ADN_Categories_GUI constructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_Categories_GUI::ADN_Categories_GUI( ADN_Categories_Data& data )
: ADN_GUI_ABC( "ADN_Categories_GUI" )
, data_      ( data )
, pListArmor_( 0 )
, pListSize_ ( 0 )
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Categories_GUI destructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_Categories_GUI::~ADN_Categories_GUI()
{
}


//-----------------------------------------------------------------------------
// Name: ADN_Categories_GUI::Build
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_Categories_GUI::Build()
{
    assert( pMainWidget_ == 0 );

    ADN_GuiBuilder builder;
    ADN_EditLine_ABC* pEdit = 0;  

    // Create the main widget.
    pMainWidget_ = new QWidget( 0, "categories main widget" );

    // horizontal layout
    QHBoxLayout* pLayout = new QHBoxLayout( pMainWidget_ );
    pLayout->setMargin(20);
    pLayout->setSpacing(20);
    pLayout->setAutoAdd(true);

    /////////////////
    // Armors
    QGroupBox* pArmorGroup = new QVGroupBox( tr( "Armor classes" ), pMainWidget_ );

    QHBox* pArmorListViewGroup = new QHBox( pArmorGroup );      

    // Armors listview
    T_ConnectorVector vArmorInfosConnectors( eNbrArmorGuiElements, (ADN_Connector_ABC*)0 );
    pListArmor_ = new ADN_ListView_Categories_Armor( pArmorListViewGroup );
    static_cast<ADN_Connector_Vector_ABC*>( &pListArmor_->GetConnector() )->Connect( &data_.GetArmorsInfos() );

    // Armor info
    QVGroupBox* pArmorInfoGroup  = new QVGroupBox( tr( "Armor class" ), pArmorGroup );

    QWidget* pHolder = builder.AddFieldHolder( pArmorInfoGroup );
    builder.AddField<ADN_EditLine_String>( pHolder, tr( "Name" ), vArmorInfosConnectors[eArmorName], 0, eVarName );
    pComboType_ = builder.AddEnumField<E_ProtectionType>( pHolder, tr( "Type" ), vArmorInfosConnectors[eArmorType], ADN_Tr::ConvertFromProtectionType );

    connect( pComboType_, SIGNAL( activated( int ) ), this, SLOT( OnTypeChanged() ) );

    QGroupBox* pArmorNeutralizationGroup = new QGroupBox( 3, Qt::Horizontal, tr( "Neutralization" ), pArmorInfoGroup );
    builder.AddField<ADN_EditLine_Double>( pArmorNeutralizationGroup, tr( "Average time" ), vArmorInfosConnectors[eNeutralizationAverage], tr( "s" ), eGreaterEqualZero );
    builder.AddField<ADN_EditLine_Double>( pArmorNeutralizationGroup, tr( "Variance" ), vArmorInfosConnectors[eNeutralizationVariance], tr( "s" ), eGreaterEqualZero );

    pArmorBreakdownGroup_ = new QGroupBox( 3, Qt::Horizontal, tr( "Breakdowns" ), pArmorInfoGroup );
    builder.AddField<ADN_EditLine_Double>( pArmorBreakdownGroup_, tr( "EVA" ), vArmorInfosConnectors[eBreakdownEVA], tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pArmorBreakdownGroup_, tr( "NEVA" ), vArmorInfosConnectors[eBreakdownNEVA], tr( "%" ), ePercentage );

    pWoundedGroup_ = new QGroupBox( 3, Qt::Horizontal, tr( "Wounded humans" ), pArmorInfoGroup );
    builder.AddField<ADN_EditLine_Double>( pWoundedGroup_, tr( "Evac" ), vArmorInfosConnectors[eWoundedEvac], tr( "%" ), ePercentage );
    builder.AddField<ADN_EditLine_Double>( pWoundedGroup_, tr( "No evac" ), vArmorInfosConnectors[eWondedNoEvac], tr( "%" ), ePercentage );

    ///////////////////
    // Sizes
    QGroupBox* pGroup=new QVGroupBox( tr( "Sizes" ),pMainWidget_);
    QHBox     * pGroupSize=new QHBox(pGroup);      
    pGroup->setInsideMargin(20);
    pGroup->setInsideSpacing(20);
    pGroupSize->setSpacing(5);
    
    // sizes list
    T_ConnectorVector    vSizeInfosConnectors(eNbrSizeGuiElements,(ADN_Connector_ABC*)0 );
    pListSize_=new ADN_ListView_Categories_Size(pGroupSize);
    static_cast<ADN_Connector_Vector_ABC*>( &pListSize_->GetConnector() )->Connect( &data_.GetSizesInfos() );
    QWhatsThis::add( pListSize_, "Les différentes catégories de volumes existants dans la simulation.\nCes catégories sont utilisées pour caractériser les composantes.\nElles influencent la perception des unités et les PHs des systèmes d'armes." );

    // sizes infos + modify
    QVGroupBox  *  pGroupOrderSize = new QVGroupBox( tr( "Order" ),pGroupSize);
    
    QFrame      *  pEmptySize      = new QFrame(pGroupOrderSize);
    QVBoxLayout *  pBoxSize        = new QVBoxLayout( pEmptySize );
    pBoxSize->addWidget(new QLabel( tr( "Biggest size" ),pEmptySize));    
    pBoxSize->addStretch();
    
    QHBox* pBtnBox     =new QHBox(pEmptySize);
    pBoxSize->addWidget(pBtnBox);

    QPushButton* pBtnIncSize=new QPushButton("+",pBtnBox);
    pBtnIncSize->setMaximumWidth(30);
    connect(pBtnIncSize,SIGNAL(clicked()),this,SLOT(IncSize()));

    pBoxSize->addSpacing(20);

    pBtnBox     =new QHBox(pEmptySize);
    pBoxSize->addWidget(pBtnBox);

    QPushButton* pBtnDecSize=new QPushButton("-",pBtnBox);
    pBtnDecSize->setMaximumWidth(30);
    connect(pBtnDecSize,SIGNAL(clicked()),this,SLOT(DecSize()));
    
    pBoxSize->addStretch();
    pBoxSize->addWidget(new QLabel( tr( "Smallest size" ),pEmptySize));

    // size
    pGroup  =new QVGroupBox( tr( "Size" ),pGroup);
    pGroup->setInsideMargin(20);
    pGroup->setInsideSpacing(20);
    pEdit=new ADN_EditLine_String(pGroup);
    vSizeInfosConnectors[eSizeName]=&pEdit->GetConnector();

    // set auto connectors
    pListArmor_->SetItemConnectors(vArmorInfosConnectors);
    pListSize_->SetItemConnectors(vSizeInfosConnectors);
}


//-----------------------------------------------------------------------------
// Name: ADN_Categories_GUI::IncSize
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_Categories_GUI::IncSize()
{
    void *curData=pListSize_->GetCurrentData();
    if (curData)
    {
        int pos=pListSize_->FindNdx(curData);
        static_cast<ADN_Connector_Vector_ABC*>(&pListSize_->GetConnector())->SwapItem(pos-1,pos);
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_Categories_GUI::DecSize
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_Categories_GUI::DecSize()
{
    void *curData=pListSize_->GetCurrentData();
    if (curData)
    {
        int pos=pListSize_->FindNdx(curData);
        static_cast<ADN_Connector_Vector_ABC*>(&pListSize_->GetConnector())->SwapItem(pos,pos+1);
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Categories_GUI::OnTypeChanged
// Created: SBO 2005-09-08
// -----------------------------------------------------------------------------
void ADN_Categories_GUI::OnTypeChanged()
{
    const QString& strItem = pComboType_->currentText();
    if( ADN_Tr::ConvertToProtectionType( std::string( strItem ) ) == eProtectionType_Human )
    {
        pArmorBreakdownGroup_->hide();
        pWoundedGroup_->hide();
    }
    else
    {
        pArmorBreakdownGroup_->show();
        pWoundedGroup_->show();
    }
}
