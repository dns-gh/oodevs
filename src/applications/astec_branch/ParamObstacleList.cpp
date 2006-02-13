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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamObstacleList.cpp $
// $Author: Ape $
// $Modtime: 30/09/04 14:29 $
// $Revision: 6 $
// $Workfile: ParamObstacleList.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "ParamObstacleList.h"
#include "moc_ParamObstacleList.cpp"

#include "ParamListView.h"
#include "ParamObstacle.h"

// -----------------------------------------------------------------------------
// Name: ParamObstacleList constructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacleList::ParamObstacleList( ASN1T_ListMissionGenObject& asnObjectList, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional )
    : QVBox         ( pParent )
    , Param_ABC ( bOptional )
    , asnObjectList_( asnObjectList )
    , pObstacles_   ( 0 )
    , pSelectedItem_( 0 )
    , pObstacleEditor_( 0 )
    , pPopupMenu_   ( new QPopupMenu( this ) )
{
    pListView_ = new ParamListView( strLabel, false, this );

    connect( pListView_, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChanged( QListViewItem* ) ) );
    connect( pListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint& ) ) );
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList destructor
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
ParamObstacleList::~ParamObstacleList()
{
    delete[] pObstacles_;
    while( ! asnUMTCoordPtrList_.empty() )
    {
        delete[] asnUMTCoordPtrList_.back();
        asnUMTCoordPtrList_.pop_back();
    }
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList::FillRemotePopupMenu
/** @param  popupMenu 
    @param  context 
*/
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
void ParamObstacleList::FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context )
{
    if( pListView_->selectedItem() != 0 )
        pObstacleEditor_->FillRemotePopupMenu( popupMenu, context );
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList::CheckValidity
/** @return 
*/
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
bool ParamObstacleList::CheckValidity()
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
// Name: ParamObstacleList::WriteMsg
/** @param  strMsg 
*/
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
void ParamObstacleList::WriteMsg( std::stringstream& strMsg )
{
    if( pSelectedItem_ != 0 && pObstacleEditor_ != 0 )
    {
        std::stringstream strDummy;
        pObstacleEditor_->WriteMsg( strDummy );
    }
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
// Name: ParamObstacleList::OnSelectionChanged
// Created: APE 2004-05-18
// -----------------------------------------------------------------------------
void ParamObstacleList::OnSelectionChanged( QListViewItem* pItem )
{
    if( pSelectedItem_ != 0 )
    {
        assert( pObstacleEditor_ != 0 );
        std::stringstream strDummy;
        pObstacleEditor_->WriteMsg( strDummy );
        T_ListItem* pListItem = ( T_ListItem* )pSelectedItem_;
        pListItem->GetValue().second = pObstacleEditor_->GetPointList();

        delete pObstacleEditor_;
        pObstacleEditor_ = 0;
    }

    pSelectedItem_ = pItem;
    if( pItem == 0 )
        return;

    ASN1T_MissionGenObject* pAsnObject = ((T_ListItem*)pItem)->GetValue().first;
    pObstacleEditor_ = new ParamObstacle( *pAsnObject, "Obstacle", "Obstacle", this, true, true );
    pObstacleEditor_->show();
    //$$$$ pas de tr.
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList::OnRequestPopup
/** @param  pItem 
    @param  pos 
*/
// Created: APE 2004-05-24
// -----------------------------------------------------------------------------
void ParamObstacleList::OnRequestPopup( QListViewItem* pItem, const QPoint& pos )
{
    pPopupMenu_->clear();
    pPopupMenu_->insertItem( tr( "Nouvel obstacle" ), this, SLOT( OnNewObstacle() ) );
    if( pItem != 0 )
        pPopupMenu_->insertItem( tr( "Effacer" ), this, SLOT( OnDeleteSelectedItem() ) );
    pPopupMenu_->insertItem( tr( "Effacer la liste" ), this, SLOT( OnClearList() ) );
    pPopupMenu_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList::OnNewObstacle
// Created: APE 2004-05-24
// -----------------------------------------------------------------------------
void ParamObstacleList::OnNewObstacle()
{
    ASN1T_MissionGenObject* pObject = new ASN1T_MissionGenObject();
    pObject->pos_obstacle.type = EnumTypeLocalisation::point;
    pObject->pos_obstacle.vecteur_point.n = 0;
    pObject->pos_obstacle.vecteur_point.elem = 0;
    T_Item item( pObject, T_PointVector() );
    QListViewItem* pItem = new T_ListItem( item, pListView_, tr( "Obstacle" ) );
    pListView_->setSelected( pItem, true );
}


// -----------------------------------------------------------------------------
// Name: ParamObstacleList::OnDeleteSelectedItem
// Created: APE 2004-05-24
// -----------------------------------------------------------------------------
void ParamObstacleList::OnDeleteSelectedItem()
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
// Name: ParamObstacleList::OnClearList
// Created: APE 2004-05-24
// -----------------------------------------------------------------------------
void ParamObstacleList::OnClearList()
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
