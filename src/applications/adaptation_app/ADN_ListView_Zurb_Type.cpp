//*****************************************************************************
//
// $Created: JDY 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListView_Categories_Size.cpp $
// $Author: Ape $
// $Modtime: 8/04/05 15:45 $
// $Revision: 6 $
// $Workfile: ADN_ListView_Categories_Size.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ListView_Zurb_Type.h"

#include <qpopupmenu.h>

#include "ADN_Connector_ListView_ABC.h"
#include "ADN_Zurb_Data.h"
#include "ADN_Zurb_GUI.h"
#include "ADN_GuiTools.h"

typedef ADN_Zurb_Data::ZurbInfos ZurbInfos;

//-----------------------------------------------------------------------------
// Internal List View Zurb_Material connector to be connected with ADN_ListView_Zurb_Material
//-----------------------------------------------------------------------------
class ADN_CLV_Zurb_Type
    : public ADN_Connector_ListView_ABC
{
public:

    ADN_CLV_Zurb_Type(ADN_ListView_Zurb_Type& list) 
        : ADN_Connector_ListView_ABC(list)
    {}

    virtual ~ADN_CLV_Zurb_Type()
    {}

    ADN_ListViewItem* CreateItem(void * obj)
    {
        // create new list item
        ADN_ListViewItem *pItem                 = new ADN_ListViewItem(&list_,obj,1);

        // connect list item with object's name
        pItem->Connect(0,static_cast<ZurbInfos*>(obj));

        return pItem;
    }

private:
    ADN_CLV_Zurb_Type& operator=( const ADN_CLV_Zurb_Type& );
};


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Zurb_Type constructor
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
ADN_ListView_Zurb_Type::ADN_ListView_Zurb_Type(QWidget * parent, const char * name, WFlags f)
:   ADN_ListView(parent,name,f)
{
    // Add a column && disable sorting
    addColumn( tr( "Sizes" ) );
    setSorting( -1, true );
    setResizeMode( QListView::AllColumns );

    // Connector creation
    pConnector_ = new ADN_CLV_Zurb_Type( *this );

    this->SetDeletionEnabled( true );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Zurb_Type destructor
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
ADN_ListView_Zurb_Type::~ADN_ListView_Zurb_Type()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Zurb_Type::ConnectItem
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
void ADN_ListView_Zurb_Type::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ZurbInfos* pInfos = (ZurbInfos*) pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Zurb_GUI::eNbrZurbGuiElements );

    vItemConnectors_[ADN_Zurb_GUI::eZurbName]->Connect( pInfos, bConnect );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Zurb_Type::OnContextMenu
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
void  ADN_ListView_Zurb_Type::OnContextMenu( const QPoint& pt)
{
    QPopupMenu popuMenu( this );

    popuMenu.insertItem( tr( "New size"), 0 );
    popuMenu.insertItem( tr( "Delete size"), 1 );
    popuMenu.setItemEnabled( 1, pCurData_ != 0 );

    int nResult = popuMenu.exec( pt );
    switch ( nResult )
    {
    case 0:
        {
            // create new sensor & add it to list
            ZurbInfos* pNewInfo = new ZurbInfos();
            pNewInfo->SetDataName( "le nom de la catégorie de volume" );

            ADN_Connector_Vector_ABC* pCList = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
            pCList->AddItem( pNewInfo );

            // Put the  new item at the top of the list (to be coherent with the application)
            int pos = FindNdx( pNewInfo );
            while( pos != 0 )
            {
                pCList->SwapItem( pos - 1, pos );
                --pos;
            }

            // set current item
            setCurrentItem( FindItem( pNewInfo ) );
            break;
        }
    case 1:
        {
            ZurbInfos* pCurSize=(ZurbInfos*)pCurData_;
            if ( pCurSize )
            {
                if( pCurSize->IsMultiRef() && ! ADN_GuiTools::MultiRefWarning() )
                    return;

                static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem(pCurSize);
            }
            break;
        }
    default:
        break;
    }
}