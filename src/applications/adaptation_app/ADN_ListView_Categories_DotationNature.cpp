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
#include "ADN_Wizard_Default.h"

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_DotationNature::ADN_ListView_Categories_DotationNature
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
ADN_ListView_Categories_DotationNature::ADN_ListView_Categories_DotationNature( QWidget * parent, const char * name, Qt::WFlags f )
    : ADN_ListView( parent, name, f )
{
    addColumn( tools::translate( "ADN_ListView_Categories_DotationNature", "Resource Natures" ) );
    setResizeMode( Q3ListView::AllColumns );
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
    ADN_Wizard_Default< helpers::ResourceNatureInfos > wizard( tools::translate( "ADN_ListView_Categories_DotationNature", "Nature" ),
                                                               tools::translate( "ADN_ListView_Categories_DotationNature", "Natures" ),
                                                               ADN_Workspace::GetWorkspace().GetCategories().GetData().GetDotationNaturesInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_DotationNature::GetToolTipFor
// Created: ABR 2012-07-26
// -----------------------------------------------------------------------------
std::string ADN_ListView_Categories_DotationNature::GetToolTipFor( Q3ListViewItem& item )
{
    void* pData = static_cast< ADN_ListViewItem& >( item ).GetData();
    helpers::ResourceNatureInfos* pCastData = static_cast< helpers::ResourceNatureInfos* >( pData );
    assert( pCastData != 0 );
    return FormatUsersList( ADN_Tr::ConvertFromWorkspaceElement( eEquipement ).c_str(),
                            ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetEquipmentsThatUse( *pCastData ) );
}
