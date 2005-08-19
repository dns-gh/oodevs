// *****************************************************************************
//
// $Created: NLD 2003-12-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_MaintenancePriorities.cpp $
// $Author: Nld $
// $Modtime: 3/02/05 14:17 $
// $Revision: 2 $
// $Workfile: MOS_MTH_MaintenancePriorities.cpp $
//
// *****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_MTH_MaintenancePriorities.h"
#include "moc_MOS_MTH_MaintenancePriorities.cpp"

#include "MOS_App.h"

#include <qlistview.h>
#include <qpopupmenu.h>

// -----------------------------------------------------------------------------
// Name: MOS_MTH_MaintenancePriorities constructor
// Created: NLD 2003-12-03
// -----------------------------------------------------------------------------
MOS_MTH_MaintenancePriorities::MOS_MTH_MaintenancePriorities( ASN1T_MaintenancePriorites& asnMaintenancePriorities, QWidget* pParent, const std::string& strName )
    : QWidget                   ( pParent )
    , MOS_MissionTool_ABC       ()
    , pAsnMaintenancePriorities_( &asnMaintenancePriorities )
{
    QVBoxLayout* pLocalLayout = new QVBoxLayout( this );

    // Title
    QLabel* pLabel = CreateTitle( this, strName );
    pLocalLayout->addWidget( pLabel );

    pTypeListView_ = new QListView( this, "" );
    pTypeListView_->addColumn( "Types" );
    pTypeListView_->setSorting( -1 );
    pLocalLayout->addWidget( pTypeListView_ );
    

    pTypesMenu_ = new QPopupMenu( this );
    
    const MOS_App::T_MosId_String_Map& equipements = MOS_App::GetApp().GetEquipements();
    for( MOS_App::CIT_MosId_String_Map it = equipements.begin(); it != equipements.end(); ++it )
        pTypesMenu_->insertItem( it->second.c_str(), it->first );

    connect( pTypeListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint &, int ) ), SLOT(OnContextMenu( QListViewItem*, const QPoint&, int )) );
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_MaintenancePriorities destructor
// Created: NLD 2003-12-03
// -----------------------------------------------------------------------------
MOS_MTH_MaintenancePriorities::~MOS_MTH_MaintenancePriorities()
{

}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MaintenancePriorities::OnContextMenu
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
void MOS_MTH_MaintenancePriorities::OnContextMenu( QListViewItem* pItem, const QPoint& pos, int )
{
    QPopupMenu* pMenu = new QPopupMenu(this);
    pMenu->insertItem( "Ajouter type", pTypesMenu_ );

    if( pItem != 0 )
        pMenu->insertItem( "Effacer type", 0 );

    int nResult = pMenu->exec( pos );
    if( nResult == -1 )
        return;

    if( nResult == 0 )
    {
        assert( pItem != 0 );
        delete pItem;
    }
    else
        new QListViewItem( pTypeListView_, pTypeListView_->lastItem(), pTypesMenu_->text( nResult ) );
        
    delete pMenu;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_MaintenancePriorities::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_MaintenancePriorities::Initialize()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MaintenancePriorities::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_MaintenancePriorities::Terminate()
{
    if( pAsnMaintenancePriorities_->n == 0 )
        return;

    delete [] pAsnMaintenancePriorities_->elem;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MaintenancePriorities::WriteMsg
// Created: NLD 2003-03-04
//-----------------------------------------------------------------------------
void MOS_MTH_MaintenancePriorities::WriteMsg(  std::string& /*sParam*/ )
{
    assert( pAsnMaintenancePriorities_ );

    pAsnMaintenancePriorities_->n = pTypeListView_->childCount();
    if( !pAsnMaintenancePriorities_->n )
        return;

    ASN1T_TypeEquipement* pPriorities = new ASN1T_TypeEquipement[ pAsnMaintenancePriorities_->n ]; //$$$ RAM

    QListViewItem* pItem = pTypeListView_->firstChild();
    uint i = 0;
    while( pItem )
    {
        pPriorities[i] = MOS_App::GetApp().GetEquipementID( (const char*)pItem->text( 0 ) );
        ++i;
        pItem = pItem->nextSibling();
    }

    pAsnMaintenancePriorities_->elem = pPriorities;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_MaintenancePriorities::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_MaintenancePriorities::FillRandomParameters()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_MaintenancePriorities::ResaveParameters
// Created: FBD 03-09-08
//-----------------------------------------------------------------------------
void MOS_MTH_MaintenancePriorities::ResaveParameters( MT_OutputArchive_ABC& /*archive*/ )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MaintenancePriorities::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_MaintenancePriorities::ReloadParameters( MT_InputArchive_ABC& /*archive*/ )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MaintenancePriorities::SetAgent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_MTH_MaintenancePriorities::SetAgent( MOS_Agent* /*pAgent*/ )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MaintenancePriorities::NotifyAgentHasMoved
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_MTH_MaintenancePriorities::NotifyAgentHasMoved( MOS_Agent& /*agent*/, const MT_Vector2D& /*vPos*/ )
{

}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MaintenancePriorities::SetMission
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_MTH_MaintenancePriorities::SetMission( MOS_Mission_ABC* /*pMission*/ )
{
}   

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MaintenancePriorities::SetWidgetTab
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_MTH_MaintenancePriorities::SetWidgetTab( QWidget* /*pWidget*/ )
{
}


