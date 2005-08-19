//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimaListView.cpp $
// $Author: Nld $
// $Modtime: 20/01/05 11:46 $
// $Revision: 35 $
// $Workfile: MOS_LimaListView.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_LimaListView.h"
#include "moc_MOS_LimaListView.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_LimaListView.inl"
#endif

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_GLWidget.h"
#include "MOS_Mainwindow.h"

#include "MOS_PointListViewItem.h"

#include "MOS_ListViewItem_ABC.h"
#include "MOS_LimaListViewItem.h"

#include "MOS_LimaCheckListItem.h"
#include "MOS_CheckListItem_ABC.h"


#include "MOS_LineManager.h"
#include "MOS_Lima.h"
#include "MOS_LimaSelector.h"
#include "MOS_LimaEditor.h"
#include "MOS_LimaMission.h"
#include "MOS_GLMouse_ABC.h"

#include "MOS_Agent.h"

#include <QHeader.h>
#include <QSplitter.h>

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimaListView::MOS_LimaListView( QWidget* pParent, E_ListViewType nType )
    :   QListView       ( pParent )
    ,   MOS_GLMouse_ABC ()
    ,   pSelectedPoint_ ( 0 )
    ,   pSelectedLima_  ( 0 )
    ,   bMovingPoint_   ( false )
    ,   bMovingLine_    ( false )
    ,   bDrawAll_       ( true )
    ,   nType_          ( nType )
    ,   bActive_        ( false )
    ,   bShift_         ( false )
    ,   bCtrl_          ( false )
    ,   pAgent_         ( 0 )
    ,   vOldPos_        ( 0., 0.)
    ,   sFileName_      ( "MOS_Lima.bin" )

