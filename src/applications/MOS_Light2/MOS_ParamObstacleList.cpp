// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamObstacleList.cpp $
// $Author: Ape $
// $Modtime: 30/09/04 14:29 $
// $Revision: 6 $
// $Workfile: MOS_ParamObstacleList.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_ParamObstacleList.h"
#include "moc_MOS_ParamObstacleList.cpp"

#include "MOS_ParamListView.h"
#include "MOS_ParamObstacle.h"

// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacleList constructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
MOS_ParamObstacleList::MOS_ParamObstacleList( ASN1T_ListMissionGenObject& asnObjectList, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional )
    : QVBox         ( pParent )
    , MOS_Param_ABC ( bOptional )
    , asnObjectList_( asnObjectList )
    , pObstacles_   ( 0 )
    , pSelectedItem_( 0 )
    , pObstacleEditor_( 0 )
    , pPopupMenu_   ( new QPopupMenu( this ) )
{
    pListView_ = new MOS_ParamListView( strLabel, false, this );

    connect( pListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( pListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacleList destructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
MOS_ParamObstacleList::~MOS_ParamObstacleList()
{
    delete[] pObstacles_;
    while( ! asnUMTCoordPtrList_.empty() )
    {
        delete[] asnUMTCoordPtrList_.back();
        asnUMTCoordPtrList_.pop_back();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacleList::FillRemotePopupMenu
/** @param  popupMenu 
    @param  context 
*/
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
void MOS_ParamObstacleList::FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context )
{
    if( pListView_->selectedItem() != 0 )
        pObstacleEditor_->FillRemotePopupMenu( popupMenu, context );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacleList::CheckValidity
/** @return 
*/
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
bool MOS_ParamObstacleList::CheckValidity()
{
    if( pListView_->childCount() == 0 )
    {
        pListView_->TurnHeaderRed( 3000 );
        return false;
    }

    if( pListView_->selectedItem() != 0 )
    {
        std::stringstream strDummy;
        pObstacleEditor_->WriteMsg( strDummy );
    }

    QListViewItem* pItem = pListView_->firstChild();
    while( pItem != 0)
    {
        ASN1T_MissionGenObject* pAsnObject = ((T_ListItem*)pItem)->GetValue().first;
        if( pAsnObject->pos_obstacle.vecteur_point.n == 0 )
        {
            pListView_->TurnHeaderRed( 3000 );
            return false;
        }
        pItem = pItem->nextSibling();
    }

    return true;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacleList::WriteMsg
/** @param  strMsg 
*/
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
void MOS_ParamObstacleList::WriteMsg( std::stringstream& strMsg )
{
    uint nNbrChilds = pListView_->childCount();
    strMsg << pListView_->header()->label( 0 ).latin1() << ": " << nNbrChilds << " obstacles.";
    asnObjectList_.n    = nNbrChilds;

    assert( !( nNbrChilds == 0 && !IsOptional() ) );
    if( nNbrChilds == 0 && IsOptional() )
        return;

    delete[] pObstacles_;
    pObstacles_ = new ASN1T_MissionGenObject[ nNbrChilds ];
    asnObjectList_.elem = pObstacles_;

    uint i = 0;
    QListViewItem* pItem = pListView_->firstChild();
    while( pItem )
    {
        ASN1T_MissionGenObject* pAsnObject = ((T_ListItem*)pItem)->GetValue().first;
        this->Copy( *pAsnObject, pObstacles_[i] );

        pItem = pItem->nextSibling();
        ++i;
    }
}

    
// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacleList::OnSelectionChanged
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
void MOS_ParamObstacleList::OnSelectionChanged( QListViewItem* pItem )
{
    if( pSelectedItem_ != 0 )
    {
        assert( pObstacleEditor_ != 0 );
        std::stringstream strDummy;
        pObstacleEditor_->WriteMsg( strDummy );
        ((T_ListItem*)pSelectedItem_)->GetValue().second = pObstacleEditor_->GetPointList();

        delete pObstacleEditor_;
        pObstacleEditor_ = 0;
    }

    pSelectedItem_ = pItem;
    if( pItem == 0 )
        return;

    ASN1T_MissionGenObject* pAsnObject = ((T_ListItem*)pItem)->GetValue().first;
    pObstacleEditor_ = new MOS_ParamObstacle( *pAsnObject, "Obstacle", "Obstacle", this, true );
    pObstacleEditor_->show();
    //$$$$ pas de tr.
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacleList::OnRequestPopup
/** @param  pItem 
    @param  pos 
*/
// Created: APE 2004-05-24
// -----------------------------------------------------------------------------
void MOS_ParamObstacleList::OnRequestPopup( QListViewItem* pItem, const QPoint& pos )
{
    pPopupMenu_->clear();
    pPopupMenu_->insertItem( tr( "Nouvel obstacle" ), this, SLOT( OnNewObstacle() ) );
    if( pItem != 0 )
        pPopupMenu_->insertItem( tr( "Effacer" ), this, SLOT( OnDeleteSelectedItem() ) );
    pPopupMenu_->insertItem( tr( "Effacer la liste" ), this, SLOT( OnClearList() ) );
    pPopupMenu_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacleList::OnNewObstacle
// Created: APE 2004-05-24
// -----------------------------------------------------------------------------
void MOS_ParamObstacleList::OnNewObstacle()
{
    ASN1T_MissionGenObject* pObject = new ASN1T_MissionGenObject();
    pObject->pos_obstacle.type = EnumTypeLocalisation::polygon;
    pObject->pos_obstacle.vecteur_point.n = 0;
    pObject->pos_obstacle.vecteur_point.elem = 0;
    T_Item item( pObject, T_PointVector() );
    QListViewItem* pItem = new T_ListItem( item, pListView_, tr( "Obstacle" ) );
    pListView_->setSelected( pItem, true );
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacleList::OnDeleteSelectedItem
// Created: APE 2004-05-24
// -----------------------------------------------------------------------------
void MOS_ParamObstacleList::OnDeleteSelectedItem()
{
    assert( pObstacleEditor_ != 0 && pListView_->selectedItem() != 0 );

    T_ListItem* pItem = (T_ListItem*)pListView_->selectedItem();
    ASN1T_MissionGenObject* pAsnObject = ((T_ListItem*)pItem)->GetValue().first;
    pListView_->setSelected( pItem, false );
    delete pAsnObject;
    delete pItem;
    pSelectedItem_ = 0;
}


// -----------------------------------------------------------------------------
// Name: MOS_ParamObstacleList::OnClearList
// Created: APE 2004-05-24
// -----------------------------------------------------------------------------
void MOS_ParamObstacleList::OnClearList()
{
    if( pListView_->selectedItem() != 0 )
        pListView_->setSelected( pListView_->selectedItem(), false );

    while( pListView_->childCount() != 0 )
    {
        ASN1T_MissionGenObject* pAsnObject = ((T_ListItem*)pListView_->firstChild())->GetValue().first;
        delete pAsnObject;
        delete pListView_->firstChild();
    }
    pSelectedItem_ = 0;
}
