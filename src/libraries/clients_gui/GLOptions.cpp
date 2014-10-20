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
#include "TerrainSettings.h"
#include "UrbanDisplayOptions.h"
#include "WatershedTexture.h"

#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/FourStateOption.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Settings.h"
#include "clients_kernel/StaticModel.h"
#include "ENT/ENT_Tr.h"

#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>
#include <graphics/Lighting_ABC.h>
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
                      const std::shared_ptr< Lighting_ABC >& lighting )
    : controllers_( controllers )
    , profile_( profile )
    , map_( staticModel.detection_ )
    , model_( model )
    , accommodationTypes_( staticModel.accommodationTypes_ )
    , options_( controllers.options_.GetViewOptions() )
    //, fires_( FIRE_GROUP_COUNT )
    , selected_( false )
    , superiorSelected_( false )
    , controlled_( false )
    , filterEntity_( controllers )
    , lockedEntity_( controllers )
    , contourLinesObserver_( new ContourLinesObserver() )
    , contourLinesComputer_( std::make_shared< ContourLinesComputer >( controllers.controller_, staticModel.detection_ ) )
    , elevation2dTexture_( std::make_shared< Elevation2dTexture >( map_ ) )
    , graphicSetup_( std::make_shared< TerrainSettings >() )
    , lighting_( lighting )
    , urbanSetup_( new UrbanDisplayOptions( controllers, accommodationTypes_ ) )
    , watershedTexture_( new WatershedTexture( map_ ) )
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
    , lockedEntity_( controllers_ )
{
    *this = other;
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
// Name: GLOptions::operator=
// Created: ABR 2014-07-24
// -----------------------------------------------------------------------------
GLOptions& GLOptions::operator=( const GLOptions& other )
{
    if( !options_ || !other.options_ )
        throw std::exception( "GLOptions operator= failed, no kernel::Options set." );
    //fires_ = other.fires_;
    *options_ = *other.options_;
    selected_ = other.selected_;
    superiorSelected_ = other.superiorSelected_;
    controlled_ = other.controlled_;
    filterEntity_ = other.filterEntity_;
    lockedEntity_ = other.lockedEntity_;
    aggregatedAutomats_ = other.aggregatedAutomats_;
    aggregatedFormations_ = other.aggregatedFormations_;
    contourLinesComputer_ = other.contourLinesComputer_;
    lighting_ = other.lighting_->Clone();
    Load();
    return *this;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::Load
// Created: ABR 2014-08-05
// -----------------------------------------------------------------------------
void GLOptions::Load()
{
    //gui::LoadFromOptions( fires_, *options_ );
    elevation2dTexture_->Load( *options_ );
    graphicSetup_->Load( *options_ );
    //urbanSetup_->Load( *options_ );
    watershedTexture_->Load( *options_ );
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
    StringToEntities( aggregatedAutomats_, model_, profile_, settings.value( "AggregatedAutomats", "" ).toString() );
    StringToEntities( aggregatedFormations_, model_, profile_, settings.value( "AggregatedFormations", "" ).toString() );
    settings.endGroup();

    settings.beginGroup( "Display" );
    options_->Purge();
    options_->Load( settings );
    settings.endGroup();
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
    settings.setValue( "AggregatedAutomats", EntitiesToString( aggregatedAutomats_ ) );
    settings.setValue( "AggregatedFormations", EntitiesToString( aggregatedFormations_ ) );
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
    const std::vector< std::string > watershedOptions =boost::assign::list_of< std::string>( "Watershed/Height" )
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
    //void LoadFromXml( T_FireOptions& dst, const kernel::OptionVariant& option )
    //{
    //    xml::xistringstream xis( option.To< QString >().toStdString() );
    //    LoadFromXml( dst, xis );
    //}
}

// -----------------------------------------------------------------------------
// Name: GLOptions::SetOption
// Created: ABR 2014-06-20
// -----------------------------------------------------------------------------
void GLOptions::Set( const std::string& name,
                     const kernel::OptionVariant& value )
{
    //options_->Set( name, value ); // will be needed when multi view will be present
    if( std::find( watershedOptions.begin(), watershedOptions.end(), name ) != watershedOptions.end() ||
        name == "Layers/" + ENT_Tr::ConvertFromLayerTypes( eLayerTypes_WeatherComposite, ENT_Tr::eToSim ) + "/Alpha" )
        watershedTexture_->Load( *options_ );
    else if( std::find( hillShadeOptions.begin(), hillShadeOptions.end(), name ) != hillShadeOptions.end() )
        elevation2dTexture_->UpdateHillShadeValues( *options_ );
    //else if( std::find( urbanOptions.begin(), urbanOptions.end(), name ) != urbanOptions.end() )
    //    urbanSetup_->Load( *options_ );
    else if( name == "ContourLines/Height" )
        contourLinesComputer_->SetHeight( value.To< int >() );
    //else if( name.find( "FireRules/" ) == 0 )
    //{
    //    for( int i = 0; i < FIRE_GROUP_COUNT; ++i )
    //        if( name == FireOption::GetOptionName( static_cast< FireGroup >( i ) ) )
    //            ::LoadFromXml( fires_[i], value );
    //}
}

// -----------------------------------------------------------------------------
// Name: GLOptions::Remove
// Created: ABR 2014-08-05
// -----------------------------------------------------------------------------
void GLOptions::Remove( const std::string& name )
{
    options_->Remove( name );
}

// -----------------------------------------------------------------------------
// Name: GLOptions::GetRatio
// Created: ABR 2014-06-24
// -----------------------------------------------------------------------------
float GLOptions::GetRatio( const std::string& level ) const
{
    const auto name = "SymbolSize/" + boost::algorithm::erase_all_copy( level, "levels/" );
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
    if( entity.GetTypeName() == kernel::Automat_ABC::typeName_ )
        aggregatedAutomats_.push_back( &entity );
    else if( entity.GetTypeName() == kernel::Formation_ABC::typeName_ )
        aggregatedFormations_.push_back( &entity );
}

// -----------------------------------------------------------------------------
// Name: GLOptions::Disaggregate
// Created: ABR 2014-07-08
// -----------------------------------------------------------------------------
void GLOptions::Disaggregate( const kernel::Entity_ABC* entity /* = 0 */ )
{
    if( !entity )
    {
        aggregatedAutomats_.clear();
        aggregatedFormations_.clear();
        return;
    }
    aggregatedAutomats_.erase( std::remove( aggregatedAutomats_.begin(), aggregatedAutomats_.end(), entity ), aggregatedAutomats_.end() );
    aggregatedFormations_.erase( std::remove( aggregatedFormations_.begin(), aggregatedFormations_.end(), entity ), aggregatedFormations_.end() );
}

// -----------------------------------------------------------------------------
// Name: GLOptions::GetAggregatedAutomats
// Created: ABR 2014-07-08
// -----------------------------------------------------------------------------
const std::vector< const kernel::Entity_ABC* >& GLOptions::GetAggregatedAutomats() const
{
    return aggregatedAutomats_;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::GetAggregatedFormations
// Created: ABR 2014-07-08
// -----------------------------------------------------------------------------
const std::vector< const kernel::Entity_ABC* >& GLOptions::GetAggregatedFormations() const
{
    return aggregatedFormations_;
}

// -----------------------------------------------------------------------------
// Name: GLOptions::IsAggregated
// Created: ABR 2014-07-25
// -----------------------------------------------------------------------------
bool GLOptions::IsAggregated( const kernel::Entity_ABC& entity ) const
{
    return std::find( aggregatedAutomats_.begin(), aggregatedAutomats_.end(), &entity ) != aggregatedAutomats_.end() ||
           std::find( aggregatedFormations_.begin(), aggregatedFormations_.end(), &entity ) != aggregatedFormations_.end();
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
// Name: std::shared_ptr< Lighting_ABC >& GLOptions::GetLighting
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
QColor GLOptions::ComputeUrbanColor( const kernel::UrbanObject_ABC& /*object*/ ) const
{
    return QColor();// TODO: urbanSetup_->ComputeUrbanColor( object );
}

// -----------------------------------------------------------------------------
// Name: GLOptions::Save
// Created: BAX 2014-06-25
// -----------------------------------------------------------------------------
//const T_FireOptions& GLOptions::GetFireOptions( FireGroup group ) const
//{
//    return fires_[group];
//}
