//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Categories_Size.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:45 $
// $Revision: 6 $
// $Workfile: ADN_ListView_Categories_Size.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ListView_UrbanAccommodation_Type.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Inhabitants_Data.h"
#include "ADN_Urban_Data.h"
#include "ADN_Urban_GUI.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"
#include "ADN_WorkspaceElement.h"

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanAccommodation_Type constructor
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
ADN_ListView_UrbanAccommodation_Type::ADN_ListView_UrbanAccommodation_Type( QWidget* parent )
    : ADN_ListView( parent, "Activity", tools::translate( "ADN_ListView_UrbanAccommodation_Type", "Activity" ) )
{
    pConnector_.reset( new ADN_Connector_ListView< ADN_Urban_Data::AccommodationInfos >( *this ) );
    SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanAccommodation_Type destructor
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
ADN_ListView_UrbanAccommodation_Type::~ADN_ListView_UrbanAccommodation_Type()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanAccommodation_Type::ConnectItem
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
void ADN_ListView_UrbanAccommodation_Type::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;
    ADN_Urban_Data::AccommodationInfos* pInfos = static_cast< ADN_Urban_Data::AccommodationInfos* >( pCurData_ );
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Urban_GUI::eNbrUrbanAccommodationGuiElements );

    vItemConnectors_[ ADN_Urban_GUI::eUrbanAccommodationName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanAccommodationNominalCapacity ]->Connect( &pInfos->nominalCapacity_, bConnect );
    vItemConnectors_[ ADN_Urban_GUI::eUrbanAccommodationMaxCapacity ]->Connect( &pInfos->maxCapacity_, bConnect );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanAccommodation_Type::OnContextMenu
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
void  ADN_ListView_UrbanAccommodation_Type::OnContextMenu( const QPoint& pt)
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< ADN_Urban_Data::AccommodationInfos > wizard( tools::translate( "ADN_ListView_UrbanAccommodation_Type", "Activities" ), ADN_Workspace::GetWorkspace().GetUrban().GetData().GetAccommodationsInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanAccommodation_Type::GetToolTipFor
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
std::string ADN_ListView_UrbanAccommodation_Type::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    ADN_Urban_Data::AccommodationInfos* pCastData = static_cast< ADN_Urban_Data::AccommodationInfos* >( pData );
    assert( pCastData != 0 );
    std::string result;
    FillMultiUsersList( tools::translate( "ADN_ListView_UrbanAccommodation_Type", "Templates" ),
                            ADN_Workspace::GetWorkspace().GetUrban().GetData().GetUrbanTemplateThatUse( *pCastData ), result );
    FillMultiUsersList( tools::translate( "ADN_ListView_UrbanAccommodation_Type", "Inhabitants" ),
                            ADN_Workspace::GetWorkspace().GetInhabitants().GetData().GetSchedulesThatUse( *pCastData ), result );
    if( result.empty() )
        result = tools::translate( "ADN_ListView_UrbanAccommodation_Type", "<b>Unused</b>" ).toStdString();
    return result;
}
