//*****************************************************************************
//
// $Created: AGN 03-08-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_PointList_ListView.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 12 $
// $Workfile: MOS_MTH_PointList_ListView.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_MTH_PointList_ListView.h"
#include "moc_MOS_MTH_PointList_ListView.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_MTH_PointList_ListView.inl"
#endif

#include "MOS_MTH_PointList_Editor.h"
#include "MOS_MTH_PointList_ListViewItem.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_World.h"

#include <qwidgetstack.h>
#include <qheader.h>


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListView constructor
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
MOS_MTH_PointList_ListView::MOS_MTH_PointList_ListView( ASN1T_ListItineraire& asnListLocalisation, QWidget* pParent, const std::string& strName )
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
    pEditor_ = new MOS_MTH_PointList_Editor( MOS_App::GetApp().GetMainWindow().GetWidgetStackLeft() );
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListView destructor
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
MOS_MTH_PointList_ListView::~MOS_MTH_PointList_ListView()
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
// Name: MOS_MTH_PointList_ListView::SetMission
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_ListView::SetMission( MOS_Mission_ABC* /*pMission*/ )
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListView::SetWidgetTab
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_ListView::SetWidgetTab( QWidget* /*pWidget*/ )
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListView::SetAgent
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_ListView::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListView::WriteMsg
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_ListView::WriteMsg( std::string& sParam )
{
    uint nNbrChilds = childCount();
    sParam += MT_FormatString( "NbChilds: %d | ", childCount() );
    if( nNbrChilds == 0 )
        return;

    ASN1T_Itineraire* pLocalisations = new ASN1T_Itineraire[ nNbrChilds ]; //$$$ RAM

    MOS_MTH_PointList_ListViewItem* pItem = static_cast< MOS_MTH_PointList_ListViewItem* >( firstChild() );
    uint i = 0;
    while( pItem )
    {
        const T_PointVector& locPoints = pItem->GetListPoint();

        assert( !locPoints.empty() );

        pLocalisations[i].type = EnumTypeLocalisation::line;
        pLocalisations[i].vecteur_point.n    = locPoints.size();
        pLocalisations[i].vecteur_point.elem = new ASN1T_CoordUTM[ locPoints.size() ];
        sParam += MT_FormatString( "PointList: %d | ", locPoints.size() );
        
        for( uint j = 0; j < locPoints.size(); ++j )
        {
            std::string strMGRS;
            MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( locPoints[j], strMGRS );
            pLocalisations[i].vecteur_point.elem[j] = strMGRS.c_str();
            sParam += strMGRS;
            sParam += " | ";
        }

        pItem = static_cast< MOS_MTH_PointList_ListViewItem* >( pItem->nextSibling() );
        ++i;
    }

    asnListLocalisation_.n    = nNbrChilds;
    asnListLocalisation_.elem = pLocalisations;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListView::FillRandomParameters
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_ListView::FillRandomParameters()
{
    uint nNbrChilds = rand() % 5;
    if( nNbrChilds == 0 )
        return;

    for ( uint i = 0 ; i < nNbrChilds ; ++i )
    {
        MOS_MTH_PointList_ListViewItem* pNewItem = new MOS_MTH_PointList_ListViewItem( this );
        pNewItem->FillRandomParameters();
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListView::ResaveParameters
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_ListView::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    uint nNbrChilds = childCount();
    archive.WriteField( "sName", std::string( "MOS_MTH_PointList_ListView|nNbrChields" ) );
    archive.WriteField( "nNbrChields", nNbrChilds );

    if( nNbrChilds == 0 )
        return;

    QListViewItem* pItem = this->firstChild();
    
    while( pItem != 0 )
    {
        MOS_MTH_PointList_ListViewItem* pLocal = static_cast< MOS_MTH_PointList_ListViewItem* >( pItem );
        pLocal->ResaveParameters( archive );
        pItem = pItem->nextSibling();
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListView::ReloadParameters
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_ListView::ReloadParameters( MT_InputArchive_ABC& archive )
{
    uint nNbrChilds = 0; //rand() % 5;
// -------------------------------------------------
        std::string sName;
        archive.ReadField( "sName", sName );
        assert( sName == "MOS_MTH_PointList_ListView|nNbrChields" );
// -------------------------------------------------
    archive.ReadField( "nNbrChields", nNbrChilds );
    if( nNbrChilds == 0 )
        return;

    for ( uint i = 0 ; i < nNbrChilds ; ++i )
    {
        MOS_MTH_PointList_ListViewItem* pNewItem = new MOS_MTH_PointList_ListViewItem( this );
        pNewItem->ReloadParameters( archive );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListView::NotifyAgentHasMoved
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_ListView::NotifyAgentHasMoved( MOS_Agent& /*agent*/, const MT_Vector2D& /*vPos*/ )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListView::Initialize
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_ListView::Initialize()
{
    asnListLocalisation_.n = 0;    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListView::Terminate
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_ListView::Terminate()
{
    clear();
    MOS_App::GetApp().GetMainWindow().UnregisterLeftWidget( pEditor_ );
    delete pEditor_;
    pEditor_ = 0;

    if( asnListLocalisation_.n == 0 )
        return;

    for( uint i = 0; i < asnListLocalisation_.n; ++i )
    {
        if( asnListLocalisation_.elem[i].vecteur_point.n > 0 )
            delete [] asnListLocalisation_.elem[i].vecteur_point.elem;
    }

    if( asnListLocalisation_.n > 0 )
        delete [] asnListLocalisation_.elem;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_PointList_ListView::OnContextMenu
// Created: AGN 03-08-26
//-----------------------------------------------------------------------------
void MOS_MTH_PointList_ListView::OnContextMenu( QListViewItem* pItem, const QPoint& pos, int )
{
    QPopupMenu * pMenu=new QPopupMenu(this);
    pMenu->insertItem( "Ajouter liste", eContextMenu_AddList );

    if( pItem != 0 )
    {
        pMenu->insertSeparator();
        pMenu->insertItem( "Editer liste", eContextMenu_EditList );
        pMenu->insertItem( "Effacer liste", eContextMenu_RemoveList );
    }

    int nResult = pMenu->exec( pos );

    if( nResult < 0 )
        return;

    switch( nResult )
    {
    case eContextMenu_AddList:
        {
            MOS_MTH_PointList_ListViewItem* pNewItem = new MOS_MTH_PointList_ListViewItem( this );
            // Edit it
            pEditor_->SetEditedItem( pNewItem );
            MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pEditor_ );

            break;
        }
    case eContextMenu_EditList:
        {
            assert( pItem != 0 );
            assert( pItem->rtti() == MOS_MTH_PointList_ListViewItem::e_rtti );

            MOS_MTH_PointList_ListViewItem* pNewItem = static_cast< MOS_MTH_PointList_ListViewItem* >( pItem );

            pEditor_->SetEditedItem( pNewItem );
            MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pEditor_ );

            break;
        }
    case eContextMenu_RemoveList:
        {
            assert( pItem != 0 );
            assert( pItem->rtti() == MOS_MTH_PointList_ListViewItem::e_rtti );

            takeItem( pItem );
            break;
        }
    }    
}

