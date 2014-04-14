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
#include "ADN_WorkspaceElement.h"

typedef ADN_Missions_FragOrder FragOrder;

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes constructor
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
ADN_ListView_FragOrderTypes::ADN_ListView_FragOrderTypes( QWidget* parent, ADN_Missions_Data::T_Mission_Vector& orders )
    : ADN_ListView( parent, "ADN_ListView_FragOrderTypes", tr( "Fragmentary orders" ) )
    , orders_( orders )
{
    pConnector_.reset( new ADN_Connector_ListView< FragOrder >( *this ) );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_FragOrderTypes destructor
// Created: SBO 2006-12-06
// -----------------------------------------------------------------------------
ADN_ListView_FragOrderTypes::~ADN_ListView_FragOrderTypes()
{
    // NOTHING
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
    vItemConnectors_[ADN_Missions_GUI::eDescriptionSheetPath]            ->Connect( &pInfos->missionSheetPath_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDescriptionContext]->Connect( &pInfos->descriptionContext_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDescriptionParameters]->Connect( &pInfos->parameters_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDescriptionBehavior]->Connect( &pInfos->descriptionBehavior_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDescriptionSpecificCases]->Connect( &pInfos->descriptionSpecific_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDescriptionComments]->Connect( &pInfos->descriptionComment_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDescriptionMissionEnd]->Connect( &pInfos->descriptionMissionEnd_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDescriptionAttachments]->Connect( &pInfos->attachments_, bConnect );

    vItemConnectors_[ADN_Missions_GUI::eParameters]                      ->Connect( &pInfos->parameters_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eFragOrderAvailableWithoutMission]->Connect( &pInfos->isAvailableWithoutMission_, bConnect );
    vItemConnectors_[ADN_Missions_GUI::eDiaType]                         ->Connect( &pInfos->diaType_, bConnect );
}

namespace
{
    class ADN_FragOrder_WizardPage : public ADN_WizardPage< ADN_Missions_ABC >
    {
    public:
        ADN_FragOrder_WizardPage( const T_ItemVector& existingItems, const QString& pageTitle, QWidget* pParent = 0 )
            : ADN_WizardPage< ADN_Missions_ABC >( existingItems, pageTitle, pParent )
            , addForAllUnits_( 0 )
            , addForAllAutomata_( 0 )
            , addForAllPops_( 0 )
        {
            // NOTHING
        }

    protected:
        virtual QWidget* CreateOptionalField()
        {
            QGroupBox* result = new gui::RichGroupBox( "checkboxs-group" );
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
                auto& units = ADN_Workspace::GetWorkspace().GetModels().GetData().GetModels( eEntityType_Pawn );
                for( auto it1 = units.begin(); it1 != units.end(); ++it1 )
                    (*it1)->AddFragOrder( static_cast< FragOrder* >( element_ ), name );
            }
            if( addForAllAutomata_->isChecked() )
            {
                auto& automata = ADN_Workspace::GetWorkspace().GetModels().GetData().GetModels( eEntityType_Automat );
                for( auto it1 = automata.begin(); it1 != automata.end(); ++it1 )
                    (*it1)->AddFragOrder( static_cast< FragOrder* >( element_ ), name );
            }
            if( addForAllPops_->isChecked() )
            {
                auto& pops = ADN_Workspace::GetWorkspace().GetModels().GetData().GetModels( eEntityType_Population );
                for( auto it1 = pops.begin(); it1 != pops.end(); ++it1 )
                    (*it1)->AddFragOrder( static_cast< FragOrder* >( element_ ), name );
            }
            static_cast< FragOrder* >( element_ )->isAvailableWithoutMission_ = true;
        }
public:
        virtual ADN_Missions_ABC* NewT()
        {
            return new ADN_Missions_FragOrder();
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
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< ADN_Missions_ABC, ADN_FragOrder_WizardPage > wizard( tr( "Fragmentary orders" ), ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissions( eMissionType_FragOrder ), this );
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
        result = tr( "<b>Unused</b>" ).toStdString();
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
    boost::shared_ptr< kernel::LocalizedString > name = static_cast< ADN_Missions_ABC* >( pCurData_ )->strName_.GetTranslation();
    if( ADN_ListView::ContextMenuDelete() )
    {
        emit NotifyElementDeleted( name, eMissionType_FragOrder );
        return true;
    }
    return false;
}
