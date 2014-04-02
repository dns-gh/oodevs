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
#include "ADN_ComboBox.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Missions_Data.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Objects_Data_ObjectInfos.h"
#include "ADN_Project_Data.h"
#include "ADN_Tr.h"
#include "ADN_Wizard.h"
#include "ENT/ENT_Tr.h"
#include "protocol/Protocol.h"

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Objects constructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Objects::ADN_ListView_Objects( QWidget* pParent )
    : ADN_ListView( pParent, "ADN_ListView_Objects", ADN_Tr::ConvertFromWorkspaceElement( eObjects ).c_str() )
{
    pConnector_.reset( new ADN_Connector_ListView< ADN_Objects_Data_ObjectInfos >( *this ) );
    SetDeletionEnabled( true );
}

//-----------------------------------------------------------------------------
// Name: ADN_ListView_Objects destructor
// Created: JDY 03-07-03
//-----------------------------------------------------------------------------
ADN_ListView_Objects::~ADN_ListView_Objects()
{
    // NOTHING
}

namespace
{
    typedef ADN_Objects_GUI::E_GuiElements E_GuiElements;

    template < typename T > class BuilderHelper;

    class LinkBuilder : private boost::noncopyable
    {
        template < typename T > friend class BuilderHelper;
    public:
        LinkBuilder( T_ConnectorVector& connectors, ADN_Objects_Data_ObjectInfos& infos, bool bConnect )
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
            auto cit = infos_.capacities_.find( T::TAG );
            if( cit == infos_.capacities_.end() )
                throw MASA_EXCEPTION( "unregister object capacity: " + T::TAG );
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
        ADN_Objects_Data_ObjectInfos& infos_;
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

    ADN_Objects_Data_ObjectInfos* pInfos = reinterpret_cast< ADN_Objects_Data_ObjectInfos* >( pCurData_ );

    LinkBuilder builder( vItemConnectors_, *pInfos, bConnect );

    vItemConnectors_[ ADN_Objects_GUI::eName ]->Connect( &pInfos->strName_, bConnect );
    vItemConnectors_[ ADN_Objects_GUI::eType ]->Connect( &pInfos->strType_, bConnect );
    vItemConnectors_[ ADN_Objects_GUI::eSymbolPolygon ]->Connect( &pInfos->symbols_[ 0 ], bConnect );
    vItemConnectors_[ ADN_Objects_GUI::eSymbolPoint ]->Connect( &pInfos->symbols_[ 1 ], bConnect );
    vItemConnectors_[ ADN_Objects_GUI::eSymbolLine ]->Connect( &pInfos->symbols_[ 2 ], bConnect );
    vItemConnectors_[ ADN_Objects_GUI::eSymbolCircle ]->Connect( &pInfos->symbols_[ 3 ], bConnect );
    vItemConnectors_[ ADN_Objects_GUI::eGeometryPolygon ]->Connect( &pInfos->geometries_[ 0 ], bConnect );
    vItemConnectors_[ ADN_Objects_GUI::eGeometryPoint ]->Connect( &pInfos->geometries_[ 1 ], bConnect );
    vItemConnectors_[ ADN_Objects_GUI::eGeometryLine ]->Connect( &pInfos->geometries_[ 2 ], bConnect );
    vItemConnectors_[ ADN_Objects_GUI::eGeometryCircle ]->Connect( &pInfos->geometries_[ 3 ], bConnect );
    vItemConnectors_[ ADN_Objects_GUI::ePointSize ]->Connect( &pInfos->pointSize_, bConnect );
    vItemConnectors_[ ADN_Objects_GUI::eDescription ]->Connect( &pInfos->description_, bConnect );

    // process capacities
    BUILDER_HELPER( Constructor );
    CONNECT_HELPER( Constructor, DefaultConsumption, nDefaultConsumption_ );
    CONNECT_HELPER( Constructor, UnitType, nUnitType_ );
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
    CONNECT_HELPER( Trafficability, MaxWeight, maxWeight_ );

    BUILDER_HELPER( Activable );

    if( ADN_Workspace::GetWorkspace().IsDevMode() )
    {
        BUILDER_HELPER( Disaster );
        CONNECT_HELPER( Disaster, DisasterType, disaster_ );
    }

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
    CONNECT_HELPER( Attrition, Mine, mineCategory_ );
    CONNECT_HELPER( Attrition, Explosive, explosiveCategory_ );
    CONNECT_HELPER( Attrition, UseDotation, useAmmo_ );
    CONNECT_HELPER( Attrition, UseMine, useMine_ );
    CONNECT_HELPER( Attrition, UseExplosive, useExplo_ );
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

    BUILDER_HELPER( Interference );

    BUILDER_HELPER( InteractWithSide );
    CONNECT_HELPER( InteractWithSide, Friend, bFriendSide_ );
    CONNECT_HELPER( InteractWithSide, Enemy, bEnemySide_ );
    CONNECT_HELPER( InteractWithSide, Neutral, bNeutralSide_ );
    CONNECT_HELPER( InteractWithSide, Civilian, bCivilian_ );

    BUILDER_HELPER( Supply );

    BUILDER_HELPER( Protection );
    CONNECT_HELPER( Protection, MaxSize, max_size_ );
    CONNECT_HELPER( Protection, GeniePrepared, geniePrepared_ );

