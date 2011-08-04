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
#include "ADN_ListView_Urban_Type.h"
#include "ADN_Connector_ListView_ABC.h"
#include "ADN_Urban_Data.h"
#include "ADN_Urban_GUI.h"
#include "ADN_GuiTools.h"

typedef ADN_Urban_Data::UrbanInfos UrbanInfos;

//-----------------------------------------------------------------------------
// Internal List View Urban_Material connector to be connected with ADN_ListView_Urban_Material
//-----------------------------------------------------------------------------
class ADN_CLV_Urban_Type
    : public ADN_Connector_ListView_ABC
{
public:

    ADN_CLV_Urban_Type(ADN_ListView_Urban_Type& list)
        : ADN_Connector_ListView_ABC(list)
    {}

    virtual ~ADN_CLV_Urban_Type()
    {}

    ADN_ListViewItem* CreateItem(void * obj)
    {
        // create new list item
        ADN_ListViewItem *pItem                 = new ADN_ListViewItem(&list_,obj,1);

        // connect list item with object's name
        pItem->Connect(0,static_cast<UrbanInfos*>(obj));

        return pItem;
    }

private:
    ADN_CLV_Urban_Type& operator=( const ADN_CLV_Urban_Type& );
};


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Urban_Type constructor
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
ADN_ListView_Urban_Type::ADN_ListView_Urban_Type(QWidget * parent, const char * name, Qt::WFlags f)
:   ADN_ListView(parent,name,f)
{
    // Add a column && disable sorting
    addColumn( tr( name ) );
    setSorting( -1, true );
    setResizeMode( Q3ListView::AllColumns );

    // Connector creation
    pConnector_ = new ADN_CLV_Urban_Type( *this );

    this->SetDeletionEnabled( true );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Urban_Type destructor
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
ADN_ListView_Urban_Type::~ADN_ListView_Urban_Type()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Urban_Type::ConnectItem
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
void ADN_ListView_Urban_Type::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    UrbanInfos* pInfos = ( UrbanInfos* )pCurData_;

    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Urban_GUI::eNbrUrbanGuiElements );

    vItemConnectors_[ADN_Urban_GUI::eUrbanName]->Connect( pInfos, bConnect );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Urban_Type::OnContextMenu
// Created: SLG 2010-03-10
//-----------------------------------------------------------------------------
void  ADN_ListView_Urban_Type::OnContextMenu( const QPoint& pt)
{
    Q3PopupMenu popuMenu( this );

    popuMenu.insertItem( tr( "New" ), 0 );
    popuMenu.insertItem( tr( "Delete" ), 1 );
    popuMenu.setItemEnabled( 1, pCurData_ != 0 );

    int nResult = popuMenu.exec( pt );
    switch ( nResult )
    {
    case 0:
        {
            // create new sensor & add it to list
            UrbanInfos* pNewInfo = new UrbanInfos();
            pNewInfo->SetDataName( "le nom de la catégorie du type urbain" );

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
            UrbanInfos* pCurSize=(UrbanInfos*)pCurData_;
            if( pCurSize )
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