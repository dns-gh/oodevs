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
void Options::Set( const std::string& name, const OptionVariant& value, bool savable )
{
    options_[ name ] = std::make_pair( value, savable );
}

// -----------------------------------------------------------------------------
// Name: Options::Get
// Created: ABR 2014-06-24
// -----------------------------------------------------------------------------
const OptionVariant& Options::Get( const std::string& name ) const
{
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
        const std::string typedName = (*it).toAscii().constData();
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
        Load( settings, ( root + (*it).toAscii().constData() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: Options::Save
// Created: ABR 2014-07-16
// -----------------------------------------------------------------------------
void Options::Save( Settings_ABC& settings, bool all )
{
    Apply( [&settings, &all]( const std::string& name, const OptionVariant& option, bool savable ) {
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
        values << QString( "%1,%2" ).arg( xisColor.attribute< std::string >( "position" ).c_str() )
                                    .arg( xisColor.attribute< std::string >( "color" ).c_str() );
    } );
    Set( "Elevation/Gradients/" + xis.attribute< std::string >( "name" ), values.join( ";" ) );
}

// -----------------------------------------------------------------------------
// Name: Options::InitializeGeneral
// Created: ABR 2014-07-24
// -----------------------------------------------------------------------------
void Options::InitializeGeneral()
{
    // int
    Set( "CoordSystem",          static_cast< int >( eCoordinateSystem_Mgrs ), true );
    Set( "RefreshRate",          50,                        true );

    // replay
    Set( "Replay/SmoothPositions", true, true );

    // color
    Set( "Color/ActiveViewFrame",      QString( "#f00f0f" ), true );
    Set( "Color/MissingLogisticLinks", QString( "#ffff00" ), true );
    Set( "Color/Neutralized",          QString( "#ebe665" ), true );
    Set( "Color/Phantom",              QString( "#3cb45a" ), true );
    Set( "Color/TacticallyDestroyed",  QString( "#ebb965" ), true );
    Set( "Color/TotallyDestroyed",     QString( "#eb6a65" ), true );

    Set( "Color/Healthy",       QString( "#2180ff" ), true );
    Set( "Color/Dead",          QString( "#000000" ), true );
    Set( "Color/Contaminated",  QString( "#00ff00" ), true );
    Set( "Color/Wounded",       QString( "#ff0000" ), true );
    Set( "Color/MostlyHealthy", QString( "#ffff00" ), true );

    // sound
    tools::Path soundPath;
    soundPath = soundPath.Absolute( tools::GeneralConfig::BuildResourceChildFile( "sounds" ) );
    Set( "Sound/Directory", QString( soundPath.Normalize().ToUTF8().c_str() ), true );
    Set( "Sound/Volume/directfire", 50, true );
    Set( "Sound/Volume/indirectsmoke", 50, true );
    Set( "Sound/Volume/indirectexplosive", 50, true );
    Set( "Sound/Volume/indirectillumination", 50, true );
    Set( "Sound/Volume/indirecteffect", 50, true );
}

// -----------------------------------------------------------------------------
// Name: Options::InitializeView
// Created: ABR 2014-07-24
// -----------------------------------------------------------------------------
void Options::InitializeView()
{
    // bool
    Set( "3D",       false, true );
    Set( "FogOfWar", false, true );
    Set( "Infra",    true,  true );

    // float
    Set( "GridSize", -1.f, true );

    // int
    Set( "GridType",             static_cast< int >( eCoordinateSystem_Local ), true );
    Set( "ResourceNetworks",     0,                                             true );

    // tri state
    Set( "BigText",   TristateOption::Auto(), true );
    Set( "SmallText", TristateOption::Auto(), true );

    // four state
    Set( "ConvexHulls",          FourStateOption::Selected(), true );
    Set( "DebugPoints",          FourStateOption::Selected(), true );
    Set( "DecisionalState",      FourStateOption::Selected(), true );
    Set( "Direction",            FourStateOption::Selected(), true );
    Set( "LogisticLinks",        FourStateOption::Selected(), true );
    Set( "MissingLogisticLinks", FourStateOption::Selected(), true );
    Set( "MissionParameters",    FourStateOption::Selected(), true );
    Set( "OldPaths",             FourStateOption::Selected(), true );
    Set( "Paths",                FourStateOption::Selected(), true );
    Set( "RealTimeLogistic",     FourStateOption::Selected(), true );
    Set( "TacticalLines",        FourStateOption::Selected(), true );
    Set( "VisionCones",          FourStateOption::Selected(), true );
    Set( "VisionLines",          FourStateOption::Selected(), true );
    Set( "VisionSurfaces",       FourStateOption::Selected(), true );
    Set( "WeaponRanges",         FourStateOption::Selected(), true );

    // Accommodation
    Set( "Accommodation/Color",      false,                            true );
    Set( "Accommodation/Displayed",  QString( "" ),                    true );
    Set( "Accommodation/Gradient",   QString( "0,#00ff00;1,#ff0000" ), true );
    Set( "Accommodation/Max",        1.f,                              true );
    Set( "Accommodation/Min",        0.f,                              true );
    Set( "Accommodation/Unoccupied", QString( "#000000" ),             true );

    // Contour lines
    Set( "ContourLines/Color",   QString( "#f5f5dc" ), true );
    Set( "ContourLines/Enabled", false,                true );
    Set( "ContourLines/Height",  100,                  true );

    // Density
    Set( "Density/Color",      false,                            true );
    Set( "Density/Gradient",   QString( "0,#00ff00;1,#ff0000" ), true );
    Set( "Density/Max",        1.f,                              true );
    Set( "Density/Min",        0.f,                              true );
    Set( "Density/Unoccupied", QString( "#000000" ),             true );

    // Efficient range
    Set( "EfficientRange/Ph",     50,                        true );
    Set( "EfficientRange/Volume", 0,                         true );
    Set( "EfficientRange/FilterIndirectWeapon", false, true );
    Set( "EfficientRange/IndirectWeapon", QString( "" ), true );
    Set( "EfficientRange/UseCustomColor", false, true );
    Set( "EfficientRange/CustomColor", QString( "#ffffff" ), true );

    // Elevation
    Set( "Elevation/FitToViewPort", false, true );
    Set( "Elevation/Gradient", QString( "default" ), true );
    xml::xifstream xisGradients( "gradients.xml" );
    xisGradients >> xml::start( "gradients" )
                 >> xml::list( "gradient", *this, &Options::ReadGradient );

    // Fires
    // FireRules are empty by default
    Set( "FireIndicators", 0, true ); // FIRE_INDICATORS_DEFAULT

    // HillShade
    Set( "HillShade/Enabled",    false, true );
    Set( "HillShade/Direction", 335,    true );
    Set( "HillShade/Strength",  1,      true );

    // lighting
    Set( "Lighting/Type",      0, true );
    Set( "Lighting/Direction", QString( "23:23" ), true );
    Set( "Lighting/Ambient",   QString( "#323232" ), true );
    Set( "Lighting/Diffuse",   QString( "#c8c8c8" ), true );

    // layers
/*    for( int i = 0; i < eNbrRegistryLayer; ++i )
    {
        const auto name = "Layers/" + ENT_Tr::ConvertFromRegistryLayer( static_cast< E_RegistryLayer >( i ) );
        Set( name + "/Alpha", 1.f, true );
        Set( name + "/Position", i, true );
    }
    Set( "Layers/" + ENT_Tr::ConvertFromRegistryLayer( eRegistryLayer_VisionSurfaces ) + "/Alpha", 0.5f, true );
    Set( "Layers/" + ENT_Tr::ConvertFromRegistryLayer( eRegistryLayer_Logo ) + "/Alpha", 0.7f, true );*/

    // symbol size
    Set( "SymbolSize", 3.f, true );

    for( int i = eNatureLevel_c; i < eNbrNatureLevel; ++i )
        Set( "SymbolSize/" + ENT_Tr::ConvertFromNatureLevel( static_cast< E_NatureLevel >( i ) ), 1.f, true );

    // terrain
    QStringList order;
    xml::xifstream xisPreferences( "preferences.xml" );
    xisPreferences >> xml::start( "preferences" ) >> xml::start( "terrains" )
                   >> xml::list( "terrain", [&]( xml::xistream& x ) {
                       const auto type = x.attribute< std::string >( "type" );
                       //const QString category = x.attribute< std::string >( "category" ).c_str();
                       //Set( "Terrains/" + type + "/Category", category, true );
                       Set( "Terrains/" + type + "/Name", QString( x.attribute< std::string >( "name", type ).c_str() ), true );
                       Set( "Terrains/" + type + "/Shown", true );
                       Set( "Terrains/" + type + "/Width", x.content< float >( "width", 1.f ) );
                       Set( "Terrains/" + type + "/Color", QString( x.content< std::string >( "color", "#000000" ).c_str() ) );
                       //if( !order.contains( category ) )
                       //    order << category;
                       } );
    Set( "Terrains/Order", order.join( ";" ) );

    // visualisation scales
    for( int i = 0; i < DefaultVisualisationScales::size_; ++i )
    {
        const auto& scale = DefaultVisualisationScales::data_[i];
        const auto name = "VisualisationScales/" + ENT_Tr::ConvertFromVisualisationScale( scale.type_ );
        Set( name + "/min", scale.min_, true );
        Set( name + "/max", scale.max_, true );
    }

    // watershed
    Set( "Watershed/Color",   QString( "#14a4da" ), true );
    Set( "Watershed/Enabled", false,                true );
    Set( "Watershed/Height",  0,                    true );
    Set( "Watershed/Inverse", false,                true );
}
