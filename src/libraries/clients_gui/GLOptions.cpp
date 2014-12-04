// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GLOptions.h"
#include "ContourLinesComputer.h"
#include "ContourLinesObserver.h"
#include "Elevation2dTexture.h"
#include "LightingProxy.h"
#include "TerrainSettings.h"
#include "UrbanDisplayOptions.h"
#include "WatershedTexture.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommandPostAttributes_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/FourStateOption.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/ProfilesModel_ABC.h"
#include "clients_kernel/Settings.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/UserProfile_ABC.h"
#include "ENT/ENT_Tr.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>
#include <graphics/Lighting_ABC.h>
#include <graphics/MapnikLayer.h>
#include <xeumeuleu/xml.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GLOptions constructor
// Created: ABR 2014-06-20
// -----------------------------------------------------------------------------
GLOptions::GLOptions( kernel::Controllers& controllers,
                      const kernel::Profile_ABC& profile,
                      const kernel::StaticModel& staticModel,
                      const kernel::EntityResolver_ABC& model,
                      const std::shared_ptr< Lighting_ABC >& lighting,
                      uint32_t mapnikThread,
                      const kernel::ProfilesModel_ABC& profilesModel )
    : controllers_( controllers )
    , profile_( profile )
    , map_( staticModel.detection_ )
    , model_( model )
    , accommodationTypes_( staticModel.accommodationTypes_ )
    , options_( controllers.options_.GetViewOptions() )
    , fires_( FIRE_GROUP_COUNT )
    , selected_( false )
    , superiorSelected_( false )
    , controlled_( false )
    , filterEntity_( controllers )
    , filterProfile_( controllers )
    , lockedEntity_( controllers )
    , contourLinesObserver_( new ContourLinesObserver() )
    , contourLinesComputer_( std::make_shared< ContourLinesComputer >( controllers.controller_, staticModel.detection_ ) )
    , elevation2dTexture_( std::make_shared< Elevation2dTexture >( map_ ) )
    , graphicSetup_( std::make_shared< TerrainSettings >() )
    , lighting_( lighting )
    , profilesModel_( profilesModel )
    , urbanSetup_( std::make_shared< UrbanDisplayOptions >( controllers, accommodationTypes_ ) )
    , watershedTexture_( new WatershedTexture( map_ ) )
    , mapnikThread_( mapnikThread )
{
    contourLinesComputer_->SetContourLinesObserver( contourLinesObserver_ );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: GLOptions constructor
// Created: ABR 2014-06-25
// -----------------------------------------------------------------------------
GLOptions::GLOptions( const GLOptions& other )
    : controllers_( other.controllers_ )
    , profile_( other.profile_ )
    , map_( other.map_ )
    , model_( other.model_ )
    , accommodationTypes_( other.accommodationTypes_ )
    , options_( std::make_shared< kernel::Options >() )
    , elevation2dTexture_( std::make_shared< Elevation2dTexture >( map_ ) )
    , graphicSetup_( std::make_shared< TerrainSettings >() )
    , urbanSetup_( new UrbanDisplayOptions( controllers_, accommodationTypes_ ) )
    , watershedTexture_( new WatershedTexture( map_ ) )
    , filterEntity_( controllers_ )
    , filterProfile_( controllers_ )
    , lockedEntity_( controllers_ )
    , mapnikThread_( other.mapnikThread_ )
    , profilesModel_( other.profilesModel_ )
{
    Copy( other, false );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: GLOptions destructor
// Created: ABR 2014-06-20
// -----------------------------------------------------------------------------
GLOptions::~GLOptions()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: GLOptions::Copy
// Created: ABR 2014-07-24
// -----------------------------------------------------------------------------
void GLOptions::Copy( const GLOptions& other, bool onlyPreferences )
{
    if( !options_ || !other.options_ )
        throw MASA_EXCEPTION( "GLOptions operator= failed, no kernel::Options set." );
    options_->Copy( *other.options_, onlyPreferences );
    fires_ = other.fires_;
    selected_ = other.selected_;
    superiorSelected_ = other.superiorSelected_;
    controlled_ = other.controlled_;
    contourLinesComputer_ = other.contourLinesComputer_;
    lighting_ = other.lighting_->Clone();
    if( !onlyPreferences )
    {
        filterEntity_ = other.filterEntity_;
        filterProfile_ = other.filterProfile_;
        lockedEntity_ = other.lockedEntity_;
        aggregatedEntities_ = other.aggregatedEntities_;
    }
    Load();
}

// -----------------------------------------------------------------------------
// Name: GLOptions::Load
// Created: ABR 2014-08-05
// -----------------------------------------------------------------------------
void GLOptions::Load()
{
    gui::LoadFromOptions( fires_, *options_ );
    elevation2dTexture_->Load( *options_ );
    graphicSetup_->Load( *options_ );
    urbanSetup_->Load( *options_ );
    watershedTexture_->Load( *options_ );
    if( auto lighting = std::dynamic_pointer_cast< LightingProxy >( lighting_ ) )
        lighting->Load( *options_ );
}

void GLOptions::Purge()
{
    elevation2dTexture_->Purge();
    watershedTexture_->Purge();
    mapnikLayer_.reset();
}

namespace
{
    kernel::Entity_ABC* FindEntity( const kernel::EntityResolver_ABC& model,
                                    const kernel::Profile_ABC& profile,
                                    unsigned int id )
    {
        auto entity = model.FindEntity( id );
        return entity && profile.IsVisible( *entity ) ? entity : 0;
    }

    void StringToEntities( std::vector< const kernel::Entity_ABC* >& vector,
                           const kernel::EntityResolver_ABC& model,
                           const kernel::Profile_ABC& profile,
                           const QString& strList )
    {
        auto list = strList.split( ';' );
        for( auto it = list.begin(); it != list.end(); ++it )
            if( auto entity = FindEntity( model, profile, it->toUInt() ) )
                vector.push_back( entity );
    }

    QString EntitiesToString( const std::vector< const kernel::Entity_ABC* >& vector )
    {
        QStringList list;
        std::for_each( vector.begin(),
            vector.end(),
            [&list]( const kernel::Entity_ABC* entity ) {
                if( entity )
                    list << QString::number( entity->GetId() );
        } );
        return list.join( ";" );
    }
}

// -----------------------------------------------------------------------------
// Name: GLOptions::Load
// Created: ABR 2014-07-16
// -----------------------------------------------------------------------------
void GLOptions::Load( kernel::Settings& settings )
{
    settings.beginGroup( "Situation" );
    SetLockedEntity( FindEntity( model_, profile_, settings.value( "LockedEntity", 0u ).toUInt() ) );
    SetFilterEntity( FindEntity( model_, profile_, settings.value( "FilterEntity", 0u ).toUInt() ) );
    aggregatedEntities_.clear();
    SetFilterProfile( profilesModel_.Find( settings.value( "FilterProfile", "" ).toString() ) );
    StringToEntities( aggregatedEntities_, model_, profile_, settings.value( "AggregatedEntities", "" ).toString() );
    settings.endGroup();

    settings.beginGroup( "Display" );
    options_->PurgeDynamicOptions();
    options_->Load( settings );
    settings.endGroup();
    Purge();
    Load();
}

// -----------------------------------------------------------------------------
// Name: GLOptions::Save
// Created: ABR 2014-07-16
// -----------------------------------------------------------------------------
void GLOptions::Save( kernel::Settings& settings )
{
    settings.beginGroup( "Situation" );
    settings.setValue( "LockedEntity", lockedEntity_ ? static_cast< unsigned int >( lockedEntity_->GetId() ) : 0u );
    settings.setValue( "FilterEntity", filterEntity_ ? static_cast< unsigned int >( filterEntity_->GetId() ) : 0u );
    settings.setValue( "FilterProfile", filterProfile_ ? filterProfile_->GetLogin(): "" );
    settings.setValue( "AggregatedEntities", EntitiesToString( aggregatedEntities_ ) );
    settings.endGroup();

    settings.beginGroup( "Display" );
    options_->Save( settings );
    settings.endGroup();
}

// -----------------------------------------------------------------------------
// Name: std::shared_ptr< kernel::Options >& GLOptions::GetOptions
// Created: ABR 2014-07-28
// -----------------------------------------------------------------------------
std::shared_ptr< kernel::Options >& GLOptions::GetOptions()
{
    return options_;
}

// -----------------------------------------------------------------------------
// Name: std::shared_ptr< kernel::Options >& GLOptions::GetOptions
// Created: ABR 2014-10-20
// -----------------------------------------------------------------------------
const std::shared_ptr< kernel::Options >& GLOptions::GetOptions() const
{
    return options_;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::Select
// Created: ABR 2014-06-20
// -----------------------------------------------------------------------------
void GLOptions::Select( bool selected, bool superiorSelected, bool controlled )
{
    selected_ = selected;
    superiorSelected_ = superiorSelected;
    controlled_ = controlled;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::IsDrawingSelection
// Created: ABR 2014-10-16
// -----------------------------------------------------------------------------
bool GLOptions::IsDrawingSelection() const
{
    return selected_;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::ShouldDisplay
// Created: ABR 2014-06-20
// -----------------------------------------------------------------------------
bool GLOptions::ShouldDisplay( const std::string& name ) const
{
    return ShouldDisplay( name, selected_, superiorSelected_, controlled_ );
}

// -----------------------------------------------------------------------------
// Name: GLOptions::ShouldDisplay
// Created: ABR 2014-06-20
// -----------------------------------------------------------------------------
bool GLOptions::ShouldDisplay( const std::string& name, bool b1, bool b2, bool b3 ) const
{
    if( name.empty() )
        throw MASA_EXCEPTION( "Invalid empty option name" );
    return options_->Get( name ).To< kernel::FourStateOption >().IsSet( b1, b2, b3 );
}

// -----------------------------------------------------------------------------
// Name: GLOptions::Get
// Created: ABR 2014-06-25
// -----------------------------------------------------------------------------
const kernel::OptionVariant& GLOptions::Get( const std::string& name ) const
{
    return options_->Get( name );
}

namespace
{
    const std::vector< std::string > watershedOptions = boost::assign::list_of< std::string>( "Watershed/Height" )
                                                                                            ( "Watershed/Color" )
                                                                                            ( "Watershed/Inverse" );
    const std::vector< std::string > hillShadeOptions = boost::assign::list_of< std::string>( "HillShade/Direction" )
                                                                                            ( "HillShade/Enabled" )
                                                                                            ( "HillShade/Strength" );
    const std::vector< std::string > urbanOptions = boost::assign::list_of< std::string>( "Accommodation/Enabled" )
                                                                                        ( "Accommodation/Min" )
                                                                                        ( "Accommodation/Max" )
                                                                                        ( "Accommodation/Displayed" )
                                                                                        ( "Accommodation/Gradient" )
                                                                                        ( "Accommodation/Unoccupied" )
                                                                                        ( "Density/Enabled" )
                                                                                        ( "Density/Gradient" )
                                                                                        ( "Density/Max" )
                                                                                        ( "Density/Min" )
                                                                                        ( "Density/Unoccupied" );
}

// -----------------------------------------------------------------------------
// Name: GLOptions::SetOption
// Created: ABR 2014-06-20
// -----------------------------------------------------------------------------
void GLOptions::Set( const std::string& name,
                     const kernel::OptionVariant& value )
{
    options_->Set( name, value );
    if( std::find( watershedOptions.begin(), watershedOptions.end(), name ) != watershedOptions.end() ||
        name == "Layers/" + ENT_Tr::ConvertFromLayerTypes( eLayerTypes_WeatherComposite, ENT_Tr::eToSim ) + "/Alpha" )
        watershedTexture_->Load( *options_ );
    else if( std::find( hillShadeOptions.begin(), hillShadeOptions.end(), name ) != hillShadeOptions.end() )
        elevation2dTexture_->UpdateHillShadeValues( *options_ );
    else if( std::find( urbanOptions.begin(), urbanOptions.end(), name ) != urbanOptions.end() )
        urbanSetup_->Load( *options_ );
    else if( name.find( "FireRules/" ) == 0 )
        for( int i = 0; i < FIRE_GROUP_COUNT; ++i )
            if( name == FireOption::GetOptionName( static_cast< FireGroup >( i ) ) )
                fires_[i] = gui::LoadFireOptions( value.To< QString >() );
}

// -----------------------------------------------------------------------------
// Name: GLOptions::Remove
// Created: ABR 2014-08-05
// -----------------------------------------------------------------------------
void GLOptions::Remove( const std::string& name )
{
    options_->Remove( name );
}

namespace
{
    std::string GetLevel( const kernel::Entity_ABC& entity )
    {
        const auto knowledge = dynamic_cast< const kernel::AgentKnowledge_ABC* >( &entity );
        const auto commandPost = entity.Retrieve< kernel::CommandPostAttributes_ABC >();
        if( knowledge && knowledge->IsCommandPost() || commandPost && commandPost->IsCommandPost() )
            return "Headquarters";
        return knowledge
            ? knowledge->GetLevel()
            : boost::algorithm::erase_all_copy(
                entity.Get< kernel::TacticalHierarchies >().GetLevel(),
                "levels/" );
    }
}

// -----------------------------------------------------------------------------
// Name: GLOptions::GetRatio
// Created: ABR 2014-06-24
// -----------------------------------------------------------------------------
float GLOptions::GetRatio( const kernel::Entity_ABC& entity ) const
{
    const auto name = "SymbolSize/" + GetLevel( entity );
    return options_->Has( name ) ? options_->Get( name ).To< float >() : 1.f;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::Has
// Created: ABR 2014-06-25
// -----------------------------------------------------------------------------
bool GLOptions::Has( const std::string& name ) const
{
    return options_->Has( name );
}

// -----------------------------------------------------------------------------
// Name: GLOptions::GetFilterEntity
// Created: ABR 2014-06-26
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* GLOptions::GetFilterEntity() const
{
    return filterEntity_;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::SetFilterEntity
// Created: ABR 2014-06-26
// -----------------------------------------------------------------------------
void GLOptions::SetFilterEntity( const kernel::Entity_ABC* filterEntity )
{
    filterEntity_ = filterEntity;
}

const kernel::Profile_ABC* GLOptions::GetFilterProfile() const
{
    return filterProfile_;
}

void GLOptions::SetFilterProfile( const kernel::Profile_ABC* filterProfile )
{
    filterProfile_ = filterProfile;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::GetLockedEntity
// Created: ABR 2014-06-26
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* GLOptions::GetLockedEntity() const
{
    return lockedEntity_;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::SetLockedEntity
// Created: ABR 2014-06-26
// -----------------------------------------------------------------------------
void GLOptions::SetLockedEntity( const kernel::Entity_ABC* lockedEntity )
{
    lockedEntity_ = lockedEntity;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::Aggregate
// Created: ABR 2014-07-08
// -----------------------------------------------------------------------------
void GLOptions::Aggregate( const kernel::Entity_ABC& entity )
{
    if( entity.GetTypeName() == kernel::Automat_ABC::typeName_ ||
        entity.GetTypeName() == kernel::Formation_ABC::typeName_ )
        aggregatedEntities_.push_back( &entity );
}

// -----------------------------------------------------------------------------
// Name: GLOptions::Disaggregate
// Created: ABR 2014-07-08
// -----------------------------------------------------------------------------
void GLOptions::Disaggregate( const kernel::Entity_ABC* entity /* = 0 */ )
{
    if( !entity ) // disagregate all
        aggregatedEntities_.clear();
    else
        aggregatedEntities_.erase( std::remove( aggregatedEntities_.begin(), aggregatedEntities_.end(), entity ), aggregatedEntities_.end() );
}

// -----------------------------------------------------------------------------
// Name: GLOptions::GetAggregatedEntities
// Created: ABR 2014-07-08
// -----------------------------------------------------------------------------
const std::vector< const kernel::Entity_ABC* >& GLOptions::GetAggregatedEntities() const
{
    return aggregatedEntities_;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::IsAggregated
// Created: ABR 2014-07-25
// -----------------------------------------------------------------------------
bool GLOptions::IsAggregated( const kernel::Entity_ABC& entity ) const
{
    return std::find( aggregatedEntities_.begin(), aggregatedEntities_.end(), &entity ) != aggregatedEntities_.end();
}

// -----------------------------------------------------------------------------
// Name: GLOptions::NotifyDeleted
// Created: ABR 2014-07-08
// -----------------------------------------------------------------------------
void GLOptions::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    Disaggregate( &entity );
}

// -----------------------------------------------------------------------------
// Name: GLOptions::GetContourLinesComputer
// Created: ABR 2014-07-02
// -----------------------------------------------------------------------------
std::shared_ptr< ContourLinesComputer >& GLOptions::GetContourLinesComputer()
{
    return contourLinesComputer_;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::GetContourLinesComputer
// Created: ABR 2014-07-03
// -----------------------------------------------------------------------------
const std::shared_ptr< ContourLinesComputer >& GLOptions::GetContourLinesComputer() const
{
    return contourLinesComputer_;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::SetContourLinesComputer
// Created: ABR 2014-07-02
// -----------------------------------------------------------------------------
void GLOptions::SetContourLinesComputer( const std::shared_ptr< ContourLinesComputer >& computer )
{
    contourLinesComputer_ = computer;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::GetWatershedTexture
// Created: ABR 2014-07-02
// -----------------------------------------------------------------------------
WatershedTexture& GLOptions::GetWatershedTexture()
{
    return *watershedTexture_;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::GetElevation2dTexture
// Created: ABR 2014-07-28
// -----------------------------------------------------------------------------
const std::shared_ptr< Elevation2dTexture >& GLOptions::GetElevation2dTexture() const
{
    return elevation2dTexture_;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::GetLighting
// Created: ABR 2014-08-05
// -----------------------------------------------------------------------------
const std::shared_ptr< Lighting_ABC >& GLOptions::GetLighting() const
{
    return lighting_;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::SetLighting
// Created: ABR 2014-07-04
// -----------------------------------------------------------------------------
void GLOptions::SetLighting()
{
    if( lighting_ )
        lighting_->Set();
}

// -----------------------------------------------------------------------------
// Name: GLOptions::GetGraphicSetup
// Created: ABR 2014-07-07
// -----------------------------------------------------------------------------
const std::shared_ptr< TerrainSettings >& GLOptions::GetTerrainSettings() const
{
    return graphicSetup_;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::ComputeUrbanColor
// Created: ABR 2014-07-08
// -----------------------------------------------------------------------------
QColor GLOptions::ComputeUrbanColor( const kernel::UrbanObject_ABC& object ) const
{
    return urbanSetup_->ComputeUrbanColor( object );
}

// -----------------------------------------------------------------------------
// Name: GLOptions::GetUrbanDisplayOptions
// Created: ABR 2014-10-20
// -----------------------------------------------------------------------------
const std::shared_ptr< UrbanDisplayOptions >& GLOptions::GetUrbanDisplayOptions() const
{
    return urbanSetup_;
}

const T_FireOptions& GLOptions::GetFireOptions( FireGroup group ) const
{
    return fires_[group];
}

graphics::MapnikLayer& GLOptions::GetMapnikLayer( const tools::Path& terrain )
{
    if( !mapnikLayer_ )
    {
        MT_LOG_INFO_MSG( "mapnik-threads: " << mapnikThread_ );
        if( terrain.IsEmpty() )
            throw MASA_EXCEPTION( "No terrain path defined" );
        mapnikLayer_.reset( new graphics::MapnikLayer( 0, terrain, mapnikThread_ ) );
    }
    return *mapnikLayer_;
}
