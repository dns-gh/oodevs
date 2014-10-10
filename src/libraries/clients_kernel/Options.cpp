// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Options.h"
#include "DefaultVisualisationScales.h"
#include "FourStateOption.h"
#include "OptionVariant.h"
#include "TristateOption.h"
#include "ENT/ENT_Tr.h"
#include "tools/FileWrapper.h"
#include "tools/GeneralConfig.h"
#include <tools/Path.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Options constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Options::Options()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Options constructor
// Created: ABR 2014-06-25
// -----------------------------------------------------------------------------
Options::Options( const Options& other )
{
    *this = other;
}

// -----------------------------------------------------------------------------
// Name: Options destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Options::~Options()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Options::operator=
// Created: ABR 2014-07-25
// -----------------------------------------------------------------------------
Options& Options::operator=( const Options& other )
{
    options_ = other.options_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: Options::Purge
// Created: ABR 2014-07-17
// -----------------------------------------------------------------------------
void Options::Purge()
{
    options_.clear();
}

// -----------------------------------------------------------------------------
// Name: Options::Apply
// Created: ABR 2014-06-24
// -----------------------------------------------------------------------------
void Options::Apply( const std::function< void ( const std::string&, const OptionVariant&, bool ) >& functor ) const
{
    for( auto it = options_.begin(); it != options_.end(); ++it )
        functor( it->first, it->second.first, it->second.second );
}

// -----------------------------------------------------------------------------
// Name: Options::Set
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Options::Set( const std::string& name, const OptionVariant& value )
{
    options_[ name ].first = value;
}

// -----------------------------------------------------------------------------
// Name: Options::Initialize
// Created: ABR 2014-10-10
// -----------------------------------------------------------------------------
void Options::Set( const std::string& name, const OptionVariant& value, bool isInPreferencePanel )
{
    options_[ name ] = std::make_pair( value, isInPreferencePanel );
}

// -----------------------------------------------------------------------------
// Name: Options::Get
// Created: ABR 2014-06-24
// -----------------------------------------------------------------------------
const OptionVariant& Options::Get( const std::string& name ) const
{
    assert( options_.count( name ) != 0 );
    return options_.at( name ).first;
}

// -----------------------------------------------------------------------------
// Name: Options::Remove
// Created: LGY 2010-09-24
// -----------------------------------------------------------------------------
void Options::Remove( const std::string& name )
{
    options_.erase( name );
}

// -----------------------------------------------------------------------------
// Name: Options::HasOption
// Created: ABR 2014-06-25
// -----------------------------------------------------------------------------
bool Options::Has( const std::string& name ) const
{
    return options_.find( name ) != options_.end();
}

// -----------------------------------------------------------------------------
// Name: Options::Create
// Created: AGE 2008-06-25
// -----------------------------------------------------------------------------
void Options::Create( Settings_ABC& settings, const std::string& name, char type )
{
    if( type == Settings_ABC::intPrefix )
        Set( name, OptionVariant( settings, name, 1 ) );
    else if( type == Settings_ABC::boolPrefix )
        Set( name, OptionVariant( settings, name, false ) );
    else if( type == Settings_ABC::floatPrefix )
        Set( name, OptionVariant( settings, name, 1.f ) );
    else if( type == Settings_ABC::tristatePrefix )
        Set( name, OptionVariant( settings, name, TristateOption::Auto() ) );
    else if( type == Settings_ABC::fourstatePrefix )
        Set( name, OptionVariant( settings, name, FourStateOption::Selected() ) );
    else if( type == Settings_ABC::stringPrefix )
        Set( name, OptionVariant( settings, name, QString( "" ) ) );
}

// -----------------------------------------------------------------------------
// Name: Options::Load
// Created: ABR 2014-07-16
// -----------------------------------------------------------------------------
void Options::Load( Settings_ABC& settings, const std::string& path /*= ""*/ )
{
    const std::string root = QString( path.c_str() ).endsWith( "/" ) ? path : path + "/";
    QStringList list = settings.EntryList( root.c_str() );
    for( auto it = list.begin(); it != list.end(); ++it )
    {
        const std::string typedName = (*it).toStdString();
        if( ! typedName.empty() )
        {
            char type = typedName[0];
            std::string name = root + typedName.substr( 1 );
            name = name[0] == '/' ? name.substr( 1 ) : name;
            Create( settings, name, type );
        }
    }
    list = settings.SubEntriesList( root.c_str() );
    for( auto it = list.begin(); it != list.end(); ++it )
        Load( settings, ( root + (*it).toStdString() ).c_str() );

    // TMP, the following will be removed in few weeks when everyone will have a clean registry.
    // This is due to the fact that the options were added before the xml was filled with the category information,
    // so right now the registry may be dirty with an empty value in that field.
    // Uninstalling and reinstalling will also fix that problem.
    if( Has( "Terrains/Order" ) && !Get( "Terrains/Order" ).To< QString >().isEmpty() )
        return;
    QStringList order;
    tools::Xifstream xisPreferences( "preferences.xml" );
    xisPreferences >> xml::start( "preferences" ) >> xml::start( "terrains" )
                   >> xml::list( "terrain", [&]( xml::xistream& x ) {
                       const auto type = x.attribute< std::string >( "type" );
                       const QString category = x.attribute< QString >( "category" );
                       Set( "Terrains/" + type + "/Category", category, true );
                       if( !category.isEmpty() && !order.contains( category ) )
                           order << category;
                       } );
    Set( "Terrains/Order", order.join( ";" ), true );
}

// -----------------------------------------------------------------------------
// Name: Options::Save
// Created: ABR 2014-07-16
// -----------------------------------------------------------------------------
void Options::Save( Settings_ABC& settings, bool all )
{
    Apply( [&]( const std::string& name, const OptionVariant& option, bool savable ) {
        if( all || savable )
            option.Save( settings, name );
    } );
}

// -----------------------------------------------------------------------------
// Name: Options::ReadGradient
// Created: ABR 2014-08-05
// -----------------------------------------------------------------------------
void Options::ReadGradient( xml::xistream& xis )
{
    QStringList values;
    xis >> xml::list( "color", [&values]( xml::xistream& xisColor ) { 
        values << QString( "%1,%2" ).arg( xisColor.attribute< QString >( "position" ) )
                                    .arg( xisColor.attribute< QString >( "color" ) );
    } );
    Set( "Elevation/Gradients/" + xis.attribute< std::string >( "name" ), values.join( ";" ), true );
}

// -----------------------------------------------------------------------------
// Name: Options::InitializeGeneral
// Created: ABR 2014-07-24
// -----------------------------------------------------------------------------
void Options::InitializeGeneral()
{
    // int
    Set( "BookmarkCoordSystem",  static_cast< int >( eCoordinateSystem_Mgrs ), false );
    Set( "CoordSystem",          static_cast< int >( eCoordinateSystem_Mgrs ), true );
    Set( "RefreshRate",          50,                                           true );

    // replay
    Set( "Replay/SmoothPositions", true, true );

    // color
    Set( "Color/ActiveViewFrame",     QString( "#f00f0f" ), true ); // not used yet
    Set( "Color/Contaminated",        QString( "#00ff00" ), true );
    Set( "Color/Dead",                QString( "#000000" ), true );
    Set( "Color/Healthy",             QString( "#2180ff" ), true );
    Set( "Color/MostlyHealthy",       QString( "#ffff00" ), true );
    Set( "Color/Neutralized",         QString( "#ebe665" ), true );
    Set( "Color/Phantom",             QString( "#3cb45a" ), true );
    Set( "Color/TacticallyDestroyed", QString( "#ebb965" ), true );
    Set( "Color/TotallyDestroyed",    QString( "#eb6a65" ), true );
    Set( "Color/Wounded",             QString( "#ff0000" ), true );

    // sound
    tools::Path soundPath;
    soundPath = soundPath.Absolute( tools::GeneralConfig::BuildResourceChildFile( "sounds" ) );
    Set( "Sound/Directory",                   QString( soundPath.Normalize().ToUTF8().c_str() ), true );
    Set( "Sound/Mute",                        false, true );
    Set( "Sound/Volume/directfire",           50,    true );
    Set( "Sound/Volume/indirectsmoke",        50,    true );
    Set( "Sound/Volume/indirectexplosive",    50,    true );
    Set( "Sound/Volume/indirectillumination", 50,    true );
    Set( "Sound/Volume/indirecteffect",       50,    true );
}

// -----------------------------------------------------------------------------
// Name: Options::InitializeView
// Created: ABR 2014-07-24
// -----------------------------------------------------------------------------
void Options::InitializeView()
{
    // bool
    Set( "3D",                    false, false );
    Set( "DisplayDestroyedUnits", true,  false );
    Set( "FogOfWar",              false, true );
    Set( "Infra",                 true,  true );

    // float
    Set( "GridSize", -1.f, false );

    // int
    Set( "GridType",             static_cast< int >( eCoordinateSystem_Local ), false );
    Set( "ResourceNetworks",     0,                                             false );

    // tri state
    Set( "BigText",   TristateOption::Auto(), false );
    Set( "SmallText", TristateOption::Auto(), false );

    // four state
    Set( "ConvexHulls",          FourStateOption::Selected(), false );
    Set( "DecisionalState",      FourStateOption::Selected(), false );
    Set( "Direction",            FourStateOption::Selected(), false );
    Set( "LogisticLinks",        FourStateOption::Selected(), false );
    Set( "MissingLogisticLinks", FourStateOption::Selected(), false );
    Set( "MissionParameters",    FourStateOption::Selected(), false );
    Set( "OldPaths",             FourStateOption::Selected(), false );
    Set( "Paths",                FourStateOption::Selected(), false );
    Set( "RealTimeLogistic",     FourStateOption::Selected(), false );
    Set( "TacticalLines",        FourStateOption::Selected(), false );
    Set( "UnitDetails",          FourStateOption::On()      , false );
    Set( "VisionCones",          FourStateOption::Selected(), false );
    Set( "VisionLines",          FourStateOption::Selected(), false );
    Set( "VisionSurfaces",       FourStateOption::Selected(), false );
    Set( "WeaponRanges",         FourStateOption::Selected(), false );

    // Accommodation
    Set( "Accommodation/Enabled",    false,                            true );
    Set( "Accommodation/Displayed",  QString( "" ),                    true );
    Set( "Accommodation/Gradient",   QString( "0,#00ff00;1,#ff0000" ), true );
    Set( "Accommodation/Max",        1.f,                              true );
    Set( "Accommodation/Min",        0.f,                              true );
    Set( "Accommodation/Unoccupied", QString( "#000000" ),             true );

    // Contour lines
    Set( "ContourLines/Color",   QString( "#f5f5dc" ), false );
    Set( "ContourLines/Enabled", false,                false );
    Set( "ContourLines/Height",  100,                  false );

    // Density
    Set( "Density/Enabled",    false,                            true );
    Set( "Density/Gradient",   QString( "0,#00ff00;1,#ff0000" ), true );
    Set( "Density/Max",        1.f,                              true );
    Set( "Density/Min",        0.f,                              true );
    Set( "Density/Unoccupied", QString( "#000000" ),             true );

    // Efficient range
    Set( "EfficientRange/Ph",                   50,                   true );
    Set( "EfficientRange/Volume",               0,                    true );
    Set( "EfficientRange/FilterIndirectWeapon", false,                true );
    Set( "EfficientRange/IndirectWeapon",       QString( "" ),        true );
    Set( "EfficientRange/UseCustomColor",       false,                true );
    Set( "EfficientRange/CustomColor",          QString( "#ffffff" ), true );

    // Elevation
    Set( "Elevation/FitToViewPort", false,                true );
    Set( "Elevation/Gradient",      QString( "default" ), true );
    tools::Xifstream xisGradients( "gradients.xml" );
    xisGradients >> xml::start( "gradients" )
                 >> xml::list( "gradient", *this, &Options::ReadGradient );

    // Fires
    // FireRules are empty by default
    Set( "FireIndicators", 0, true ); // FIRE_INDICATORS_DEFAULT // not used yet

    // HillShade
    Set( "HillShade/Enabled",   false, true );
    Set( "HillShade/Direction", 335,   true );
    Set( "HillShade/Strength",  1,     true );

    // lighting
    Set( "Lighting/Type",      0,                    true );
    Set( "Lighting/Direction", QString( "23:23" ),   true );
    Set( "Lighting/Ambient",   QString( "#323232" ), true );
    Set( "Lighting/Diffuse",   QString( "#c8c8c8" ), true );

    // symbol size
    Set( "SymbolSize", 3.f, false ); // move this one under SymbolSize/ when the report will be done
    for( int i = eNatureLevel_c; i < eNbrNatureLevel; ++i )
        Set( "SymbolSize/" + ENT_Tr::ConvertFromNatureLevel( static_cast< E_NatureLevel >( i ) ), 1.f, true ); // not used yet

    // terrain
    QStringList order;
    tools::Xifstream xisPreferences( "preferences.xml" );
    xisPreferences >> xml::start( "preferences" ) >> xml::start( "terrains" )
                   >> xml::list( "terrain", [&]( xml::xistream& x ) {
                       const auto type = x.attribute< std::string >( "type" );
                       const QString category = x.attribute< QString >( "category" );
                       Set( "Terrains/" + type + "/Category", category,                                       true );
                       Set( "Terrains/" + type + "/Name",     x.attribute< QString >( "name", type.c_str() ), true );
                       Set( "Terrains/" + type + "/Shown",    true,                                           true );
                       Set( "Terrains/" + type + "/Width",    x.content< float >( "width", 1.f ),             true );
                       Set( "Terrains/" + type + "/Color",    x.content< QString >( "color", "#000000" ),     true );
                       if( !category.isEmpty() && !order.contains( category ) )
                           order << category;
                       } );
    Set( "Terrains/Order", order.join( ";" ), true );

    // visualisation scales
    for( size_t i = 0; i < DefaultVisualisationScales::size_; ++i )
    {
        const auto& scale = DefaultVisualisationScales::data_[i];
        const auto name = "VisualisationScales/" + ENT_Tr::ConvertFromVisualisationScale( scale.type_ );
        Set( name + "/Min", scale.min_, true );
        Set( name + "/Max", scale.max_, true );
    }

    // watershed
    Set( "Watershed/Color",   QString( "#14a4da" ), false );
    Set( "Watershed/Enabled", false,                false );
    Set( "Watershed/Height",  0,                    false );
    Set( "Watershed/Inverse", false,                false );
}

// -----------------------------------------------------------------------------
// Name: Options::InitializeLayers
// Created: ABR 2014-09-30
// -----------------------------------------------------------------------------
void Options::InitializeLayers( const std::vector< E_LayerTypes >& types )
{
    for( int i = 0; i < static_cast< int >( types.size() ); ++i )
    {
        const auto name = "Layers/" + ENT_Tr::ConvertFromLayerTypes( types[ i ], ENT_Tr::eToSim );
        Set( name + "/Alpha", 1.f, true );
        Set( name + "/Position", i, true );
    }
}
