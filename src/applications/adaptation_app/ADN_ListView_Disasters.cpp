// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_ListView_Disasters.h"
#include "moc_ADN_ListView_Disasters.cpp"
#include "ADN_Tr.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Disasters_Data.h"
#include "ADN_Disasters_GUI.h"
#include "ADN_Wizard.h"

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Disasters constructor
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
ADN_ListView_Disasters::ADN_ListView_Disasters( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_ListView_Disasters", ADN_Tr::ConvertFromWorkspaceElement( eDisasters ).c_str() )
{
    pConnector_ = new ADN_Connector_ListView< ADN_Disasters_Data::DisasterInfos >( *this );
    SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Disasters destructor
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
ADN_ListView_Disasters::~ADN_ListView_Disasters()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Disasters::ConnectItem
// Created: LGY 2012-11-13
//-----------------------------------------------------------------------------
void ADN_ListView_Disasters::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ADN_Disasters_Data::DisasterInfos* pInfos = static_cast< ADN_Disasters_Data::DisasterInfos* >( pCurData_ );
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Disasters_GUI::eNbrGuiElements );

    vItemConnectors_[ ADN_Disasters_GUI::eName ]->Connect( &pInfos->strName_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Disasters::OnContextMenu
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
void ADN_ListView_Disasters::OnContextMenu( const QPoint& point )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< ADN_Disasters_Data::DisasterInfos > wizard( ADN_Tr::ConvertFromWorkspaceElement( eDisasters ).c_str(), ADN_Workspace::GetWorkspace().GetDisasters().GetData().GetDisastersInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( point );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Disasters::GetToolTipFor
// Created: LGY 2012-11-13
// -----------------------------------------------------------------------------
std::string ADN_ListView_Disasters::GetToolTipFor( const QModelIndex& )
{
    return "";
}
