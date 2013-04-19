// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Packages_ListView.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Missions_GUI.h"
#include "ADN_Activities_Data.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"

typedef ADN_Activities_Data::PackageInfos Package;

class ADN_Packages_Connector : public ADN_Connector_ListView< Package >
{
public:
    ADN_Packages_Connector( ADN_Packages_ListView& list )
        : ADN_Connector_ListView( list )
    {}

    virtual ~ADN_Packages_Connector()
    {}

    ADN_ListViewItem* CreateItem( void* obj )
    {
        ADN_ListViewItem *pItem = new ADN_ListViewItem( &list_,obj, 1 );
        Package* pInfos = static_cast< Package*>( obj );
        pItem->Connect( 0, &pInfos->strName_ );
        if( pInfos->strName_ == " - " )
            pItem->setVisible( false );
        return pItem;
    }
};

// -----------------------------------------------------------------------------
// Name: ADN_Packages_ListView constructor
// Created: NPT 2013-04-17
// -----------------------------------------------------------------------------
ADN_Packages_ListView::ADN_Packages_ListView( QWidget* pParent /* = 0 */, const char* szName /* = 0 */, Qt::WFlags f /* = 0 */ )
    : ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tools::translate( "ADN_Packages_ListView", "Name" ) );
    setResizeMode( Q3ListView::AllColumns );
    // Connector creation
    pConnector_ = new ADN_Packages_Connector( *this );
    SetDeletionEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: ADN_Packages_ListView destructor
// Created: NPT 2013-04-17
// -----------------------------------------------------------------------------
ADN_Packages_ListView::~ADN_Packages_ListView()
{
    delete pConnector_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Packages_ListView::ConnectItem
// Created: NPT 2013-04-17
// -----------------------------------------------------------------------------
void ADN_Packages_ListView::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    Package* pInfos = ( Package* ) pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Missions_GUI::eNbrPackageGuiElements );
    vItemConnectors_[ ADN_Missions_GUI::ePackageName ]->Connect( &pInfos->strName_, bConnect );
}

// -----------------------------------------------------------------------------
// Name: ADN_Packages_ListView::OnContextMenu
// Created: NPT 2013-04-17
// -----------------------------------------------------------------------------
void ADN_Packages_ListView::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< Package > wizard( tr( "Packages" ), ADN_Workspace::GetWorkspace().GetMissions().GetData().activitiesData_->GetPackages(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        Package* pCastData = static_cast< Package* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
            tools::translate( "ADN_Packages_ListView", "Unit missions" ), ADN_Workspace::GetWorkspace().GetMissions().GetData().GetUnitMissionsThatUse( *pCastData ), eMissions, 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
            tools::translate( "ADN_Packages_ListView", "Automata missions" ), ADN_Workspace::GetWorkspace().GetMissions().GetData().GetAutomataMissionsThatUse( *pCastData ), eMissions, 1 );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_Packages_ListView::GetToolTipFor
// Created: NPT 2013-04-19
// -----------------------------------------------------------------------------
std::string ADN_Packages_ListView::GetToolTipFor( Q3ListViewItem& item )
{
    void* pData = static_cast< ADN_ListViewItem& >( item ).GetData();
    Package* pCastData = static_cast< Package* >( pData );
    assert( pCastData != 0 );
    std::string retValue;
    FillMultiUsersList( tr( "Unit missions" ), ADN_Workspace::GetWorkspace().GetMissions().GetData().GetUnitMissionsThatUse( *pCastData ), retValue );
    FillMultiUsersList( tr( "Automata missions" ), ADN_Workspace::GetWorkspace().GetMissions().GetData().GetAutomataMissionsThatUse( *pCastData ), retValue );
    if( retValue.empty() )
        retValue = tr( "<b>Unused</b>" ).toAscii().constData();
    return retValue;
}
