//*****************************************************************************
// 
// $Created: AGN 03-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_ListLocalisation_ListView.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 13 $
// $Workfile: MOS_MTH_ListLocalisation_ListView.cpp $
// 
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_MTH_ListLocalisation_ListView.h"
#include "moc_MOS_MTH_ListLocalisation_ListView.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_MTH_ListLocalisation_ListView.inl"
#endif

#include "MOS_MTH_ListLocalisation_Editor.h"
#include "MOS_MTH_ListLocalisation_ListViewItem.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_World.h"

#include <qwidgetstack.h>
#include <qheader.h>

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListView constructor
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
MOS_MTH_ListLocalisation_ListView::MOS_MTH_ListLocalisation_ListView( ASN1T_ListLocalisation& asnListLocalisation, QWidget* pParent, const std::string& strName )
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
    pEditor_ = new MOS_MTH_ListLocalisation_Editor( MOS_App::GetApp().GetMainWindow().GetWidgetStackLeft() );
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListView constructor
// Created: AGN 03-08-08
//---------------------------------------------²--------------------------------
MOS_MTH_ListLocalisation_ListView::MOS_MTH_ListLocalisation_ListView( ASN1T_ListPolygon& asnListLocalisation, QWidget* pParent, const std::string& strName )
    : QListView           ( pParent, strName.c_str() )
    , MOS_MissionTool_ABC ()
    , asnListLocalisation_( (ASN1T_ListLocalisation&)asnListLocalisation )
    , pAgent_             ( 0 )
    , pEditor_            ( 0 )
{
    this->addColumn( strName.c_str() );

    if( strName.empty() )
        this->header()->hide();

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint &, int ) ), SLOT(OnContextMenu( QListViewItem*, const QPoint&, int )) );
    pEditor_ = new MOS_MTH_ListLocalisation_Editor( MOS_App::GetApp().GetMainWindow().GetWidgetStackLeft() );
}



