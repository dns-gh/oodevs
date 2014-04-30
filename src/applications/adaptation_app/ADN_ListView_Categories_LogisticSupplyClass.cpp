// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_Categories_LogisticSupplyClass.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Categories_Data.h"
#include "ADN_LogisticSupplyClasses_Data.h"
#include "ADN_LogisticSupplyClasses_GUI.h"
#include "ADN_Tr.h"
#include "ADN_Units_Data.h"
#include "ADN_Wizard.h"
#include "ADN_WorkspaceElement.h"

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_LogisticSupplyClass::ADN_ListView_Categories_LogisticSupplyClass
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
ADN_ListView_Categories_LogisticSupplyClass::ADN_ListView_Categories_LogisticSupplyClass( QWidget* parent )
    : ADN_ListView( parent, "ADN_ListView_Categories_LogisticSupplyClass", tools::translate( "ADN_ListView_Categories_LogisticSupplyClass", "Classes" ) )
{
    pConnector_.reset( new ADN_Connector_ListView< ADN_LogisticSupplyClasses_Data::LogisticSupplyClass >( *this ) );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_LogisticSupplyClass::~ADN_ListView_Categories_LogisticSupplyClass
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
ADN_ListView_Categories_LogisticSupplyClass::~ADN_ListView_Categories_LogisticSupplyClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_LogisticSupplyClass::ConnectItem
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_ListView_Categories_LogisticSupplyClass::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ADN_LogisticSupplyClasses_Data::LogisticSupplyClass* pInfos = (ADN_LogisticSupplyClasses_Data::LogisticSupplyClass*) pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_LogisticSupplyClasses_GUI::eNbrLogisticSupplyClassGuiElements );

    vItemConnectors_[ ADN_LogisticSupplyClasses_GUI::eLogisticSupplyClassName ]->Connect( &pInfos->strName_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_LogisticSupplyClass::OnContextMenu
// Created: SBO 2006-03-23
// -----------------------------------------------------------------------------
void ADN_ListView_Categories_LogisticSupplyClass::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< ADN_LogisticSupplyClasses_Data::LogisticSupplyClass > wizard( tools::translate( "ADN_ListView_Categories_LogisticSupplyClass", "Classes" ),
                                                       ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_LogisticSupplyClasses_Data >( eLogisticSupplyClasses ).GetLogisticSupplyClasses(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        ADN_LogisticSupplyClasses_Data::LogisticSupplyClass* pCastData = static_cast< ADN_LogisticSupplyClasses_Data::LogisticSupplyClass* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                      ADN_Tr::ConvertFromWorkspaceElement( eUnits ).c_str(),
                                      ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsThatUse( *pCastData ), eUnits );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_LogisticSupplyClass::GetToolTipFor
// Created: ABR 2012-07-25
// -----------------------------------------------------------------------------
std::string ADN_ListView_Categories_LogisticSupplyClass::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    ADN_LogisticSupplyClasses_Data::LogisticSupplyClass* pCastData = static_cast< ADN_LogisticSupplyClasses_Data::LogisticSupplyClass* >( pData );
    assert( pCastData != 0 );

    std::string result;
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eUnits ).c_str(),
                        ADN_Workspace::GetWorkspace().GetUnits().GetData().GetUnitsThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eResources ).c_str(),
                        ADN_Workspace::GetWorkspace().GetResources().GetData().GetResourcesThatUse( *pCastData ), result );

    if( result.empty() )
        result = tools::translate( "ADN_ListView_Categories_LogisticSupplyClass", "<b>Unused</b>" ).toStdString();
    return result;
}
