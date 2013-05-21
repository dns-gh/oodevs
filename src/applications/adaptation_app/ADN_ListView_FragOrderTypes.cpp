// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_FragOrderTypes.h"
#include "moc_ADN_ListView_FragOrderTypes.cpp"

#include "ADN_Missions_Data.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Tools.h"
#include "ADN_Missions_GUI.h"
#include "ADN_Models_Data.h"
#include "ADN_Wizard.h"
#include "ADN_WizardPage.h"

typedef ADN_Missions_Data::FragOrder FragOrder;

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes constructor
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
ADN_ListView_FragOrderTypes::ADN_ListView_FragOrderTypes( ADN_Missions_Data::T_Mission_ABC_Vector& /* orders */, QWidget* parent /* = 0*/, const char* szName /* = 0*/ )
    : ADN_ListView( parent, szName )
{
    addColumn( tr( "Fragmentary orders" ) );
    setResizeMode( Q3ListView::AllColumns );
    pConnector_ = new ADN_Connector_ListView< FragOrder >( *this );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes destructor
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
ADN_ListView_FragOrderTypes::~ADN_ListView_FragOrderTypes()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes::ConnectItem
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
void ADN_ListView_FragOrderTypes::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    FragOrder* pInfos = (FragOrder*)pCurData_;
    vItemConnectors_[ADN_Missions_GUI::eName]                            ->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDoctrineDescription]             ->Connect( &pInfos->doctrineDescription_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eUsageDescription]                ->Connect( &pInfos->usageDescription_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eParameters]                      ->Connect( &pInfos->parameters_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eFragOrderAvailableWithoutMission]->Connect( &pInfos->isAvailableWithoutMission_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDiaType]                         ->Connect( &pInfos->diaType_, bConnect );
}

namespace
{
    class ADN_FragOrder_WizardPage : public ADN_WizardPage< ADN_Missions_Data::ADN_Missions_ABC >
    {
    public:
        ADN_FragOrder_WizardPage( const T_ItemVector& existingItems, const QString& pageTitle, QWidget* pParent = 0 )
            : ADN_WizardPage< ADN_Missions_Data::ADN_Missions_ABC >( existingItems, pageTitle, pParent )
            , addForAllUnits_( 0 )
            , addForAllAutomata_( 0 )
            , addForAllPops_( 0 )
        {
            // NOTHING
        }

    protected:
        virtual QWidget* CreateOptionalField()
        {
            QGroupBox* result = new QGroupBox();
            QVBoxLayout* layout = new QVBoxLayout( result );
            addForAllUnits_ = new QCheckBox( qApp->translate( "ADN_FragOrder_WizardSecondPage", "Add for all Units" ) );
            addForAllAutomata_ = new QCheckBox( qApp->translate( "ADN_FragOrder_WizardSecondPage", "Add for all Automata" ) );
            addForAllPops_ = new QCheckBox( qApp->translate( "ADN_FragOrder_WizardSecondPage", "Add for all Crowds" ) );
            layout->addWidget( addForAllUnits_ );
            layout->addWidget( addForAllAutomata_ );
            layout->addWidget( addForAllPops_ );
            return result;
        }

        virtual void ApplyOptions()
        {
            if( !element_ )
                return;
            std::string name = GetName();
            if( addForAllUnits_->isChecked() )
            {
                ADN_Models_Data::T_ModelInfos_Vector& units = ADN_Workspace::GetWorkspace().GetModels().GetData().GetUnitModelsInfos();
                for( ADN_Models_Data::IT_ModelInfos_Vector it1 = units.begin(); it1 != units.end(); ++it1 )
                    (*it1)->AddFragOrder( element_, name );
            }
            if( addForAllAutomata_->isChecked() )
            {
                ADN_Models_Data::T_ModelInfos_Vector& automata = ADN_Workspace::GetWorkspace().GetModels().GetData().GetAutomataModelsInfos();
                for( ADN_Models_Data::IT_ModelInfos_Vector it1 = automata.begin(); it1 != automata.end(); ++it1 )
                    (*it1)->AddFragOrder( element_, name );
            }
            if( addForAllPops_->isChecked() )
            {
                ADN_Models_Data::T_ModelInfos_Vector& pops = ADN_Workspace::GetWorkspace().GetModels().GetData().GetPopulationModelsInfos();
                for( ADN_Models_Data::IT_ModelInfos_Vector it1 = pops.begin(); it1 != pops.end(); ++it1 )
                    (*it1)->AddFragOrder( element_, name );
            }
            static_cast< FragOrder* >( element_ )->isAvailableWithoutMission_ = true;
        }

