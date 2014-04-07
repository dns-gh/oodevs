// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_Categories_DotationNature.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Categories_Data.h"
#include "ADN_Natures_Data.h"
#include "ADN_Natures_GUI.h"
#include "ADN_Resources_Data.h"
#include "ADN_Natures_Data.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"
#include "ADN_WorkspaceElement.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_DotationNature::ADN_ListView_Categories_DotationNature
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
ADN_ListView_Categories_DotationNature::ADN_ListView_Categories_DotationNature( QWidget* parent )
    : ADN_ListView( parent, "ADN_ListView_Categories_DotationNature", tools::translate( "ADN_ListView_Categories_DotationNature", "Resource Natures" ) )
{
    pConnector_.reset( new ADN_Connector_ListView< ADN_Natures_Data::NatureInfos >( *this ) );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_DotationNature::~ADN_ListView_Categories_DotationNature
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
ADN_ListView_Categories_DotationNature::~ADN_ListView_Categories_DotationNature()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_DotationNature::ConnectItem
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_ListView_Categories_DotationNature::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ADN_Natures_Data::NatureInfos* pInfos = (ADN_Natures_Data::NatureInfos*) pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Natures_GUI::eNbrDotationNatureGuiElements );

    vItemConnectors_[ ADN_Natures_GUI::eDotationNatureName ]->Connect( &pInfos->strName_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_DotationNature::OnContextMenu
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_ListView_Categories_DotationNature::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< ADN_Natures_Data::NatureInfos > wizard( tools::translate( "ADN_ListView_Categories_DotationNature", "Natures" ), ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Natures_Data >( eNatures ).GetNaturesInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        ADN_Natures_Data::NatureInfos* pCastData = static_cast< ADN_Natures_Data::NatureInfos* >( pCurData_ );
        assert( pCastData != 0 );
        for( uint n = 0; n < sword::DotationType_MAX; ++n )
        {
            auto value = static_cast< sword::DotationType >( n );
            FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                          ENT_Tr::ConvertFromDotationType( value, ENT_Tr::eToSim ).c_str(),
                                          ADN_Workspace::GetWorkspace().GetResources().GetData().GetResourcesThatUse( *pCastData, value ), eResources, value );
        }
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_DotationNature::GetToolTipFor
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
std::string ADN_ListView_Categories_DotationNature::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    ADN_Natures_Data::NatureInfos* pCastData = static_cast< ADN_Natures_Data::NatureInfos* >( pData );
    assert( pCastData != 0 );

    std::string result;
    for( uint n = 0; n < sword::DotationType_MAX; ++n )
    {
        auto value = static_cast< sword::DotationType >( n );
        FillMultiUsersList( ENT_Tr::ConvertFromDotationType( value, ENT_Tr::eToSim ).c_str(),
                            ADN_Workspace::GetWorkspace().GetResources().GetData().GetResourcesThatUse( *pCastData, value ), result );
    }

    if( result.empty() )
        result = tools::translate( "ADN_ListView_Categories_DotationNature", "<b>Unused</b>" ).toStdString();
    return result;
}
