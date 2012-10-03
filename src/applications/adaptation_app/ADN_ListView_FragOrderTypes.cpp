// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
ADN_ListView_FragOrderTypes::ADN_ListView_FragOrderTypes( ADN_Missions_Data::T_FragOrder_Vector& orders, QWidget* parent /* = 0*/, const char* szName /* = 0*/ )
    : ADN_ListView( parent, szName, tr( "Fragmentary orders" ) )
    , orders_( orders )
{
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
    vItemConnectors_[ADN_Missions_GUI::eMissionSheetDescription]         ->Connect( &pInfos->missionSheetContent_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eParameters]                      ->Connect( &pInfos->parameters_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eFragOrderAvailableWithoutMission]->Connect( &pInfos->isAvailableWithoutMission_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDiaType]                         ->Connect( &pInfos->diaType_, bConnect );
}

namespace
{
    class ADN_FragOrder_WizardPage : public ADN_WizardPage< FragOrder >
    {
    public:
        ADN_FragOrder_WizardPage( const T_ItemVector& existingItems, const QString& pageTitle, QWidget* pParent = 0 )
            : ADN_WizardPage< FragOrder >( existingItems, pageTitle, pParent )
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
             element_->isAvailableWithoutMission_ = true;
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
        ADN_Wizard< FragOrder, ADN_FragOrder_WizardPage > wizard( tr( "Fragmentary orders" ), ADN_Workspace::GetWorkspace().GetMissions().GetData().GetFragOrders(), this );
        FillContextMenuWithDefault( popupMenu, wizard );
        if( pCurData_ != 0 )
        {
            FragOrder* pCastData = static_cast< FragOrder* >( pCurData_ );
            assert( pCastData != 0 );
            FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), tr( "Unit models" ),
                                          ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( eEntityType_Pawn, *pCastData ), eModels, static_cast< int >( eEntityType_Pawn ) );
            FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), tr( "Automata models" ),
                                          ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( eEntityType_Automat, *pCastData ), eModels, static_cast< int >( eEntityType_Automat ) );
            FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), tr( "Crowds models" ),
                                          ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( eEntityType_Population, *pCastData ), eModels, static_cast< int >( eEntityType_Population ) );
        }
        popupMenu.exec( pt );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes::GetToolTipFor
// Created: ABR 2011-09-29
// -----------------------------------------------------------------------------
std::string ADN_ListView_FragOrderTypes::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    FragOrder* pCastData = static_cast< FragOrder* >( pData );
    assert( pCastData != 0 );

    std::string result;
    FillMultiUsersList( tr( "Unit models" ), ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( eEntityType_Pawn, *pCastData ), result );
    FillMultiUsersList( tr( "Automata models" ), ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( eEntityType_Automat, *pCastData ), result );
    FillMultiUsersList( tr( "Crowds models" ), ADN_Workspace::GetWorkspace().GetModels().GetData().GetModelsThatUse( eEntityType_Population, *pCastData ), result );

    if( result.empty() )
        result = tr( "<b>Unused</b>" ).toAscii().constData();
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Missions::ContextMenuDelete
// Created: NPT 2012-08-01
// -----------------------------------------------------------------------------
bool ADN_ListView_FragOrderTypes::ContextMenuDelete()
{
    if( pCurData_ == 0 )
        return false;
    std::string name = static_cast< FragOrder* >( pCurData_ )->strName_.GetData();
    if( ADN_ListView::ContextMenuDelete() )
    {
        emit NotifyFragOrderDeleted( name, eNbrEntityTypes );
        return true;
    }
    return false;
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
