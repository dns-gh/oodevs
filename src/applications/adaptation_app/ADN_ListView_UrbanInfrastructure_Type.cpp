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
#include "ADN_ListView_UrbanInfrastructure_Type.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Urban_Data.h"
#include "ADN_Objects_Data.h"
#include "ADN_Urban_GUI.h"
#include "ADN_GuiTools.h"

//-----------------------------------------------------------------------------
// Internal List View Urban_Infrastructure connector to be connected with ADN_ListView_Urban_Infrastructure
//-----------------------------------------------------------------------------
class ADN_CLV_UrbanInfrastructure_Type
    : public ADN_Connector_ListView_ABC
{
public:

    ADN_CLV_UrbanInfrastructure_Type(ADN_ListView_UrbanInfrastructure_Type& list)
        : ADN_Connector_ListView_ABC(list)
    {}

    virtual ~ADN_CLV_UrbanInfrastructure_Type()
    {}

    ADN_ListViewItem* CreateItem(void * obj)
    {
        // create new list item
        ADN_ListViewItem *pItem                 = new ADN_ListViewItem( &list_, obj, 1 );

        return pItem;
    }

private:
    ADN_CLV_UrbanInfrastructure_Type& operator=( const ADN_CLV_UrbanInfrastructure_Type& );
};


//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanInfrastructure_Type constructor
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
ADN_ListView_UrbanInfrastructure_Type::ADN_ListView_UrbanInfrastructure_Type(QWidget * parent, const char * name, Qt::WFlags f)
:   ADN_ListView(parent,name,f)
{
    // Add a column && disable sorting
    addColumn( tr( name ) );
    setSorting( -1, true );
    setResizeMode( Q3ListView::AllColumns );

    // Connector creation
    pConnector_ = new ADN_Connector_ListView<ADN_Urban_Data::InfrastructureInfos>(*this);

    this->SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanInfrastructure_Type destructor
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
ADN_ListView_UrbanInfrastructure_Type::~ADN_ListView_UrbanInfrastructure_Type()
{
    delete pConnector_;
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanInfrastructure_Type::ConnectItem
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
void ADN_ListView_UrbanInfrastructure_Type::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ADN_Urban_Data::InfrastructureInfos* pInfos = ( ADN_Urban_Data::InfrastructureInfos* )pCurData_;

    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Urban_GUI::eNbrUrbanInfrastructureGuiElements );

    vItemConnectors_[ADN_Urban_GUI::eUrbanInfrastructureName]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ADN_Urban_GUI::eUrbanInfrastructureSymbol]->Connect( &pInfos->pSymbol_, bConnect );

    ADN_Urban_Data::InfrastructureInfos::CIT_CapacityMap cit = pInfos->capacities_.find( "medical" );
    if( cit == pInfos->capacities_.end() )
        throw std::runtime_error( "unregister object capacity: medical ");

    vItemConnectors_[ADN_Urban_GUI::eMedicalCapacityPresent]->Connect( &pInfos->capacities_[ "medical" ]->bPresent_, bConnect );
    vItemConnectors_[ADN_Urban_GUI::eMedicalCapacity_NightRate]->Connect( &static_cast< ADN_Objects_Data::ADN_CapacityInfos_Medical* >( pInfos->capacities_[ "medical" ].get() )->emergencyBedsRate_, bConnect );
    vItemConnectors_[ADN_Urban_GUI::eMedicalCapacity_EmergencyDoctorRate]->Connect( &static_cast< ADN_Objects_Data::ADN_CapacityInfos_Medical* >( pInfos->capacities_[ "medical" ].get() )->emergencyDoctorsRate_, bConnect );
    vItemConnectors_[ADN_Urban_GUI::eMedicalCapacity_EmergencyBedRate]->Connect( &static_cast< ADN_Objects_Data::ADN_CapacityInfos_Medical* >( pInfos->capacities_[ "medical" ].get() )->nightDoctorsRate_, bConnect );

}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_UrbanInfrastructure_Type::OnContextMenu
// Created: SLG 2010-12-20
//-----------------------------------------------------------------------------
void  ADN_ListView_UrbanInfrastructure_Type::OnContextMenu( const QPoint& pt)
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
             ADN_Urban_Data::InfrastructureInfos* pNewInfo = new ADN_Urban_Data::InfrastructureInfos();

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
            ADN_Urban_Data::InfrastructureInfos* pCurInfrastructure= (ADN_Urban_Data::InfrastructureInfos*) pCurData_;
            if( pCurInfrastructure->IsMultiRef() && ! ADN_GuiTools::MultiRefWarning() )
                return;

            static_cast< ADN_Connector_Vector_ABC* >( pConnector_ )->RemItem( pCurInfrastructure );
            break;
        }
    default:
        break;
    }

}
