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
#include "ADN_Categories_GUI.h"
#include "ADN_Equipement_Data.h"
#include "ADN_ResourceNatureInfos.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"
#include "ENT/ENT_Tr_Gen.h"

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_DotationNature::ADN_ListView_Categories_DotationNature
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
ADN_ListView_Categories_DotationNature::ADN_ListView_Categories_DotationNature( QWidget* parent )
    : ADN_ListView( parent, "ADN_ListView_Categories_DotationNature", tools::translate( "ADN_ListView_Categories_DotationNature", "Resource Natures" ) )
{
    pConnector_ = new ADN_Connector_ListView< helpers::ResourceNatureInfos >( *this );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_DotationNature::~ADN_ListView_Categories_DotationNature
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
ADN_ListView_Categories_DotationNature::~ADN_ListView_Categories_DotationNature()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_DotationNature::ConnectItem
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_ListView_Categories_DotationNature::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    helpers::ResourceNatureInfos* pInfos = (helpers::ResourceNatureInfos*) pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Categories_GUI::eNbrDotationNatureGuiElements );

    vItemConnectors_[ ADN_Categories_GUI::eDotationNatureName ]->Connect( &pInfos->strName_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_DotationNature::OnContextMenu
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_ListView_Categories_DotationNature::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< helpers::ResourceNatureInfos > wizard( tools::translate( "ADN_ListView_Categories_DotationNature", "Natures" ), ADN_Workspace::GetWorkspace().GetCategories().GetData().GetDotationNaturesInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        helpers::ResourceNatureInfos* pCastData = static_cast< helpers::ResourceNatureInfos* >( pCurData_ );
        assert( pCastData != 0 );
        for( uint n = 0; n < eNbrDotationFamily; ++n )
        {
            E_DotationFamily value = static_cast< E_DotationFamily >( n );
            FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                          ENT_Tr::ConvertFromDotationFamily( value ).c_str(),
                                          ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetEquipmentsThatUse( *pCastData, value ), eEquipement, value );
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
    helpers::ResourceNatureInfos* pCastData = static_cast< helpers::ResourceNatureInfos* >( pData );
    assert( pCastData != 0 );

    std::string result;
    for( uint n = 0; n < eNbrDotationFamily; ++n )
    {
        E_DotationFamily value = static_cast< E_DotationFamily >( n );
        FillMultiUsersList( ENT_Tr::ConvertFromDotationFamily( value ).c_str(),
                            ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetEquipmentsThatUse( *pCastData, value ), result );
    }

    if( result.empty() )
        result = tr( "<b>Unused</b>" ).toAscii().constData();
    return result;
}