//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListView destructor
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
MOS_MTH_ListLocalisation_ListView::~MOS_MTH_ListLocalisation_ListView()
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
// Name: MOS_MTH_ListLocalisation_ListView::SetMission
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_ListView::SetMission( MOS_Mission_ABC* /*pMission*/ )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListView::SetWidgetTab
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_ListView::SetWidgetTab( QWidget* /*pWidget*/ )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListView::SetAgent
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_ListView::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListView::WriteMsg
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_ListView::WriteMsg( std::string& sParam )
{
    uint nNbrChilds = childCount();
    sParam += MT_FormatString( "NbListLocalisation:%d | ", nNbrChilds );

    if( nNbrChilds == 0 )
        return;
    
    ASN1T_Localisation* pLocalisations = new ASN1T_Localisation[ nNbrChilds ]; //$$$ RAM

    MOS_MTH_ListLocalisation_ListViewItem* pItem = static_cast< MOS_MTH_ListLocalisation_ListViewItem* >( firstChild() );
    uint i = 0;
    while( pItem )
    {
        const T_PointVector& locPoints = pItem->GetLocalisation();

        assert( !locPoints.empty() );

        pLocalisations[i].type               = pItem->GetType();
        pLocalisations[i].vecteur_point.n    = locPoints.size();
        pLocalisations[i].vecteur_point.elem = new ASN1T_CoordUTM[ locPoints.size() ];
        for( uint j = 0; j < locPoints.size(); ++j )
        {
            std::string strMGRS;
            MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( locPoints[j], strMGRS );
            pLocalisations[i].vecteur_point.elem[j] = strMGRS.c_str();
        }

        pItem = static_cast< MOS_MTH_ListLocalisation_ListViewItem* >( pItem->nextSibling() );
        ++i;
    }

    asnListLocalisation_.n    = nNbrChilds;
    asnListLocalisation_.elem = pLocalisations;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListView::NotifyAgentHasMoved
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_ListView::NotifyAgentHasMoved( MOS_Agent& /*agent*/, const MT_Vector2D& /*vPos*/ )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListView::Initialize
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_ListView::Initialize()
{
    asnListLocalisation_.n = 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListView::Terminate
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_ListView::Terminate()
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
// Name: MOS_MTH_ListLocalisation_ListView::OnContextMenu
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_ListView::OnContextMenu( QListViewItem* pItem, const QPoint& pos, int )
{
    QPopupMenu * pMenu=new QPopupMenu(this);
    pMenu->insertItem( "Ajouter localisation", eContextMenu_AddLocalisation );

    if( pItem != 0 )
    {
        pMenu->insertSeparator();
        pMenu->insertItem( "Editer localisation", eContextMenu_EditLocalisation );
        pMenu->insertItem( "Effacer localisation", eContextMenu_RemoveLocalisation );
    }

    int nResult = pMenu->exec( pos );

    if( nResult < 0 )
        return;

    switch( nResult )
    {
    case eContextMenu_AddLocalisation:
        {
            MOS_MTH_ListLocalisation_ListViewItem* pNewItem = new MOS_MTH_ListLocalisation_ListViewItem( this );
            // Edit it
            pEditor_->SetEditedItem( pNewItem );
            MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pEditor_ );

            break;
        }
    case eContextMenu_EditLocalisation:
        {
            assert( pItem != 0 );
            assert( pItem->rtti() == MOS_MTH_ListLocalisation_ListViewItem::e_rtti );

            MOS_MTH_ListLocalisation_ListViewItem* pNewItem = static_cast< MOS_MTH_ListLocalisation_ListViewItem* >( pItem );

            pEditor_->SetEditedItem( pNewItem );
            MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pEditor_ );

            break;
        }
    case eContextMenu_RemoveLocalisation:
        {
            assert( pItem != 0 );
            assert( pItem->rtti() == MOS_MTH_ListLocalisation_ListViewItem::e_rtti );

            takeItem( pItem );
            break;
        }
    }    
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListView::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_ListView::FillRandomParameters()
{
    uint nNbrChilds = ( rand() % 5 ) + 1;
    assert( nNbrChilds );

    for ( uint i = 0 ; i < nNbrChilds ; ++i )
    {
        MOS_MTH_ListLocalisation_ListViewItem* pNewItem = new MOS_MTH_ListLocalisation_ListViewItem( this );
        pNewItem->FillRandomParameters();
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListView::ResaveParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_ListView::ResaveParameters( MT_OutputArchive_ABC& archive )
{
    uint nNbrChilds = childCount();
    archive.WriteField( "sName", std::string( "MOS_MTH_ListLocalisation_ListView|nNbrChields" ) );
    archive.WriteField( "nNbrChields", nNbrChilds );

    if( nNbrChilds == 0 )
        return;

    QListViewItem* pItem = this->firstChild();
    
    while( pItem != 0 )
    {
        MOS_MTH_ListLocalisation_ListViewItem* pLocal = static_cast< MOS_MTH_ListLocalisation_ListViewItem* >( pItem );
        pLocal->ResaveParameters( archive );
        pItem = pItem->nextSibling();
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ListLocalisation_ListView::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_ListLocalisation_ListView::ReloadParameters( MT_InputArchive_ABC& archive )
{
    uint nNbrChilds = 0; //rand() % 5;
// -------------------------------------------------
        std::string sName;
        archive.ReadField( "sName", sName );
        assert( sName == "MOS_MTH_ListLocalisation_ListView|nNbrChields" );
// -------------------------------------------------
    archive.ReadField( "nNbrChields", nNbrChilds );
    if( nNbrChilds == 0 )
        return;

    for ( uint i = 0 ; i < nNbrChilds ; ++i )
    {
        MOS_MTH_ListLocalisation_ListViewItem* pNewItem = new MOS_MTH_ListLocalisation_ListViewItem( this );
        pNewItem->ReloadParameters( archive );
    }
}