//*****************************************************************************
// 
// $Created: AGN 03-07-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_DynaObject_ListView.cpp $
// $Author: Nld $
// $Modtime: 21/03/05 16:54 $
// $Revision: 23 $
// $Workfile: MOS_DynaObject_ListView.cpp $
// 
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_DynaObject_ListView.h"
#include "moc_MOS_DynaObject_ListView.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_DynaObject_ListView.inl"
#endif

#include "MOS_DynaObject_ListViewItem.h"
#include "MOS_DynaObject.h"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_WorldWidget.h"
#include "MOS_GLWidget.h"
#include "MOS_DynaObject_Editor.h"
#include "MOS_DynaObject_Creator.h"
#include "MOS_ASN_Messages.h"

#include <qwidgetstack.h>
#include <qheader.h>

//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListView constructor
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
MOS_DynaObject_ListView::MOS_DynaObject_ListView( QWidget* pParentWidget )
: QListView( pParentWidget )
{
    pDynaObjectEditor_  = new MOS_DynaObject_Editor ( MOS_App::GetApp().GetMainWindow().GetWidgetStackLeft() );
    pDynaObjectCreator_ = new MOS_DynaObject_Creator( MOS_App::GetApp().GetMainWindow().GetWidgetStackLeft() );

    setSorting( -1 );
    header()->hide();
    addColumn( "Objets" );
    setRootIsDecorated( true );

    connect( this, SIGNAL( clicked( QListViewItem* ) ), this, SLOT( SlotOnClick( QListViewItem* ) ) );

    // Creates items for each type of objects
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::bouchon_mines, new QListViewItem( this, "Bouchons de mines" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::zone_minee_lineaire, new QListViewItem( this, "Zones minées linéaires" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::zone_minee_par_dispersion, new QListViewItem( this, "Zones minées par dispersion" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::fosse_anti_char, new QListViewItem( this, "Fossés antichar" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::abattis, new QListViewItem( this, "Abattis" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::barricade, new QListViewItem( this, "Barricades" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::eboulement, new QListViewItem( this, "Eboulements" ) ) );;
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::destruction_route, new QListViewItem( this, "Destructions de route" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::destruction_pont, new QListViewItem( this, "Destructions de pont" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::pont_flottant, new QListViewItem( this, "Ponts flottants" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::poste_tir, new QListViewItem( this, "Postes de tir" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::zone_protegee, new QListViewItem( this, "Zones protégées" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::zone_implantation_canon, new QListViewItem( this, "Zones implantation canon" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::zone_implantation_cobra, new QListViewItem( this, "Zones implantation COBRA" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::zone_implantation_lrm, new QListViewItem( this, "Zones implantation LRM" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::site_franchissement, new QListViewItem( this, "Sites de franchissement" ) ) );    
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::nuage_nbc, new QListViewItem( this, "Nuage NBC" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::site_decontamination, new QListViewItem( this, "Site de décontamination" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::plot_ravitaillement, new QListViewItem( this, "Plot de ravitaillement" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::zone_brouillage_brod, new QListViewItem( this, "Zone de brouillage Brod" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::rota , new QListViewItem( this, "ROTA" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::zone_nbc, new QListViewItem( this, "Zone NBC" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::zone_brouillage_bromure, new QListViewItem( this, "Zone de brouillage Bromure" ) ) );
	itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::aire_poser, new QListViewItem( this, "Aire de poser" ) ) );
	itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::piste, new QListViewItem( this, "Piste" ) ) );
	itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::plateforme, new QListViewItem( this, "PlateForme" ) ) );
	itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::zone_mobilite_amelioree, new QListViewItem( this, "Zone à mobilité améliorée" ) ) );
	itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::zone_poser_helicoptere, new QListViewItem( this, "Zone de poser hélicoptere" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::aire_logistique, new QListViewItem( this, "Aire logistique" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::itineraire_logistique, new QListViewItem( this, "Itineraire Logistique" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::camp_prisonniers, new QListViewItem( this, "Camp Prisonniers" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::camp_refugies, new QListViewItem( this, "Camp Refugies" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::poste_controle, new QListViewItem( this, "Poste Controle" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::terrain_largage, new QListViewItem( this, "Terrain largage" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::zone_interdite_tir, new QListViewItem( this, "Zone interdite au tir" ) ) );
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::zone_interdite_mouvement, new QListViewItem( this, "Zone interdite au mouvement" ) ) );   
    itemObjectTypeMap_.insert( std::make_pair( EnumObjectType::zone_implantation_mortier, new QListViewItem( this, "Zone implantation mortier" ) ) );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem *, const QPoint &, int ) ), this, SLOT( SlotContextMenuRequested( QListViewItem*, const QPoint & ) ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListView destructor
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
MOS_DynaObject_ListView::~MOS_DynaObject_ListView()
{
    delete pDynaObjectCreator_;
    delete pDynaObjectEditor_;
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListView::AddObject
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
void MOS_DynaObject_ListView::AddObject( MOS_DynaObject& object )
{
    IT_ObjectId_Item_Map found = itemObjectTypeMap_.find( object.GetType() );
    assert( found != itemObjectTypeMap_.end() && "Unknown object type" );

    new MOS_DynaObject_ListViewItem( found->second, object );
}


//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListView::RemoveObject
// Created: NLD 2003-07-29
//-----------------------------------------------------------------------------
void MOS_DynaObject_ListView::RemoveObject( MOS_DynaObject& object )
{
    if( object.GetViewItem() )
        delete object.GetViewItem();
}



// =============================================================================
// SLOTS
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListView::SlotOnClick
// Created: AGN 03-07-18
//-----------------------------------------------------------------------------
void MOS_DynaObject_ListView::SlotOnClick( QListViewItem* pItem )
{
    if( pItem == 0 )
        return;

    if( pItem->rtti() != MOS_DynaObject_ListViewItem::e_rtti )
        return;

    MOS_DynaObject_ListViewItem* pMyItem = static_cast< MOS_DynaObject_ListViewItem* >( pItem );
    MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().SetWantedCenter( pMyItem->GetObject().GetCenter() );    
}

//-----------------------------------------------------------------------------
// Name: MOS_DynaObject_ListView::SlotContextMenuRequested
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
void MOS_DynaObject_ListView::SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point )
{
    QPopupMenu* pMenu = new QPopupMenu( this );

    if( pItem && pItem->rtti() == MOS_DynaObject_ListViewItem::e_rtti )
    {
        pMenu->insertItem( "Edit object"   , 1 );
        pMenu->insertItem( "Destroy object", 2 );
    }
    else
        pMenu->insertItem( "Create object", 0 );

    int nSelected = pMenu->exec( point );
    delete pMenu;
    switch( nSelected )
    {
        case 0:
            {
                pDynaObjectCreator_->Initialize();
                MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pDynaObjectCreator_ );
                break;
            }
        case 1:
            {
                const MOS_DynaObject& object = static_cast< MOS_DynaObject_ListViewItem* >( pItem )->GetObject();
                pDynaObjectEditor_->SetObject( &object );
                MOS_App::GetApp().GetMainWindow().PushWidgetStackLeft( pDynaObjectEditor_ );
                break;
            }
        case 2:
            {
                const MOS_DynaObject& object = static_cast< MOS_DynaObject_ListViewItem* >( pItem )->GetObject();
                MOS_ASN_MsgObjectMagicAction asnMsg;
                asnMsg.GetAsnMsg().oid_objet = object.GetID();
                asnMsg.GetAsnMsg().action.t  = T_MsgObjectMagicAction_action_destroy_object;
                asnMsg.Send( 546 );
                break;
            }
        default:
            return;
    }
}

