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
#include "ADN_ListView_Categories_Size.h"
#include "ADN_Connector_ListView_ABC.h"
#include "ADN_Categories_Data.h"
#include "ADN_Categories_GUI.h"
#include "ADN_GuiTools.h"

typedef ADN_Categories_Data::SizeInfos SizeInfos;


//-----------------------------------------------------------------------------
// Internal List View Categories_Size connector to be connected with ADN_ListView_Categories_Size
//-----------------------------------------------------------------------------
class ADN_CLV_Categories_Size
: public ADN_Connector_ListView_ABC
{
public:

    ADN_CLV_Categories_Size(ADN_ListView_Categories_Size& list)
    : ADN_Connector_ListView_ABC(list)
    {}

    virtual ~ADN_CLV_Categories_Size()
    {}

    ADN_ListViewItem* CreateItem(void * obj)
    {
        // create new list item
        ADN_ListViewItem *pItem                 = new ADN_ListViewItem(&list_,obj,1);

        // connect list item with object's name
        pItem->Connect(0,static_cast<SizeInfos*>(obj));

        return pItem;
    }

private:
    ADN_CLV_Categories_Size& operator=( const ADN_CLV_Categories_Size& );
};


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Size constructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_ListView_Categories_Size::ADN_ListView_Categories_Size(QWidget * parent, const char * name, Qt::WFlags f)
:   ADN_ListView(parent,name,f)
{
    // Add a column && disable sorting
    addColumn( tr( "Sizes" ) );
    setSorting( -1, true );
    setResizeMode( Q3ListView::AllColumns );

    // Connector creation
    pConnector_ = new ADN_CLV_Categories_Size( *this );

    this->SetDeletionEnabled( true, false );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Size destructor
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
ADN_ListView_Categories_Size::~ADN_ListView_Categories_Size()
{
    delete pConnector_;
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Size::ConnectItem
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void ADN_ListView_Categories_Size::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    SizeInfos* pInfos = (SizeInfos*) pCurData_;
    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Categories_GUI::eNbrSizeGuiElements );

    vItemConnectors_[ADN_Categories_GUI::eSizeName]->Connect( pInfos, bConnect );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Categories_Size::OnContextMenu
// Created: JDY 03-08-27
//-----------------------------------------------------------------------------
void  ADN_ListView_Categories_Size::OnContextMenu( const QPoint& pt)
{
    Q3PopupMenu popuMenu( this );

    popuMenu.insertItem( tr( "New size"), 0 );
    popuMenu.insertItem( tr( "Delete size"), 1 );
    popuMenu.setItemEnabled( 1, pCurData_ != 0 );

    int nResult = popuMenu.exec( pt );
    switch ( nResult )
    {
        case 0:
        {
            // create new sensor & add it to list
            SizeInfos* pNewInfo = new SizeInfos();
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
            SizeInfos* pCurSize=(SizeInfos*)pCurData_;
            if( pCurSize )
            {
                if( pCurSize->IsMultiRef() && bDeletionWarning_ && ! ADN_GuiTools::MultiRefWarning() )
                    return;

                static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem(pCurSize);
            }
            break;
        }
        default:
            break;
    }
}