    BUILDER_HELPER( TerrainHeuristic );
    CONNECT_HELPER( TerrainHeuristic, LocationScore, scores_ );

    BUILDER_HELPER( Propagation );
    CONNECT_HELPER( Propagation, ModelType, nModel_ );

    BUILDER_HELPER( Detection );
    CONNECT_HELPER( Detection, HasDetectionTime, bDetectTime_ );
    CONNECT_HELPER( Detection, HasIdentificationTime, bIdentTime_ );
    CONNECT_HELPER( Detection, HasRecoTime, bRecoTime_ );
    CONNECT_HELPER( Detection, DetectionTime, detectTime_ );
    CONNECT_HELPER( Detection, IdentificationTime, identTime_ );
    CONNECT_HELPER( Detection, RecoTime, recoTime_ );

    BUILDER_HELPER( Spawn );
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

namespace
{
    class ADN_Object_WizardPage : public ADN_WizardPage< ADN_Objects_Data_ObjectInfos >
    {
    public:
        ADN_Object_WizardPage( const T_ItemVector& existingItems, const QString& pageTitle, QWidget* pParent = 0 )
            : ADN_WizardPage( existingItems, pageTitle, pParent )
        {
            // NOTHING
        }
        virtual ~ADN_Object_WizardPage()
        {
            // NOTHING
        }

        ADN_Objects_Data_ObjectInfos* CreateObject()
        {
            ADN_Objects_Data_ObjectInfos* object = ADN_WizardPage::CreateObject();
            if( object )
            {
                if( !object->strType_.GetData().empty() )
                    ADN_Workspace::GetWorkspace().GetProject().addedObjects_[ object->strName_.GetData() ] = object->strType_.GetData();
                object->strType_ = ADN_Objects_Data_ObjectInfos::GenerateNextType();
            }
            return object;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: ADN_Automata_ListView::OnContextMenu
// Created: APE 2005-01-10
// -----------------------------------------------------------------------------
void ADN_ListView_Objects::OnContextMenu( const QPoint& pt )
{
    Q3PopupMenu popupMenu( this );
    ADN_Wizard< ADN_Objects_Data_ObjectInfos, ADN_Object_WizardPage > wizard( ADN_Tr::ConvertFromWorkspaceElement( eObjects ).c_str(),
                                                                                ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos(), this );
    FillContextMenuWithDefault( popupMenu, wizard );
    if( pCurData_ != 0 )
    {
        ADN_Objects_Data_ObjectInfos* pCastData = static_cast< ADN_Objects_Data_ObjectInfos* >( pCurData_ );
        assert( pCastData != 0 );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                        ADN_Tr::ConvertFromWorkspaceElement( eSensors ).c_str(),
                                        ADN_Workspace::GetWorkspace().GetSensors().GetData().GetSensorsThatUse( *pCastData ), eSensors );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                        ADN_Tr::ConvertFromWorkspaceElement( eResources ).c_str(),
                                        ADN_Workspace::GetWorkspace().GetResources().GetData().GetResourcesThatUse( *pCastData ), eResources, sword::category_ammunition );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                        ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(),
                                        ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ), eEquipments );
        FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(),
                                        ADN_Tr::ConvertFromWorkspaceElement( eObjects ).c_str(),
                                        ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectsThatUse( *pCastData ), eObjects );

        for( int type = 0; type < eNbrMissionType; ++type )
            FillContextMenuWithUsersList( popupMenu, pCastData->strName_.GetData().c_str(), ENT_Tr::ConvertFromMissionType( static_cast< E_MissionType >( type ) ).c_str(),
                                            ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissionsThatUse( static_cast< E_MissionType >( type ), *pCastData ), eMissions, type );
    }
    popupMenu.exec( pt );
}

// -----------------------------------------------------------------------------
// Name: ADN_ListView_Objects::GetToolTipFor
// Created: ABR 2012-07-25
// -----------------------------------------------------------------------------
std::string ADN_ListView_Objects::GetToolTipFor( const QModelIndex& index )
{
    if( !index.isValid() )
        return "";
    void* pData = static_cast< ADN_ListViewItem* >( dataModel_.GetItemFromIndex( index ) )->GetData();
    ADN_Objects_Data_ObjectInfos* pCastData = (ADN_Objects_Data_ObjectInfos*)pData;
    assert( pCastData != 0 );

    std::string result;
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eSensors ).c_str(),
                        ADN_Workspace::GetWorkspace().GetSensors().GetData().GetSensorsThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eResources ).c_str(),
                        ADN_Workspace::GetWorkspace().GetResources().GetData().GetResourcesThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eEquipments ).c_str(),
                        ADN_Workspace::GetWorkspace().GetEquipments().GetData().GetEquipmentsThatUse( *pCastData ), result );
    FillMultiUsersList( ADN_Tr::ConvertFromWorkspaceElement( eObjects ).c_str(),
                        ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectsThatUse( *pCastData ), result );

    for( int type = 0; type < eNbrMissionType; ++type )
        FillMultiUsersList( ENT_Tr::ConvertFromMissionType( static_cast< E_MissionType >( type ) ).c_str(),
                            ADN_Workspace::GetWorkspace().GetMissions().GetData().GetMissionsThatUse( static_cast< E_MissionType >( type ), *pCastData ), result );

    if( result.empty() )
        result = tools::translate( "ADN_ListView_Objects", "<b>Unused</b>" ).toStdString();
    return result;
}
