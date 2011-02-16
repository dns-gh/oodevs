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
#include "ADN_ListView_UrbanAccommodation_Type.h"

#include <qpopupmenu.h>

#include "ADN_Connector_ListView.h"
#include "ADN_Urban_Data.h"
#include "ADN_Urban_GUI.h"
#include "ADN_GuiTools.h"

//-----------------------------------------------------------------------------
// Internal List View Urban_Accommodation connector to be connected with ADN_ListView_Urban_Accommodation
//-----------------------------------------------------------------------------
class ADN_CLV_UrbanAccommodation_Type
    : public ADN_Connector_ListView_ABC
{
public:

    ADN_CLV_UrbanAccommodation_Type(ADN_ListView_UrbanAccommodation_Type& list)
        : ADN_Connector_ListView_ABC(list)
    {}

    virtual ~ADN_CLV_UrbanAccommodation_Type()
    {}

    ADN_ListViewItem* CreateItem(void * obj)
    {
        // create new list item
        ADN_ListViewItem *pItem                 = new ADN_ListViewItem( &list_, obj, 1 );

        return pItem;
    }

private:
    ADN_CLV_UrbanAccommodation_Type& operator=( const ADN_CLV_UrbanAccommodation_Type& );
};


//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanAccommodation_Type constructor
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
ADN_ListView_UrbanAccommodation_Type::ADN_ListView_UrbanAccommodation_Type(QWidget * parent, const char * name, WFlags f)
:   ADN_ListView(parent,name,f)
{
    // Add a column && disable sorting
    addColumn( tr( name ) );
    setSorting( -1, true );
    setResizeMode( QListView::AllColumns );

    // Connector creation
    pConnector_ = new ADN_Connector_ListView<ADN_Urban_Data::AccommodationInfos>(*this);

    this->SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanAccommodation_Type destructor
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
ADN_ListView_UrbanAccommodation_Type::~ADN_ListView_UrbanAccommodation_Type()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanAccommodation_Type::ConnectItem
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
void ADN_ListView_UrbanAccommodation_Type::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ADN_Urban_Data::AccommodationInfos* pInfos = ( ADN_Urban_Data::AccommodationInfos* )pCurData_;

    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Urban_GUI::eNbrUrbanAccommodationGuiElements );

    vItemConnectors_[ADN_Urban_GUI::eUrbanAccommodationName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Urban_GUI::eUrbanAccommodationCapacity]->Connect( &pInfos->value_, bConnect );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanAccommodation_Type::OnContextMenu
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
void  ADN_ListView_UrbanAccommodation_Type::OnContextMenu( const QPoint& pt)
{
    QPopupMenu popuMenu( this );

    popuMenu.insertItem( tr( "New" ), 0 );
    popuMenu.insertItem( tr( "Delete" ), 1 );
    popuMenu.setItemEnabled( 1, pCurData_ != 0 );

    int nResult = popuMenu.exec( pt );
    switch ( nResult )
    {
    case 0:
        {
            // create new sensor & add it to list
             ADN_Urban_Data::AccommodationInfos* pNewInfo = new ADN_Urban_Data::AccommodationInfos();

            ADN_Connector_Vector_ABC* pCList = static_cast< ADN_Connector_Vector_ABC* >( pConnector_ );
            pCList->AddItem( pNewInfo );

            // Put the  new item at the top of the list (to be coherent with the application)
            int pos= FindNdx( pNewInfo );
            while( pos != 0 )
            {
                static_cast<ADN_Connector_Vector_ABC*>(&GetConnector())->SwapItem( pos - 1, pos );
                --pos;
            }

            // set current item
            setCurrentItem( FindItem( pNewInfo ) );
            break;
        }
    case 1:
        {
            ADN_Urban_Data::AccommodationInfos* pCurAccommodation= (ADN_Urban_Data::AccommodationInfos*) pCurData_;
            if( pCurAccommodation->IsMultiRef() && ! ADN_GuiTools::MultiRefWarning() )
                return;

            static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurAccommodation );
            break;
        }
    default:
        break;
    }

}
