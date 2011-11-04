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
#include "ADN_Objects_Data.h"
#include "ADN_Objects_GUI.h"
#include "ADN_Connector_ListView.h"
#include "ADN_Objects_Wizard.h"
#include "ADN_ComboBox.h"
#include "ADN_Tr.h"

typedef ADN_Objects_Data::ObjectInfos ObjectInfos;

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Objects constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Objects::ADN_ListView_Objects( QWidget* pParent, const char* szName, Qt::WFlags f, ADN_ComboBox* pComboListUnitType, ADN_ComboBox* pComboListPropagation )
    : ADN_ListView( pParent, szName, f )
    , pComboListUnitType_   ( pComboListUnitType ) 
    , pComboListPropagation_( pComboListPropagation ) 
{
    // Add one column.
    addColumn( tools::translate( "ADN_ListView_Objects", "Objects" ) );
    setResizeMode( Q3ListView::AllColumns );

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

namespace
{
    typedef ADN_Objects_GUI::E_GuiElements E_GuiElements;

    template < typename T > class BuilderHelper;

    class LinkBuilder : private boost::noncopyable
    {
        template < typename T > friend class BuilderHelper;
    public:
        LinkBuilder( T_ConnectorVector& connectors, ObjectInfos& infos, bool bConnect )
            : connectors_( connectors )
            , infos_     ( infos )
            , bConnect_  ( bConnect )
        {
            // NOTHING
        }

    private:
        template< typename T >
        void Check()
        {
            ObjectInfos::CIT_CapacityMap cit = infos_.capacities_.find( T::TAG );
            if( cit == infos_.capacities_.end() )
                throw std::runtime_error( "unregister object capacity: " + T::TAG );
        }

        template< typename T >
        T& GetConnector()
        {
            return *static_cast< T* >( infos_.capacities_[ T::TAG ].get() );
        }

        template< typename T >
        void Connect( E_GuiElements element )
        {
            connectors_[ element ]->Connect( &GetConnector< T >().bPresent_, bConnect_ );
        }

    private:
        T_ConnectorVector& connectors_;
        ObjectInfos& infos_;
        bool bConnect_;
    };

    template < typename T >
    class BuilderHelper : private boost::noncopyable
    {
    public:
        BuilderHelper( LinkBuilder& builder, E_GuiElements element )
            : builder_( builder )
            , element_( element )
        {
            builder_.Check< T >();
        }

        ~BuilderHelper()
        {
            builder_.Connect< T >( element_ );
        }

        T& GetConnector()
        {
            return builder_.GetConnector< T >();
        }

    private:
        LinkBuilder& builder_;
        E_GuiElements element_;
    };
}

#define BUILDER_HELPER( capacity ) \
    BuilderHelper< ADN_Objects_Data::ADN_CapacityInfos_##capacity > capacity( builder, ADN_Objects_GUI::e##capacity##CapacityPresent )

#define CONNECT_HELPER( capacity, element, member ) \
    vItemConnectors_[ ADN_Objects_GUI::e##capacity##Capacity_##element ]->Connect( &capacity.GetConnector().member, bConnect )

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Objects::ConnectItem
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
void ADN_ListView_Objects::ConnectItem( bool bConnect )
{
    if( pCurData_ == 0 )
        return;

    ObjectInfos* pInfos = reinterpret_cast< ObjectInfos* >( pCurData_ );

    LinkBuilder builder( vItemConnectors_, *pInfos, bConnect );

    vItemConnectors_[ ADN_Objects_GUI::eName ]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ ADN_Objects_GUI::eGeometry ]->Connect( &pInfos->geometries_, bConnect );
    vItemConnectors_[ ADN_Objects_GUI::ePointSize ]->Connect( &pInfos->pointSize_, bConnect );
    vItemConnectors_[ ADN_Objects_GUI::eSymbol ]->Connect( &pInfos->symbol_, bConnect );

    // process capacities
    BUILDER_HELPER( Constructor );
    CONNECT_HELPER( Constructor, DefaultConsumption, nDefaultConsumption_ );
    CONNECT_HELPER( Constructor, UnitType, unitType_ );
    UpdateComboList( pComboListUnitType_, Constructor.GetConnector().unitType_.GetData() );
    ADN_Objects_Data::ADN_CapacityInfos_Buildable& buildable = *Constructor.GetConnector().ptrBuildable_.get();
    vItemConnectors_[ ADN_Objects_GUI::eBuildableCapacity_Dotation ]->Connect( &buildable.categories_, bConnect );
    vItemConnectors_[ ADN_Objects_GUI::eBuildableCapacityPresent ]->Connect( &buildable.bPresent_, bConnect );
    ADN_Objects_Data::ADN_CapacityInfos_Improvable& improvable = *Constructor.GetConnector().ptrImprovable_.get();
    vItemConnectors_[ ADN_Objects_GUI::eImprovableCapacity_Dotation ]->Connect( &improvable.categories_, bConnect );
    vItemConnectors_[ ADN_Objects_GUI::eImprovableCapacityPresent ]->Connect( &improvable.bPresent_, bConnect );

    BUILDER_HELPER( Avoidable);
    CONNECT_HELPER( Avoidable, Distance, rDistance_ );

    BUILDER_HELPER( Bypassable );
    CONNECT_HELPER( Bypassable, Speed, rSpeed_ );

    BUILDER_HELPER( InteractionHeight );
    CONNECT_HELPER( InteractionHeight, Height, height_ );

    BUILDER_HELPER( Mobility );
    CONNECT_HELPER( Mobility, DefaultSpeed, rDefaultSpeed_ );
    CONNECT_HELPER( Mobility, MaxAgentSpeed, rMaxAgentSpeed_ );
    CONNECT_HELPER( Mobility, SpeedModifier, nSpeedModifier_ );

    BUILDER_HELPER( Trafficability );
    CONNECT_HELPER( Trafficability, Limited, limited_ );
    CONNECT_HELPER( Trafficability, MaxWeight, maxWeight_ );

    BUILDER_HELPER( Activable );

    BUILDER_HELPER( Logistic );

    BUILDER_HELPER( SupplyRoute );

    BUILDER_HELPER( TimeLimited );

    BUILDER_HELPER( Bridging );
    CONNECT_HELPER( Bridging, Type, type_ );

    BUILDER_HELPER( Lodging );
    
    BUILDER_HELPER( Workable );
    CONNECT_HELPER( Workable, Size, worker_ );

    BUILDER_HELPER( Attrition );
    CONNECT_HELPER( Attrition, Dotation, ammoCategory_ );
    CONNECT_HELPER( Attrition, UseDotation, useAmmo_ );
    CONNECT_HELPER( Attrition, Surface, attritionSurface_ );
    CONNECT_HELPER( Attrition, Ph, ph_ );

    BUILDER_HELPER( UrbanDestruction );
    CONNECT_HELPER( UrbanDestruction, Data, modifUrbanBlocks_ );

    BUILDER_HELPER( Contamination );
    CONNECT_HELPER( Contamination, MaxToxic, max_toxic_ );

    BUILDER_HELPER( Intoxication );
    CONNECT_HELPER( Intoxication, MaxToxic, max_toxic_ );

    BUILDER_HELPER( Population );
    CONNECT_HELPER( Population, Density, density_ );

    BUILDER_HELPER( Decontamination );

    BUILDER_HELPER( Extinguishable );

    BUILDER_HELPER( Medical );
    CONNECT_HELPER( Medical, NightRate, nightDoctorsRate_ );
    CONNECT_HELPER( Medical, EmergencyDoctorRate, emergencyDoctorsRate_ );
    CONNECT_HELPER( Medical, EmergencyBedRate, emergencyBedsRate_ );

    BUILDER_HELPER( Interference );

    BUILDER_HELPER( InteractWithEnemy );

    BUILDER_HELPER( Occupable );

    BUILDER_HELPER( Supply );
    
    BUILDER_HELPER( Protection );
    CONNECT_HELPER( Protection, MaxSize, max_size_ );
    CONNECT_HELPER( Protection, GeniePrepared, geniePrepared_ );

    BUILDER_HELPER( TerrainHeuristic );
    CONNECT_HELPER( TerrainHeuristic, LocationScore, scores_ );

    BUILDER_HELPER( Propagation );
    CONNECT_HELPER( Propagation, ModelType, model_ );
    UpdateComboList( pComboListPropagation_, Propagation.GetConnector().model_.GetData() );

    BUILDER_HELPER( Detection );
    CONNECT_HELPER( Detection, HasDetectionTime, bDetectTime_ );
    CONNECT_HELPER( Detection, HasIdentificationTime, bIdentTime_ );
    CONNECT_HELPER( Detection, HasRecoTime, bRecoTime_ );
    CONNECT_HELPER( Detection, DetectionTime, detectTime_ );
    CONNECT_HELPER( Detection, IdentificationTime, identTime_ );
    CONNECT_HELPER( Detection, RecoTime, recoTime_ );

    BUILDER_HELPER( Spawn );
    Spawn.GetConnector().Load();
    CONNECT_HELPER( Spawn, ActionRange, rActionRange_ );
    CONNECT_HELPER( Spawn, ObjectType, object_ );
    CONNECT_HELPER( Spawn, NBC, nbc_ );

    BUILDER_HELPER( AttitudeModifier );
    CONNECT_HELPER( AttitudeModifier, Attitude, attitude_ );

    BUILDER_HELPER( Perception );
    CONNECT_HELPER( Perception, Blinded, blinded_ );

    BUILDER_HELPER( Scattering );
    CONNECT_HELPER( Scattering, HumanByTimeStep, humanByTimeStep_ );

    BUILDER_HELPER( Delay );

    BUILDER_HELPER( Structural );
    CONNECT_HELPER( Structural, Value, rStructuralState_ );

    BUILDER_HELPER( FirePropagationModifier );
    CONNECT_HELPER( FirePropagationModifier, Modifiers, modifiers_ );

    BUILDER_HELPER( Burn );

    BUILDER_HELPER( Flood );
    CONNECT_HELPER( Flood, HurtHumans1, nNbHurtHumans1_ );
    CONNECT_HELPER( Flood, HurtHumans2, nNbHurtHumans2_ );
    CONNECT_HELPER( Flood, HurtHumans3, nNbHurtHumans3_ );
    CONNECT_HELPER( Flood, HurtHumansE, nNbHurtHumansE_ );
    CONNECT_HELPER( Flood, DeadHumans, nNbDeadHumans_ );

    BUILDER_HELPER( AltitudeModifier );

    BUILDER_HELPER( Universal );

    BUILDER_HELPER( Stock );

    BUILDER_HELPER( ResourceNetwork );

    BUILDER_HELPER( UndergroundNetwork );

    BUILDER_HELPER( FireForbidden );

    BUILDER_HELPER( Border );

    ADN_Tools::CheckConnectorVector( vItemConnectors_, ADN_Objects_GUI::eNbrGuiElements );
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_ListView::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_ListView_Objects::OnContextMenu( const QPoint& pt )
{
    if( ADN_Workspace::GetWorkspace().GetOpenMode() == eOpenMode_Admin )
    {
        Q3PopupMenu popupMenu( this );
        ADN_Objects_Wizard wizard;
        FillContextMenuWithDefault( popupMenu, wizard );
        popupMenu.exec( pt );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_ListView::UpdateComboList
// Created: MMC 2011-07-19
// -----------------------------------------------------------------------------
void ADN_ListView_Objects::UpdateComboList( ADN_ComboBox* pCombo, const std::string& value )
{
    if ( !pCombo )
        return;

    for ( int i=0; i < pCombo->count(); ++i )
        if ( value == static_cast< std::string >( pCombo->text( i ) ) )
        { 
            pCombo->setCurrentItem( i ); 
            return; 
        }
}