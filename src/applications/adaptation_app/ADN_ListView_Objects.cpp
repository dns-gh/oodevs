//*****************************************************************************
//
// $Created: JDY 03-07-03 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_ListView_Objects.cpp $
// $Author: Nld $
// $Modtime: 29/04/05 16:39 $
// $Revision: 11 $
// $Workfile: ADN_ListView_Objects.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_ListView_Objects.h"

#include <qpopupmenu.h>

#include "ADN_Objects_Data.h"
#include "ADN_Objects_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_ObjectCreator_ABC.h"


typedef ADN_Objects_Data::ObjectInfos ObjectInfos;


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Objects constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Objects::ADN_ListView_Objects( QWidget* pParent, const char* szName, WFlags f )
    : ADN_ListView( pParent, szName, f )
{
    // Add one column.
    addColumn( tr( "Objects" ) );
    setResizeMode( QListView::AllColumns );
    
    // Connector creation
    pConnector_ = new ADN_Connector_ListView< ObjectInfos >( *this );
}


//-----------------------------------------------------------------------------
// Name: ADN_ListView_Objects destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Objects::~ADN_ListView_Objects()
{
    delete pConnector_; 
}


/*namespace
{	
	static int INDEX=0;
	template<typename T1, typename T2> struct GuiConnector
	{
		static void Connect(T_ConnectorVector& connectors, ObjectInfos* pInfos, bool bConnect)
		{
			T2& temp = *static_cast<T2*>(pInfos->capacities_[T2::DISPLAY_NAME]);
			connectors[INDEX]->Connect( &temp.bPresent_, bConnect );
			++INDEX;
			// forward
			T1::Connect(connectors, pInfos, bConnect);
		}
	};
	template<typename T2> struct GuiConnector<ADN_Objects_Data::NullType, T2>
	{
		static void Connect(T_ConnectorVector& connectors, ObjectInfos* pInfos, bool bConnect)
		{
			T2& temp = *static_cast<T2*>(pInfos->capacities_[T2::DISPLAY_NAME]);
			connectors[INDEX]->Connect( &temp.bPresent_, bConnect );
			INDEX=0;
		}
	};
}*/

namespace
{
    class LinkBuilder
    {
        typedef ADN_Objects_GUI::E_GuiElements E_GuiElements;
        
    public:
        LinkBuilder( T_ConnectorVector& connectors, ObjectInfos& infos, bool bConnect ) 
            : connectors_ ( connectors )
            , infos_ ( infos )
            , bConnect_ ( bConnect )
        {}
        
        template< typename T >
        T& Link( E_GuiElements element )
        {
            ObjectInfos::CIT_CapacityMap cit = infos_.capacities_.find( T::TAG );
            if ( cit == infos_.capacities_.end() )
                throw std::runtime_error( "unregister object capacity: " + T::TAG );
            T& connector = *static_cast< T* >( infos_.capacities_[ T::TAG ].get() );
            connectors_[ element ]->Connect( &connector.bPresent_, bConnect_ );
            return connector;
        }

    private:
        LinkBuilder& operator=( const LinkBuilder& );
        T_ConnectorVector& connectors_;
        ObjectInfos& infos_;
        bool bConnect_;
    };
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Objects::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListView_Objects::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ObjectInfos* pInfos = (ObjectInfos*)pCurData_;
    
    LinkBuilder builder( vItemConnectors_, *pInfos, bConnect );

    vItemConnectors_[ ADN_Objects_GUI::eName ]->Connect( &pInfos->strName_, bConnect );
	
    
	/*typedef boost::mpl::fold<   ADN_Objects_Data::ObjectInfos::ADN_CapacityTypes, 
								ADN_Objects_Data::NullType, 
								GuiConnector<boost::mpl::_1, boost::mpl::_2 > >::type ConnectorType;
	ConnectorType::Connect(capacitiesConnectors_, pInfos, bConnect);*/
	// process capacities