    public:
        virtual ADN_Missions_Data::ADN_Missions_ABC* NewT()
        {
            return new ADN_Missions_Data::FragOrder();
        }

    private:
        QCheckBox* addForAllUnits_;
        QCheckBox* addForAllAutomata_;
        QCheckBox* addForAllPops_;
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes::OnContextMenu
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
void ADN_ListView_FragOrderTypes::OnContextMenu( const QPoint& pt )
{
    if( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin )
    {
        Q3PopupMenu popupMenu( this );
        ADN_Wizard< ADN_Missions_Data::ADN_Missions_ABC, ADN_FragOrder_WizardPage > wizard( tr( "Fragmentary orders" ), ADN_Workspace::GetWorkspace().GetMissions().GetData().GetFragOrders(), this );
        FillContextMenuWithDefault( popupMenu, wizard );
        if( pCurData_ != 0 )
        {
            FragOrder* pCastData = static_cast< FragOrder* >( pCurData_ );
            assert( pCastData != 0 );
            FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), tr( "Unit models" ),
                                          ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( ADN_Models_Data::ModelInfos::ePawn, *pCastData ), eModels, static_cast< int >( ADN_Models_Data::ModelInfos::ePawn ) );
            FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), tr( "Automata models" ),
                                          ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( ADN_Models_Data::ModelInfos::eAutomat, *pCastData ), eModels, static_cast< int >( ADN_Models_Data::ModelInfos::eAutomat ) );
            FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), tr( "Crowds models" ),
                                          ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( ADN_Models_Data::ModelInfos::ePopulation, *pCastData ), eModels, static_cast< int >( ADN_Models_Data::ModelInfos::ePopulation ) );
        }
        popupMenu.exec( pt );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes::GetToolTipFor
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
std::string ADN_ListView_FragOrderTypes::GetToolTipFor( Q3ListViewItem& item )
{
    void* pData = static_cast< ADN_ListViewItem& >( item ).GetData();
    FragOrder* pCastData = static_cast< FragOrder* >( pData );
    assert( pCastData != 0 );

    std::string result;
    FillMultiUsersList( tr( "Unit models" ), ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( ADN_Models_Data::ModelInfos::ePawn, *pCastData ), result );
    FillMultiUsersList( tr( "Automata models" ), ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( ADN_Models_Data::ModelInfos::eAutomat, *pCastData ), result );
    FillMultiUsersList( tr( "Crowds models" ), ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( ADN_Models_Data::ModelInfos::ePopulation, *pCastData ), result );

    if( result.empty() )
        result = tr( "<b>Unused</b>" ).toAscii().constData();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes::OnToogled
// Created: HBD 2010-09-06
// -----------------------------------------------------------------------------
void ADN_ListView_FragOrderTypes::OnToogled( bool isChecked )
{
    if( pCurData_ == 0 || isChecked )
        return;
    FragOrder* pInfos = (FragOrder*)pCurData_;
    const std::string& name = pInfos->strName_.GetData();
    ADN_Models_Data::T_ModelInfos_Vector& units = ADN_Workspace::GetWorkspace().GetModels().GetData().GetUnitModelsInfos();
    ADN_Models_Data::T_ModelInfos_Vector& automata = ADN_Workspace::GetWorkspace().GetModels().GetData().GetAutomataModelsInfos();
    ADN_Models_Data::T_ModelInfos_Vector& pops = ADN_Workspace::GetWorkspace().GetModels().GetData().GetPopulationModelsInfos();
    for( ADN_Models_Data::IT_ModelInfos_Vector it1 = units.begin(); it1 != units.end(); ++it1 )
        (*it1)->RemoveFragOder( name );
    for( ADN_Models_Data::IT_ModelInfos_Vector it1 = automata.begin(); it1 != automata.end(); ++it1 )
        (*it1)->RemoveFragOder( name );
    for( ADN_Models_Data::IT_ModelInfos_Vector it1 = pops.begin(); it1 != pops.end(); ++it1 )
        (*it1)->RemoveFragOder( name );
}