{
    setSorting( -1 );
    header()->hide();
    addColumn( "LIMAS" );
    connect( this, SIGNAL( contextMenuRequested( QListViewItem *, const QPoint &, int ) ), this, SLOT( SlotContextMenuRequested( QListViewItem*, const QPoint & ) ) );
    connect( this, SIGNAL( pressed( QListViewItem * ) ), this, SLOT( SlotItemPressed( QListViewItem * ) ) );
    connect( this, SIGNAL( doubleClicked( QListViewItem * ) ), this, SLOT( SlotItemDoubleClicked( QListViewItem * ) ) );
    connect( this, SIGNAL( itemRenamed( QListViewItem* , int , const QString& ) ), this, SLOT( SlotItemRenamed( QListViewItem* , int , const QString& ) ) );
    setRootIsDecorated( true );

}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_LimaListView::~MOS_LimaListView()
{
}
//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::CreateLimaFromSelector
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_LimaListView::CreateLimaFromSelector( MOS_LimaListView* pSelector )
{
    assert( nType_ == eListViewMission );

    RIT_LimaPtrVector itLimaItem = limaVector_.rbegin();
    while( itLimaItem != limaVector_.rend() )
    {
        MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)*itLimaItem;
        pLima->GetLima().UnregisterListViewItem( this, pLima );
        DeleteLima( *pLima );
        itLimaItem = limaVector_.rbegin();
    }

    MOS_LineManager::T_LimaSet limaSet;
    pSelector->GetTaggedLimas( limaSet );

    for ( MOS_LineManager::IT_LimaSet itLima = limaSet.begin() ; itLima != limaSet.end() ; ++itLima )
    {
        
        MOS_Lima& lima = **itLima;

        MOS_LimaListViewItem* pLima = 0;
        if ( limaVector_.size() )
            pLima = new MOS_LimaListViewItem( eItemTypeEltLima, lima, *this, **(limaVector_.rbegin()), lima.GetName() );
        else
            pLima = new MOS_LimaListViewItem( eItemTypeEltLima, lima, *this, lima.GetName() );
        pLima->setRenameEnabled( 0 , false );
        
        RegisterLima( *pLima );
        pLima->CreateFuncLima( eLimaFuncLD );
        pLima->CreateFuncLima( eLimaFuncLCA );
        pLima->CreateFuncLima( eLimaFuncLC );
        pLima->CreateFuncLima( eLimaFuncLI );
        pLima->CreateFuncLima( eLimaFuncLO );
        pLima->CreateFuncLima( eLimaFuncLCAR );
        pLima->CreateFuncLima( eLimaFuncLR );
        pLima->CreateFuncLima( eLimaFuncLDM );
        pLima->CreateFuncLima( eLimaFuncLFM );
        pLima->CreateFuncLima( eLimaFuncLIA );
         
        CT_PointPtrVector& pointVector = lima.GetListPoints();
        for ( CIT_PointPtrVector itPoint = pointVector.begin() ; itPoint != pointVector.end() ; ++itPoint )
        {
            MOS_PointListViewItem& point = pLima->CreatePoint( **itPoint );
            point.setVisible( false );
        }
        lima.RegisterListViewItem( this, pLima );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::GetTaggedLimas
// Created: FBD 03-01-09
//-----------------------------------------------------------------------------
void MOS_LimaListView::GetTaggedLimas( MOS_LineManager::T_LimaSet& limaSet )
{
    assert( nType_ == eListViewSelector );
    for ( IT_LimaPtrVector itLima = limaVector_.begin() ; itLima != limaVector_.end() ; ++itLima )
    {
        MOS_LimaCheckListItem* pLima = (MOS_LimaCheckListItem*)*itLima;
        if ( pLima->isOn() )
            limaSet.insert( &pLima->GetLima() );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimaListView::Initialize()
{
    MOS_App::GetApp().GetLineManager().RegisterLimaListView( *this );
    MOS_GLMouse_ABC::Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimaListView::Terminate()
{
    MOS_GLMouse_ABC::Terminate();
    MOS_App::GetApp().GetLineManager().UnregisterLimaListView( *this );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::CreateLima
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
QListViewItem* MOS_LimaListView::CreateLima( MOS_Lima& lima )
{
    if ( nType_ == eListViewSelector )
    {
        MOS_LimaCheckListItem* pLima = 0;
        if ( limaVector_.size() )
            pLima = new MOS_LimaCheckListItem( eItemTypeEltLima, lima, *this, **(limaVector_.rbegin()), QCheckListItem::CheckBox, lima.GetName() );
        else
            pLima = new MOS_LimaCheckListItem( eItemTypeEltLima, lima, *this, QCheckListItem::CheckBox, lima.GetName()  );
        
        pLima->setRenameEnabled( 0, false );
        RegisterLima( *pLima );
        CT_PointPtrVector& pointVector = lima.GetListPoints();
        for ( CIT_PointPtrVector itPoint = pointVector.begin() ; itPoint != pointVector.end() ; ++itPoint )
        {
            MOS_PointListViewItem& point = pLima->CreatePoint( **itPoint );
            point.setEnabled( false );

        }

        return pLima;

    }
    else
    if ( nType_ == eListViewEditor )
    {
        MOS_LimaListViewItem* pLima = 0;
        if ( limaVector_.size() )
            pLima = new MOS_LimaListViewItem( eItemTypeEltLima, lima, *this, **(limaVector_.rbegin()), lima.GetName() );
        else
            pLima = new MOS_LimaListViewItem( eItemTypeEltLima, lima, *this, lima.GetName() );

        pLima->CreateFuncLima( eLimaFuncLD );
        pLima->CreateFuncLima( eLimaFuncLCA );
        pLima->CreateFuncLima( eLimaFuncLC );
        pLima->CreateFuncLima( eLimaFuncLI );
        pLima->CreateFuncLima( eLimaFuncLO );
        pLima->CreateFuncLima( eLimaFuncLCAR );
        pLima->CreateFuncLima( eLimaFuncLR );
        pLima->CreateFuncLima( eLimaFuncLDM );
        pLima->CreateFuncLima( eLimaFuncLFM );
        pLima->CreateFuncLima( eLimaFuncLIA );
        
        RegisterLima( *pLima );

        CT_PointPtrVector& pointVector = lima.GetListPoints();
        for ( CIT_PointPtrVector itPoint = pointVector.begin() ; itPoint != pointVector.end() ; ++itPoint )
            pLima->CreatePoint( **itPoint );
        return pLima;
    }
    else
    if( nType_ == eListViewMission )
    {
        return 0;
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::DeleteLima
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_LimaListView::DeleteLima( QListViewItem& lima )
{
    delete &lima;
    UnregisterLima( lima );
}
//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::RegisterLima
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_LimaListView::RegisterLima( QListViewItem& lima )
{
    limaVector_.push_back( &lima );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::UnregisterLima
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_LimaListView::UnregisterLima( QListViewItem& lima )
{
    IT_LimaPtrVector itLima = std::find( limaVector_.begin(), limaVector_.end(), &lima );
    assert( itLima != limaVector_.end() );
    limaVector_.erase( itLima );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::WriteMsg
// Created: NLD 2003-03-01
//-----------------------------------------------------------------------------
void MOS_LimaListView::WriteMsg( ASN1T_ListOID& asnLimas, std::string& sParam )
{
    assert( nType_ == eListViewMission );

    if ( limaVector_.size() )
        sParam += MT_FormatString( "%d Lima | ", limaVector_.size()  );
    else
        sParam += "No Lima |";


    asnLimas.n    = limaVector_.size();
    asnLimas.elem = new ASN1T_OID[ limaVector_.size() ]; //$$$$ RAM

    int i = 0;
    for( IT_LimaPtrVector itLima = limaVector_.begin() ; itLima != limaVector_.end() ; ++itLima )
    {
        MOS_LimaListViewItem* pLimaItem = (MOS_LimaListViewItem*)(*itLima);
        pLimaItem->WriteMsg( asnLimas.elem[i], sParam );
        ++i;
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::GetListLima
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
MOS_LimaListView::T_LimaPtrVector& MOS_LimaListView::GetListLima()
{
    return limaVector_;

}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_LimaListView::FillRandomParameters()
{
    assert( nType_ == eListViewMission );

    RIT_LimaPtrVector itLimaItem = limaVector_.rbegin();
    while( itLimaItem != limaVector_.rend() )
    {
        MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)*itLimaItem;
        pLima->GetLima().UnregisterListViewItem( this, pLima );
        DeleteLima( *pLima );
        itLimaItem = limaVector_.rbegin();
    }


    T_LimaPtrVector& limaVectorEditor = MOS_App::GetApp().GetMainWindow().GetLimaEditor().GetListLima();
    if ( limaVectorEditor.size() < 1 )
        return;


    MOS_LineManager::T_LimaSet limaSet;
    uint nWantedLima = rand() % limaVectorEditor.size();
    while ( limaSet.size() < nWantedLima )
    {
        for ( IT_LimaPtrVector itLima = limaVectorEditor.begin() ; itLima != limaVectorEditor.end() ; ++itLima )
        {
            MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)*itLima;
            uint nValue = rand() % 2;
            if ( nValue == 1 )
            {
                limaSet.insert( &pLima->GetLima() );
            }
        }
    }

    for ( MOS_LineManager::IT_LimaSet itLima = limaSet.begin() ; itLima != limaSet.end() ; ++itLima )
    {
        
        MOS_Lima& lima = **itLima;

        MOS_LimaListViewItem* pLima = 0;
        if ( limaVector_.size() )
            pLima = new MOS_LimaListViewItem( eItemTypeEltLima, lima, *this, **(limaVector_.rbegin()), lima.GetName() );
        else
            pLima = new MOS_LimaListViewItem( eItemTypeEltLima, lima, *this, lima.GetName() );
        pLima->setRenameEnabled( 0 , false );
        
        RegisterLima( *pLima );
        pLima->CreateFuncLima( eLimaFuncLD );
        pLima->CreateFuncLima( eLimaFuncLCA );
        pLima->CreateFuncLima( eLimaFuncLC );
        pLima->CreateFuncLima( eLimaFuncLI );
        pLima->CreateFuncLima( eLimaFuncLO );
        pLima->CreateFuncLima( eLimaFuncLCAR );
        pLima->CreateFuncLima( eLimaFuncLR );
        pLima->CreateFuncLima( eLimaFuncLDM );
        pLima->CreateFuncLima( eLimaFuncLFM );
        pLima->CreateFuncLima( eLimaFuncLIA );
 
        CT_PointPtrVector& pointVector = lima.GetListPoints();
        for ( CIT_PointPtrVector itPoint = pointVector.begin() ; itPoint != pointVector.end() ; ++itPoint )
        {
            MOS_PointListViewItem& point = pLima->CreatePoint( **itPoint );
            point.setVisible( false );
        }
        lima.RegisterListViewItem( this, pLima );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::ResaveParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_LimaListView::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    assert( nType_ == eListViewMission );

    T_LimaPtrVector& LimaVectorEditor = MOS_App::GetApp().GetMainWindow().GetLimaEditor().GetListLima();
    if ( LimaVectorEditor.size() < 2 )
        return;

    uint nNbLima = limaVector_.size();
    archive.WriteField( "sName", std::string( "MOS_LimaListView|nNbLima" ) );
    archive.WriteField( "nNbLima", nNbLima );

    for ( IT_LimaPtrVector itLima = limaVector_.begin() ; itLima != limaVector_.end() ; ++itLima )
    {
        MOS_LimaListViewItem* pLimaItem = (MOS_LimaListViewItem*)*itLima;

        uint nLima = 0;
        T_LimaPtrVector& LimaVectorEditor = MOS_App::GetApp().GetMainWindow().GetLimaEditor().GetListLima();
        for ( IT_LimaPtrVector itLimaEditor = LimaVectorEditor.begin() ; itLimaEditor != LimaVectorEditor.end() ; ++itLimaEditor  )
        {
            MOS_LimaListViewItem* pLimaItemDest = (MOS_LimaListViewItem*)*itLimaEditor;

            if ( &pLimaItem->GetLima() == &pLimaItemDest->GetLima() )
            {
                archive.WriteField( "sName", std::string( "MOS_LimaListView|nLima" ) );
                archive.WriteField( "nLima", nLima );
                break;
            }
            ++nLima;
        }
    }
 
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_LimaListView::ReloadParameters( MT_InputArchive_ABC& archive )
{
    assert( nType_ == eListViewMission );

    RIT_LimaPtrVector itLimaItem = limaVector_.rbegin();
    while( itLimaItem != limaVector_.rend() )
    {
        MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)*itLimaItem;
        pLima->GetLima().UnregisterListViewItem( this, pLima );
        DeleteLima( *pLima );
        itLimaItem = limaVector_.rbegin();
    }


    T_LimaPtrVector& limaVectorEditor = MOS_App::GetApp().GetMainWindow().GetLimaEditor().GetListLima();
    if ( limaVectorEditor.size() < 1 )
        return;


    uint nNbLima = 0;
    std::string sName( "MOS_LimaListView|nNbLima" );
    archive.ReadField( "sName", sName );
    archive.ReadField( "nNbLima", nNbLima );

    MOS_LineManager::T_LimaSet limaSet;
    for ( uint j = 0 ; j < nNbLima ; ++j )
    {
        uint nLima = 0;
        std::string sName( "MOS_LimaListView|nLima" );
        archive.ReadField( "sName", sName );
        archive.ReadField( "nLima", nLima );
        limaSet.insert( &((MOS_LimaListViewItem*)limaVectorEditor[nLima])->GetLima() );
    }

    for ( MOS_LineManager::IT_LimaSet itLima = limaSet.begin() ; itLima != limaSet.end() ; ++itLima )
    {
        
        MOS_Lima& lima = **itLima;

        MOS_LimaListViewItem* pLima = 0;
        if ( limaVector_.size() )
            pLima = new MOS_LimaListViewItem( eItemTypeEltLima, lima, *this, **(limaVector_.rbegin()), lima.GetName() );
        else
            pLima = new MOS_LimaListViewItem( eItemTypeEltLima, lima, *this, lima.GetName() );
        pLima->setRenameEnabled( 0 , false );
        
        RegisterLima( *pLima );
        pLima->CreateFuncLima( eLimaFuncLD );
        pLima->CreateFuncLima( eLimaFuncLCA );
        pLima->CreateFuncLima( eLimaFuncLC );
        pLima->CreateFuncLima( eLimaFuncLI );
        pLima->CreateFuncLima( eLimaFuncLO );
        pLima->CreateFuncLima( eLimaFuncLCAR );
        pLima->CreateFuncLima( eLimaFuncLR );
        pLima->CreateFuncLima( eLimaFuncLDM );
        pLima->CreateFuncLima( eLimaFuncLFM );
        pLima->CreateFuncLima( eLimaFuncLIA );
 
        CT_PointPtrVector& pointVector = lima.GetListPoints();
        for ( CIT_PointPtrVector itPoint = pointVector.begin() ; itPoint != pointVector.end() ; ++itPoint )
        {
            MOS_PointListViewItem& point = pLima->CreatePoint( **itPoint );
            point.setVisible( false );
        }
        lima.RegisterListViewItem( this, pLima );
    }
}

//=============================================================================
// SLOTS
//=============================================================================
//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::SlotContextMenuRequested
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_LimaListView::SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point )
{
    if ( nType_ == eListViewEditor )
    {
        // PopupMenu
        QPopupMenu* pMenu = new QPopupMenu( this );
    
        if( !pItem ) 
        {
            pMenu->insertItem( QString( "Add LIMA"), 0 );
        }
        else
        {
            switch( ((MOS_ListViewItem_ABC*)pItem)->GetItemType() )
            {
                case eItemTypeEltLima: 
                {
                    pMenu->insertItem( QString( "Add LIMA"), 0 );
                    pMenu->insertItem( QString( "Add point" ), 2 );
                    pMenu->insertItem( QString( "Del LIMA %1" ).arg( pSelectedLima_->text(0) ), 1 );

                }
                break;

                case eItemTypeEltPoint:
                {
                    pMenu->insertItem( QString( "Add point" ), 2 );
                    pMenu->insertItem( QString( "Del point" ), 3 );
                }
                break;
            }
        }

        int nSelected = pMenu->exec( point ) ;
        if ( nSelected == -1)
            return;

        if ( nSelected == 0 )
        {
            pSelectedLima_ =  CreateLimaFromInterface();
            pSelectedPoint_ = CreatePointFromInterface( *pSelectedLima_ );
            //APE LocOnPoint( pSelectedPoint_ );

        }
        else
        if ( nSelected == 1 )
        {
            if ( pSelectedLima_ )
                DeleteLimaFromInterface( *pSelectedLima_ );
        }
        else
        if ( nSelected == 2 )
        {
            if ( pSelectedLima_ )
            {
                pSelectedPoint_ = CreatePointFromInterface( *pSelectedLima_, pSelectedPoint_ );
                //APE LocOnPoint( pSelectedPoint_ );
            }
        }
        else
        if ( nSelected == 3 )
        {
            if ( pSelectedLima_ && pSelectedPoint_ )
            {
            
                if ( nType_ == eListViewSelector )
                {
                    MOS_LimaCheckListItem* pLima = static_cast< MOS_LimaCheckListItem* >( pSelectedLima_ );
                    MOS_PointListViewItem* pNewPoint = pLima->GetPointBefore( *pSelectedPoint_ );
                    DeletePointFromInterface( *pSelectedLima_, *pSelectedPoint_ );
                    if ( pNewPoint == 0)
                        pNewPoint = pLima->GetFirstPoint();

                    pSelectedPoint_ = pNewPoint;
                    //APE LocOnPoint( pSelectedPoint_ );
                }
                else
                {
                    MOS_LimaListViewItem* pLima = static_cast< MOS_LimaListViewItem* >( pSelectedLima_ );
                    MOS_PointListViewItem* pNewPoint = pLima->GetPointBefore( *pSelectedPoint_ );
                    DeletePointFromInterface( *pSelectedLima_, *pSelectedPoint_ );
                    if ( pNewPoint == 0)
                        pNewPoint = pLima->GetFirstPoint();

                    pSelectedPoint_ = pNewPoint;
                    //APE LocOnPoint( pSelectedPoint_ );
                }

            }
        }
    }
    else
    if ( nType_ == eListViewMission )
    {
        // PopupMenu
        QPopupMenu* pMenu = new QPopupMenu( this );
        if( !pItem ) 
        {
            pMenu->insertItem( QString( "Add LIMA"), 0 );
        }

        int nSelected = pMenu->exec( point ) ;
        if ( nSelected == -1)
            return;

        if ( nSelected == 0 )
        {
            EnterLimaSelector();
        }
    }
    else
    if ( nType_ == eListViewSelector )
    {
        // PopupMenu
        QPopupMenu* pMenu = new QPopupMenu( this );
        if( !pItem ) 
        {
            pMenu->insertItem( QString( "Edit LIMA"), 0 );
        }

        int nSelected = pMenu->exec( point ) ;
        if ( nSelected == -1)
            return;

        if ( nSelected == 0 )
        {
            EnterLimaEditor();
        }
    }
        
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::EnterLimaEditor
// Created: FBD 03-02-11
//-----------------------------------------------------------------------------
void MOS_LimaListView::EnterLimaEditor()
{
    MOS_LimaListView* pListView = &MOS_App::GetApp().GetMainWindow().GetLimaEditor();
    MOS_LimaEditor* pEditor = (MOS_LimaEditor*)pListView->parent();
    MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pEditor );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::EnterLimaSelector
// Created: FBD 03-02-11
//-----------------------------------------------------------------------------
void MOS_LimaListView::EnterLimaSelector()
{
    MOS_LimaListView* pListView = &MOS_App::GetApp().GetMainWindow().GetLimaSelector();
    MOS_LimaSelector* pSelector = (MOS_LimaSelector*)pListView->parent();
    pSelector->SetLimaMission( *this );
    MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pSelector );
    
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::LocOnPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_LimaListView::LocOnPoint( MOS_PointListViewItem* pPoint, bool bSelectList )
{
    if ( pPoint )
    {
        if ( bSelectList )
            setSelected( pPoint, true );
        MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().SetWantedCenter( pPoint->GetPoint() );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::SlotItemPressed
// Created: NLD 2002-07-18
//-----------------------------------------------------------------------------
void MOS_LimaListView::SlotItemPressed( QListViewItem* pItem )
{
    if( !pItem ) 
        return;

    E_ItemType nType = eItemTypeNone;
    if ( nType_ == eListViewSelector )
    {
        MOS_CheckListItem_ABC* pTmpItem = (MOS_CheckListItem_ABC*)pItem;
        nType = pTmpItem->GetItemType();
    }
    else
    {
        MOS_ListViewItem_ABC* pTmpItem = (MOS_ListViewItem_ABC*)pItem;
        nType = pTmpItem->GetItemType();
    }

    // Lima
    if( nType == eItemTypeEltLima )
    {
        pSelectedLima_ = pItem;
        if ( nType_ == eListViewSelector )
        {
            MOS_LimaCheckListItem* pLima = (MOS_LimaCheckListItem*)pSelectedLima_;
            pSelectedPoint_ = pLima->GetFirstPoint();
        }
        else
        {
            MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)pSelectedLima_;
            pSelectedPoint_ = pLima->GetFirstPoint();
        }

    }
    // Point
    else
    if( nType == eItemTypeEltPoint )
    {
        pSelectedPoint_ = static_cast< MOS_PointListViewItem* >( pItem );
        pSelectedLima_ = static_cast< MOS_LimaListViewItem* >( pItem->parent() );
        LocOnPoint( pSelectedPoint_, false );
    }
    else
    if( nType == eItemTypeEltFuncLima )
    {
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::SlotItemDoubleClicked
// Created: NLD 2002-07-18
//-----------------------------------------------------------------------------
void MOS_LimaListView::SlotItemDoubleClicked( QListViewItem* pItem )
{
    if( !pItem || !pSelectedPoint_) 
        return;

    LocOnPoint( pSelectedPoint_, false );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimaListView::Draw()
{
    for ( CIT_LimaPtrVector itLima = limaVector_.begin() ; itLima != limaVector_.end(); ++itLima )
    {
        if ( (*itLima) == pSelectedLima_ || bDrawAll_ )
        {
            if ( nType_ == eListViewSelector )
            {
                MOS_LimaCheckListItem* pLima = (MOS_LimaCheckListItem*)*itLima;
                pLima->Draw( (MOS_LimaCheckListItem*)pSelectedLima_, pSelectedPoint_ );
            }
            else
            {
                MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)( *itLima );
                pLima->Draw( (MOS_LimaListViewItem*)pSelectedLima_, pSelectedPoint_ );
            }
        }
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::LMBPressEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_LimaListView::LMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    return false;    
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::RMBPressEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
bool MOS_LimaListView::RMBPressEvent( const MT_Vector2D& vPos )
{
    if ( nType_ != eListViewEditor )
        return false;

    bool bReturn = false;
    
    if ( !bMovingPoint_ && !bMovingLine_ )
    {
        for ( CIT_LimaPtrVector itLima = limaVector_.begin() ; itLima != limaVector_.end(); ++itLima )
        {
            if ( !bDrawAll_ )
                if ( pSelectedLima_ != (*itLima) )
                    continue;

            if ( bShift_ )
            {
                MT_Vector2D vNewPos( vPos );
                vOldPos_ = vPos;
                pSelectedPoint_ = CreatePointFromInterface( *pSelectedLima_, pSelectedPoint_ ,&vNewPos );
                //APE LocOnPoint( pSelectedPoint_ );
                return true;
            }
            else
            if ( bCtrl_ )
            {
                MOS_PointListViewItem* pCurPoint = 0;
                if ( nType_ == eListViewEditor )
                {
//                    MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)( *itLima );
                    pCurPoint = pSelectedPoint_;
                }

                if ( pCurPoint )
                {
                    vPosMovingLine_ = vPos;
                    pSelectedPoint_ = pCurPoint;
                    pSelectedLima_ = static_cast< MOS_LimaListViewItem* >(pCurPoint->parent());
                    setSelected( pSelectedPoint_, true );
                    bMovingLine_ = true;
                    pSelectedPoint_->parent()->setOpen(true);
                    bReturn = true;
                }


            }
            else
            {

                MOS_PointListViewItem* pCurPoint = 0;
                if ( nType_ == eListViewSelector )
                {
                    MOS_LimaCheckListItem* pLima = (MOS_LimaCheckListItem*)*itLima;
                    pCurPoint = pLima->IsInsidePoint( vPos );
                }
                else
                {
                    MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)( *itLima );
                    pCurPoint = pLima->IsInsidePoint( vPos );
                }

                if ( pCurPoint )
                {
                    pSelectedPoint_ = pCurPoint;
                    pSelectedLima_ = static_cast< MOS_LimaListViewItem* >(pCurPoint->parent());

                    setSelected( pSelectedPoint_, true );
                    bMovingPoint_ = true;
                    pSelectedPoint_->parent()->setOpen(true);
                    bReturn = true;
                }
            
            }
        }
    }
    return bReturn;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::MouseMoveEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimaListView::MouseMoveEvent( const MT_Vector2D& vPos )
{
    if ( nType_ != eListViewEditor )
        return;

    if ( bMovingPoint_ && pSelectedLima_ && pSelectedPoint_ )
    {
        SetPosPoint( *pSelectedLima_, *pSelectedPoint_, vPos );
        vOldPos_ = vPos;
    }
    else
    if ( bMovingLine_ && pSelectedLima_ && pSelectedPoint_ )
    {
        SetPosLima( *pSelectedLima_, vPos, vPosMovingLine_ );
    }

}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::LMBReleaseEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_LimaListView::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::RightMouseReleaseEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimaListView::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    if ( nType_ != eListViewEditor )
        return;

    if ( bMovingPoint_ )
        bMovingPoint_ = false;
    if ( bMovingLine_ )
        bMovingLine_ = false;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::SetPosPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_LimaListView::SetPosPoint( QListViewItem& lima, MOS_PointListViewItem& point, const MT_Vector2D& vPos )
{
    if ( nType_ == eListViewSelector )
    {
        MOS_LimaCheckListItem* pLima = (MOS_LimaCheckListItem*)&lima;
        pLima->GetLima().SetState( eStateLimaCoordModified );
    }
    else
    {
        MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)&lima;
        pLima->GetLima().SetState( eStateLimaCoordModified );
    }
    point.SetPos( vPos );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::SetPosLima
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_LimaListView::SetPosLima( QListViewItem& lima, const MT_Vector2D& vPos, MT_Vector2D& vPosMovingLine )
{
    if ( nType_ == eListViewSelector )
    {
        MOS_LimaCheckListItem* pLima = (MOS_LimaCheckListItem*)&lima;
        pLima->GetLima().SetState( eStateLimaCoordModified );
        pLima->SetPos( vPos, vPosMovingLine );
    }
    else
    {
        MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)&lima;
        pLima->GetLima().SetState( eStateLimaCoordModified );
        pLima->SetPos( vPos, vPosMovingLine );
    }

}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::CreateLimaFromInterface
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
QListViewItem* MOS_LimaListView::CreateLimaFromInterface()
{
    MOS_Lima* pLima = new MOS_Lima( true );
    pLima->SetState( eStateLimaCreated );
    pLima->SetNetworkState( eStateLimaNotRegistered );
    return pLima->GetItem( *this );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::DeleteLimaFromInterface
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_LimaListView::DeleteLimaFromInterface( QListViewItem& lima )
{
    if ( nType_ == eListViewSelector )
    {
        MOS_LimaCheckListItem* pLima = (MOS_LimaCheckListItem*)&lima;
        pLima->GetLima().SetState( eStateLimaDeleted );
    }
    else
    {
        MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)&lima;
        pLima->GetLima().SetState( eStateLimaDeleted );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::CreatePointFromInterface
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_LimaListView::CreatePointFromInterface( QListViewItem& lima, MOS_PointListViewItem* pPoint, MT_Vector2D* pPos )
{
    if ( nType_ == eListViewSelector )
    {
        MOS_LimaCheckListItem* pLima = (MOS_LimaCheckListItem*)&lima;
        pLima->GetLima().SetState( eStateLimaCoordModified );
        if ( pPos )
        {
            const MT_Vector2D& vPoint = pLima->GetLima().CreatePoint( *pPos, pPoint );
            return pLima->GetPoint( vPoint );
        }
        else
        {
            const MT_Vector2D& vPoint = pLima->GetLima().CreatePoint( pLima->GetCenterPoint( pPoint ), pPoint );
            return pLima->GetPoint( vPoint );
        }

    }
    else
    {
        MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)&lima;
        pLima->GetLima().SetState( eStateLimaCoordModified );
        if ( pPos )
        {
            const MT_Vector2D& vPoint = pLima->GetLima().CreatePoint( *pPos, pPoint );
            return pLima->GetPoint( vPoint );
        }
        else
        {
            const MT_Vector2D& vPoint = pLima->GetLima().CreatePoint( pLima->GetCenterPoint( pPoint ), pPoint );
            return pLima->GetPoint( vPoint );
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::DeletePointFromInterface
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_LimaListView::DeletePointFromInterface( QListViewItem& lima, MOS_PointListViewItem& point )
{
    MT_Vector2D& vPoint = point.GetPoint();
    if ( nType_ == eListViewSelector )
    {
        MOS_LimaCheckListItem* pLima = (MOS_LimaCheckListItem*)&lima;
        pLima->GetLima().SetState( eStateLimaCoordModified );
        pLima->GetLima().DeletePoint( vPoint );
    }
    else
    {
        MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)&lima;
        pLima->GetLima().SetState( eStateLimaCoordModified );
        pLima->GetLima().DeletePoint( vPoint );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::SlotSetShowAll
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_LimaListView::SlotSetShowAll( bool bOn )
{
    bDrawAll_ = bOn;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::SlotItemRenamed
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
void MOS_LimaListView::SlotItemRenamed( QListViewItem* pItem, int /*nCol*/, const QString& sText )
{
    MOS_LimaListViewItem* pLima = static_cast< MOS_LimaListViewItem* >( pItem );
    pLima->GetLima().SetState( eStateLimaCoordModified );
    pLima->GetLima().SetName( std::string( sText ) );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::mousePressEvent
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
void MOS_LimaListView::mousePressEvent( QMouseEvent* /*pMouseEvent*/ )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_LimaListView::IsActive( bool bMove )
{
    if ( nType_ == eListViewSelector )
    {
        MOS_LimaSelector* pParent = (MOS_LimaSelector*)parent();
        return pParent->IsActive( bMove );
    }
    else
    if ( nType_ == eListViewEditor )
    {
        MOS_LimaEditor* pParent = (MOS_LimaEditor*)parent();
        return pParent->IsActive( bMove );
    }
    else
    if ( nType_ == eListViewMission )
    {
        MOS_LimaMission* pParent = (MOS_LimaMission*)parent();
        return pParent->IsActive( bMove );
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::keyPressEvent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_LimaListView::keyPressEvent( QKeyEvent * e )
{
   if ( e->key() == Qt::Key_Shift )
   {
        bShift_ = true;        
   }
   else
   if ( e->key() == Qt::Key_Control )
   {
        bCtrl_ = true;        
   }
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::keyPressEvent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_LimaListView::keyReleaseEvent( QKeyEvent * e )
{
   if ( e->key() == Qt::Key_Shift )
   {
        bShift_ = false;        
   }
   else
   if ( e->key() == Qt::Key_Control )
   {
        bCtrl_ = false;        
   }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::SetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_LimaListView::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListView::GetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
MOS_Agent* MOS_LimaListView::GetAgent()
{
    return pAgent_;    
}

//-----------------------------------------------------------------------------
//  Name  :  MOS_LimaListView::WriteForGEN
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
int MOS_LimaListView::Write() const
{
    if ( nType_ != eListViewEditor )
        return 0;
    
    MT_FlatBinaryOutputArchive archive;
    uint nlimas = limaVector_.size();
    archive.WriteField( "limas", nlimas );
    
    for( CIT_LimaPtrVector itlima = limaVector_.begin(); itlima != limaVector_.end(); ++itlima )    
    {
        MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)(*itlima);
        pLima->GetLima().Write( archive );
    }     
    archive.WriteToFile( sFileName_ );
    return 0;
}

//-----------------------------------------------------------------------------
//  Name  :  MOS_LimaListView::WriteForGEN
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
int MOS_LimaListView::Read()
{
    if( !MOS_App::GetApp().GetLineManager().GetLimaList().empty() )
        return 0;

    if ( nType_ != eListViewEditor )
        return 0;
    
    MT_FlatBinaryInputArchive archive;
    archive.Open( sFileName_ );

    uint nlimas = 0;
    archive.ReadField( "limas", nlimas );
    
    for ( uint i = 0 ; i < nlimas ; ++i )
    {
        MOS_LimaListViewItem* pLima = (MOS_LimaListViewItem*)CreateLimaFromInterface();
        pLima->GetLima().Read( archive );
        pLima->GetLima().SetState( eStateLimaCreated );
        pLima->GetLima().SetNetworkState( eStateLimaNotRegistered );
    }
    
    MOS_App::GetApp().GetLineManager().UpdateLimaToSim();

    return 0;
}