	ADN_Objects_Data::ADN_CapacityInfos_Constructor& constructor = builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Constructor >( ADN_Objects_GUI::eConstructorCapacityPresent );
    {
        vItemConnectors_[ADN_Objects_GUI::eConstructorCapacity_DefaultConsumption]->Connect( &constructor.nDefaultConsumption_, bConnect );
        vItemConnectors_[ADN_Objects_GUI::eConstructorCapacity_UnitType]->Connect( &constructor.unitType_, bConnect );
	
        ADN_Objects_Data::ADN_CapacityInfos_Buildable& buildable = *static_cast< ADN_Objects_Data::ADN_CapacityInfos_Buildable* >( constructor.ptrBuildable_.get() );
        vItemConnectors_[ADN_Objects_GUI::eBuildableCapacityPresent]->Connect( &buildable.bPresent_, bConnect );
        vItemConnectors_[ADN_Objects_GUI::eBuildableCapacity_Dotation]->Connect( &buildable.categories_, bConnect );
        
        ADN_Objects_Data::ADN_CapacityInfos_Improvable& improvable = *static_cast< ADN_Objects_Data::ADN_CapacityInfos_Improvable* >( constructor.ptrImprovable_.get() );
        vItemConnectors_[ADN_Objects_GUI::eImprovableCapacityPresent]->Connect( &improvable.bPresent_, bConnect );
        vItemConnectors_[ADN_Objects_GUI::eImprovableCapacity_Dotation]->Connect( &improvable.categories_, bConnect );
    }
//    ADN_Objects_Data::ADN_CapacityInfos_Buildable& buildable = builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Buildable >( ADN_Objects_GUI::eBuildableCapacityPresent );    	    
//    vItemConnectors_[ADN_Objects_GUI::eBuildableCapacity_Consumption]->Connect( &buildable.nDefaultConsumption_, bConnect );
//    vItemConnectors_[ADN_Objects_GUI::eBuildableCapacity_DotationPresent]->Connect( &buildable.hasCategory_, bConnect );
//    vItemConnectors_[ADN_Objects_GUI::eBuildableCapacity_Dotation]->Connect( &buildable.category_, bConnect );
//
//	ADN_Objects_Data::ADN_CapacityInfos_Minable& minable = builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Minable >( ADN_Objects_GUI::eMinableCapacityPresent );    	    
//    vItemConnectors_[ADN_Objects_GUI::eMinableCapacity_Consumption]->Connect( &minable.nDefaultConsumption_, bConnect );
//    vItemConnectors_[ADN_Objects_GUI::eMinableCapacity_DotationPresent]->Connect( &minable.hasCategory_, bConnect );
//    vItemConnectors_[ADN_Objects_GUI::eMinableCapacity_Dotation]->Connect( &minable.category_, bConnect );

	ADN_Objects_Data::ADN_CapacityInfos_Avoidable& avoidable = builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Avoidable >( ADN_Objects_GUI::eAvoidableCapacityPresent );    
    vItemConnectors_[ADN_Objects_GUI::eAvoidableCapacity_Distance]->Connect( &avoidable.rDistance_, bConnect );
    
    ADN_Objects_Data::ADN_CapacityInfos_Bypassable& bypassable = builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Bypassable >( ADN_Objects_GUI::eBypassableCapacityPresent );
    vItemConnectors_[ADN_Objects_GUI::eBypassableCapacity_Speed]->Connect( &bypassable.rSpeed_, bConnect );
    
    ADN_Objects_Data::ADN_CapacityInfos_Mobility& mobility = builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Mobility >( ADN_Objects_GUI::eMobilityCapacityPresent );
    vItemConnectors_[ADN_Objects_GUI::eMobilityCapacity_DefaultSpeed]->Connect( &mobility.rDefaultSpeed_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eMobilityCapacity_SpeedModifier]->Connect( &mobility.nSpeedModifier_, bConnect );
    vItemConnectors_[ADN_Objects_GUI::eMobilityCapacity_MaxAgentSpeed]->Connect( &mobility.rMaxAgentSpeed_, bConnect );

    builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Activable >( ADN_Objects_GUI::eActivableCapacityPresent );
    builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Logistic >( ADN_Objects_GUI::eLogisticCapacityPresent );
//    builder.Link< ADN_Objects_Data::ADN_CapacityInfos_SupplyRoute >( ADN_Objects_GUI::eSupplyRouteCapacityPresent );
    builder.Link< ADN_Objects_Data::ADN_CapacityInfos_TimeLimited >( ADN_Objects_GUI::eTimeLimitedCapacityPresent );
    builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Bridging >( ADN_Objects_GUI::eBridgingCapacityPresent );
    
    ADN_Objects_Data::ADN_CapacityInfos_Workable& workable = builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Workable >( ADN_Objects_GUI::eWorkableCapacityPresent );
    vItemConnectors_[ADN_Objects_GUI::eWorkableCapacity_Size]->Connect( &workable.worker_, bConnect );

    builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Attrition >( ADN_Objects_GUI::eAttritionCapacityPresent );
    
    ADN_Objects_Data::ADN_CapacityInfos_Contamination& contamination = builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Contamination >( ADN_Objects_GUI::eContaminationCapacityPresent );
    vItemConnectors_[ADN_Objects_GUI::eContaminationCapacity_MaxToxic]->Connect( &contamination.max_toxic_, bConnect );

    ADN_Objects_Data::ADN_CapacityInfos_Intoxication& intoxication = builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Intoxication >( ADN_Objects_GUI::eIntoxicationCapacityPresent );
    vItemConnectors_[ADN_Objects_GUI::eIntoxicationCapacity_MaxToxic]->Connect( &intoxication.max_toxic_, bConnect );

    builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Decontamination >( ADN_Objects_GUI::eDecontaminationCapacityPresent );
    builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Detection >( ADN_Objects_GUI::eDetectionCapacityPresent );
    builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Extinguishable >( ADN_Objects_GUI::eExtinguishableCapacityPresent );
    builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Healable >( ADN_Objects_GUI::eHealableCapacityPresent );
    builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Interference >( ADN_Objects_GUI::eInterferenceCapacityPresent );
    builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Occupable >( ADN_Objects_GUI::eOccupableCapacityPresent );
    builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Supply >( ADN_Objects_GUI::eSupplyCapacityPresent );
    
    ADN_Objects_Data::ADN_CapacityInfos_Protection& protection = builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Protection >( ADN_Objects_GUI::eProtectionCapacityPresent );
    vItemConnectors_[ ADN_Objects_GUI::eProtectionCapacity_MaxSize ]->Connect( &protection.max_size_, bConnect );
    vItemConnectors_[ ADN_Objects_GUI::eProtectionCapacity_GeniePrepared ]->Connect( &protection.geniePrepared_, bConnect );
        
    ADN_Objects_Data::ADN_CapacityInfos_TerrainHeuristic& heuristic = builder.Link< ADN_Objects_Data::ADN_CapacityInfos_TerrainHeuristic >( ADN_Objects_GUI::eTerrainHeuristicCapacityPresent );
    vItemConnectors_[ ADN_Objects_GUI::eTerrainHeuristicCapacity_LocationScore ]->Connect( &heuristic.scores_, bConnect );

    ADN_Objects_Data::ADN_CapacityInfos_Propagation& propagation = builder.Link< ADN_Objects_Data::ADN_CapacityInfos_Propagation >( ADN_Objects_GUI::ePropagationCapacityPresent );
    vItemConnectors_[ ADN_Objects_GUI::ePropagationCapacity_ModelType ]->Connect( &propagation.model_, bConnect );
    
	ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Objects_GUI::eNbrGuiElements );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_ListView::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_ListView_Objects::OnContextMenu( const QPoint& pt )
{
    QPopupMenu          popupMenu( this );
    ADN_DefaultObjectCreator< ADN_Objects_Data::ObjectInfos >   wizard;

    FillContextMenuWithDefault( popupMenu, wizard );
    popupMenu.exec( pt );
}