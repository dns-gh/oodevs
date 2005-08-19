//*****************************************************************************
//
// $Created: AGN 03-08-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_Obstacle_ListView.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 11 $
// $Workfile: MOS_MTH_Obstacle_ListView.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_MTH_Obstacle_ListView.h"
#include "moc_MOS_MTH_Obstacle_ListView.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_MTH_Obstacle_ListView.inl"
#endif

#include "MOS_MTH_Obstacle_Editor.h"
#include "MOS_MTH_Obstacle_ListViewItem.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_World.h"

#include <qwidgetstack.h>
#include <qheader.h>
#include <qpopupmenu.h>

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListView constructor
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
MOS_MTH_Obstacle_ListView::MOS_MTH_Obstacle_ListView( ASN1T_ListMissionGenObject& asnListLocalisation, QWidget* pParent, const std::string& strName )
    : QListView           ( pParent, strName.c_str() )
    , MOS_MissionTool_ABC ()
    , asnListLocalisation_( asnListLocalisation )
    , pAgent_             ( 0 )
    , pEditor_            ( 0 )
{
    this->addColumn( strName.c_str() );

    if( strName.empty() )
        this->header()->hide();

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint &, int ) ), SLOT(OnContextMenu( QListViewItem*, const QPoint&, int )) );
    pEditor_ = new MOS_MTH_Obstacle_Editor( MOS_App::GetApp().GetMainWindow().GetWidgetStackLeft() );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListView destructor
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
MOS_MTH_Obstacle_ListView::~MOS_MTH_Obstacle_ListView()
{
    clear();
    if( pEditor_ != 0 )
    {
        MOS_App::GetApp().GetMainWindow().UnregisterLeftWidget( pEditor_ );
        delete pEditor_;
        pEditor_ = 0;
    }    
}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListView::SetMission
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_ListView::SetMission( MOS_Mission_ABC* /*pMission*/ )
{

}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListView::SetWidgetTab
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_ListView::SetWidgetTab( QWidget* /*pWidget*/ )
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListView::SetAgent
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_ListView::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListView::WriteMsg
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_ListView::WriteMsg( std::string& sParam )
{
    uint nNbrChilds = childCount();
    sParam += MT_FormatString( "NbObstacle:%d", childCount() );
    if( nNbrChilds == 0 )
        return;

    ASN1T_MissionGenObject* pObstacles = new ASN1T_MissionGenObject[ nNbrChilds ]; //$$$ RAM

    MOS_MTH_Obstacle_ListViewItem* pItem = static_cast< MOS_MTH_Obstacle_ListViewItem* >( firstChild() );
    uint i = 0;
    while( pItem )
    {
        const T_PointVector& locPoints = pItem->GetListPoint();

        assert( !locPoints.empty() );
        sParam += MT_FormatString( "NbPointObstacle:%d | ", locPoints.size() );

        pObstacles[i].oid_obstacle_planifie = pItem->GetIDPlanifie();
        pObstacles[i].preliminaire = pItem->GetSousType();
        pObstacles[i].type_obstacle = pItem->GetObstacleType();
        pObstacles[i].urgence = pItem->GetUrgence();
        pObstacles[i].priorite = pItem->GetPriorite();

        pObstacles[i].pos_obstacle.type = pItem->GetLocalisationType();
        pObstacles[i].pos_obstacle.vecteur_point.n    = locPoints.size();
        pObstacles[i].pos_obstacle.vecteur_point.elem = new ASN1T_CoordUTM[ locPoints.size() ];
        for( uint j = 0; j < locPoints.size(); ++j )
        {
            std::string strMGRS;
            MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( locPoints[j], strMGRS );
            pObstacles[i].pos_obstacle.vecteur_point.elem[j] = strMGRS.c_str();
        }

        pItem = static_cast< MOS_MTH_Obstacle_ListViewItem* >( pItem->nextSibling() );
        ++i;
    }

    asnListLocalisation_.n    = nNbrChilds;
    asnListLocalisation_.elem = pObstacles;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListView::FillRandomParameters
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_ListView::FillRandomParameters()
{
    // TODO
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListView::ReloadParameters
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_ListView::ReloadParameters( MT_InputArchive_ABC& /*archive*/ )
{
    // TODO    
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListView::ResaveParameters
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_ListView::ResaveParameters( MT_OutputArchive_ABC& /*archive*/ )
{
    // TODO    
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListView::NotifyAgentHasMoved
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_ListView::NotifyAgentHasMoved( MOS_Agent& /*agent*/, const MT_Vector2D& /*vPos*/ )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListView::Initialize
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_ListView::Initialize()
{
    asnListLocalisation_.n = 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListView::Terminate
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_ListView::Terminate()
{
    clear();
    MOS_App::GetApp().GetMainWindow().UnregisterLeftWidget( pEditor_ );
    delete pEditor_;
    pEditor_ = 0;

    if( asnListLocalisation_.n == 0 )
        return;

    for( uint i = 0; i < asnListLocalisation_.n; ++i )
    {
        if( asnListLocalisation_.elem[i].pos_obstacle.vecteur_point.n > 0 )
            delete [] asnListLocalisation_.elem[i].pos_obstacle.vecteur_point.elem;
    }

    if( asnListLocalisation_.n > 0 )
        delete [] asnListLocalisation_.elem;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_Obstacle_ListView::OnContextMenu
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_Obstacle_ListView::OnContextMenu( QListViewItem* pItem, const QPoint& pos, int )
{

    QPopupMenu* pMenu = new QPopupMenu(this);
    pMenu->insertItem( "Ajouter obstacle", eContextMenu_AddObstacle );

    if( pItem != 0 )
    {
        pMenu->insertSeparator();
        pMenu->insertItem( "Editer obstacle", eContextMenu_EditObstacle );
        pMenu->insertItem( "Effacer obstacle", eContextMenu_RemoveObstacle );
    }

    int nResult = pMenu->exec( pos );

    if( nResult < 0 )
        return;

    switch( nResult )
    {
    case eContextMenu_AddObstacle:
        {
            MOS_MTH_Obstacle_ListViewItem* pNewItem = new MOS_MTH_Obstacle_ListViewItem( this );
            // Edit it
            pEditor_->SetEditedItem( pNewItem );
            MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pEditor_ );

            break;
        }
    case eContextMenu_EditObstacle:
        {
            assert( pItem != 0 );
            assert( pItem->rtti() == MOS_MTH_Obstacle_ListViewItem::e_rtti );

            MOS_MTH_Obstacle_ListViewItem* pNewItem = static_cast< MOS_MTH_Obstacle_ListViewItem* >( pItem );

            pEditor_->SetEditedItem( pNewItem );
            MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pEditor_ );

            break;
        }
    case eContextMenu_RemoveObstacle:
        {
            assert( pItem != 0 );
            assert( pItem->rtti() == MOS_MTH_Obstacle_ListViewItem::e_rtti );

            takeItem( pItem );
            break;
        }
    }    
}

