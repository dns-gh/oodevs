// *****************************************************************************
//
// $Created: NLD 2003-12-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_MedicalPriorities.cpp $
// $Author: Nld $
// $Modtime: 4/02/05 9:29 $
// $Revision: 2 $
// $Workfile: MOS_MTH_MedicalPriorities.cpp $
//
// *****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_MTH_MedicalPriorities.h"
#include "moc_MOS_MTH_MedicalPriorities.cpp"

#include "MOS_App.h"

#include <qlistview.h>
#include <qpopupmenu.h>

// -----------------------------------------------------------------------------
// Name: MOS_MTH_MedicalPriorities constructor
// Created: NLD 2003-12-03
// -----------------------------------------------------------------------------
MOS_MTH_MedicalPriorities::MOS_MTH_MedicalPriorities( ASN1T_SantePriorites& asnMedicalPriorities, QWidget* pParent, const std::string& strName )
    : QWidget                   ( pParent )
    , MOS_MissionTool_ABC       ()
    , pAsnMedicalPriorities_( &asnMedicalPriorities )
{
    QVBoxLayout* pLocalLayout = new QVBoxLayout( this );

    // Title
    QLabel* pLabel = CreateTitle( this, strName );
    pLocalLayout->addWidget( pLabel );

    pTypeListView_ = new QListView( this, "" );
    pTypeListView_->addColumn( "Blessures" );
    pTypeListView_->setSorting( -1 );
    pLocalLayout->addWidget( pTypeListView_ );
    
    pTypesMenu_ = new QPopupMenu( this );

    pTypesMenu_->insertItem( "U1" );
    pTypesMenu_->insertItem( "U2" );
    pTypesMenu_->insertItem( "U3" );
    pTypesMenu_->insertItem( "UE" );
    
    connect( pTypeListView_, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint &, int ) ), SLOT(OnContextMenu( QListViewItem*, const QPoint&, int )) );
}

// -----------------------------------------------------------------------------
// Name: MOS_MTH_MedicalPriorities destructor
// Created: NLD 2003-12-03
// -----------------------------------------------------------------------------
MOS_MTH_MedicalPriorities::~MOS_MTH_MedicalPriorities()
{

}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MedicalPriorities::OnContextMenu
// Created: AGN 03-08-08
//-----------------------------------------------------------------------------
void MOS_MTH_MedicalPriorities::OnContextMenu( QListViewItem* pItem, const QPoint& pos, int )
{
    QPopupMenu* pMenu = new QPopupMenu(this);
    pMenu->insertItem( "Ajouter blessure", pTypesMenu_ );

    if( pItem != 0 )
        pMenu->insertItem( "Effacer blessure", 0 );

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
// Name: MOS_MTH_MedicalPriorities::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_MedicalPriorities::Initialize()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MedicalPriorities::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_MTH_MedicalPriorities::Terminate()
{
    if( pAsnMedicalPriorities_->n == 0 )
        return;

    delete [] pAsnMedicalPriorities_->elem;
}

// -----------------------------------------------------------------------------
// Name: GetWoundID
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
ASN1T_EnumHumanWound GetWoundID( const std::string& strWound )
{
    if( strWound == "U1" ) return EnumHumanWound::blesse_urgence_1;
    if( strWound == "U2" ) return EnumHumanWound::blesse_urgence_2;
    if( strWound == "U3" ) return EnumHumanWound::blesse_urgence_3;
    //if( strWound == "UE" ) return 5;
    return EnumHumanWound::blesse_urgence_extreme;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MedicalPriorities::WriteMsg
// Created: NLD 2003-03-04
//-----------------------------------------------------------------------------
void MOS_MTH_MedicalPriorities::WriteMsg(  std::string& sParam )
{
    assert( pAsnMedicalPriorities_ );

    pAsnMedicalPriorities_->n = pTypeListView_->childCount();
    if( !pAsnMedicalPriorities_->n )
        return;

    ASN1T_EnumHumanWound* pPriorities = new ASN1T_EnumHumanWound[ pAsnMedicalPriorities_->n ]; //$$$ RAM

    QListViewItem* pItem = pTypeListView_->firstChild();
    uint i = 0;
    while( pItem )
    {
        pPriorities[i] = GetWoundID( (const char*)pItem->text( 0 ) );
        ++i;
        pItem = pItem->nextSibling();
    }

    pAsnMedicalPriorities_->elem = pPriorities;
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MedicalPriorities::FillRandomParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_MedicalPriorities::FillRandomParameters()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_MedicalPriorities::ResaveParameters
// Created: FBD 03-09-08
//-----------------------------------------------------------------------------
void MOS_MTH_MedicalPriorities::ResaveParameters( MT_OutputArchive_ABC& archive )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MedicalPriorities::ReloadParameters
// Created: FBD 03-08-01
//-----------------------------------------------------------------------------
void MOS_MTH_MedicalPriorities::ReloadParameters( MT_InputArchive_ABC& archive )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MedicalPriorities::SetAgent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_MTH_MedicalPriorities::SetAgent( MOS_Agent* /*pAgent*/ )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MedicalPriorities::NotifyAgentHasMoved
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
void MOS_MTH_MedicalPriorities::NotifyAgentHasMoved( MOS_Agent& /*agent*/, const MT_Vector2D& /*vPos*/ )
{

}

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MedicalPriorities::SetMission
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
void MOS_MTH_MedicalPriorities::SetMission( MOS_Mission_ABC* /*pMission*/ )
{
}   

//-----------------------------------------------------------------------------
// Name: MOS_MTH_MedicalPriorities::SetWidgetTab
// Created: FBD 03-01-20
//-----------------------------------------------------------------------------
void MOS_MTH_MedicalPriorities::SetWidgetTab( QWidget* /*pWidget*/ )
{
}


