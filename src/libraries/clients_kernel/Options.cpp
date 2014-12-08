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
#include "MT_Tools/MT_Logger.h"
#include <boost/assign.hpp>
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
    Copy( other, false );
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
// Name: Options::Copy
// Created: ABR 2014-07-25
// -----------------------------------------------------------------------------
void Options::Copy( const Options& other, bool onlyPreferences )
{
    if( onlyPreferences )
        other.Apply( [&]( const std::string& name, const OptionVariant& value, bool isPreference )
        {
            if( isPreference )
                Set( name, value );
        } );
    else
        options_ = other.options_;
}

namespace
{
    // Temporary solution to ignore old registry entries: we list the keys where
    // the user can create options dynamically, so we can ignore others.
    // The boolean value below is the 'isPreference' parameter.
    const std::map< std::string, bool > allowedDynamicKeys = boost::assign::map_list_of( "/Elevation/Gradients/", true );
}

void Options::PurgeDynamicOptions()
{
    for( auto it = allowedDynamicKeys.begin(); it != allowedDynamicKeys.end(); ++it )
        for( auto itOption = options_.begin(); itOption != options_.end(); )
            if( itOption->first.find( "allowedDynamicKeys" ) == 0 )
            {
                auto copy = itOption++;
                options_.erase( copy );
            }
            else
                ++itOption;
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
    auto it = options_.find( name );
    if( it == options_.end() )
        throw MASA_EXCEPTION( "Option not initialized: " + name );
    it->second.first = value;
}

// -----------------------------------------------------------------------------
// Name: Options::Create
// Created: ABR 2014-10-10
// -----------------------------------------------------------------------------
void Options::Create( const std::string& name, const OptionVariant& value, bool isPreference )
{
    auto it = options_.find( name );
    if( it != options_.end() )
        throw MASA_EXCEPTION( "Option already initialized: " + name );
    options_[ name ] = std::make_pair( value, isPreference );
}

// -----------------------------------------------------------------------------
// Name: Options::Get
// Created: ABR 2014-06-24
// -----------------------------------------------------------------------------
const OptionVariant& Options::Get( const std::string& name ) const
{
    auto it = options_.find( name );
    if( it == options_.end() )
        throw MASA_EXCEPTION( "Option not initialized: " + name );
    return it->second.first;
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
// Name: Options::InternalCreate
// Created: AGE 2008-06-25
// -----------------------------------------------------------------------------
OptionVariant Options::CreateVariant( Settings_ABC& settings, const std::string& name, char type )
{
    if( type == Settings_ABC::intPrefix )
        return OptionVariant( settings, name, 1 );
    else if( type == Settings_ABC::boolPrefix )
        return OptionVariant( settings, name, false );
    else if( type == Settings_ABC::floatPrefix )
        return OptionVariant( settings, name, 1.f );
    else if( type == Settings_ABC::tristatePrefix )
        return OptionVariant( settings, name, TristateOption::Auto() );
    else if( type == Settings_ABC::fourstatePrefix )
        return OptionVariant( settings, name, FourStateOption::Selected() );
    else if( type == Settings_ABC::stringPrefix )
        return OptionVariant( settings, name, QString( "" ) );
    throw MASA_EXCEPTION( std::string( "Invalid option type: " ) + type );
}

// -----------------------------------------------------------------------------
// Name: Options::Load
// Created: ABR 2014-07-16
// -----------------------------------------------------------------------------
void Options::Load( Settings_ABC& settings, const std::string& path /*= ""*/ )
{
    const auto root = QString( path.c_str() ).endsWith( "/" ) ? path : path + "/";
    auto itDynamicKey = allowedDynamicKeys.begin();
    for( ; itDynamicKey != allowedDynamicKeys.end(); ++itDynamicKey )
        if( root.find( itDynamicKey->first ) == 0 )
            break;
    const auto entries = settings.EntryList( root.c_str() );
    for( auto it = entries.begin(); it != entries.end(); ++it )
    {
        const std::string typedName = (*it).toStdString();
        if( !typedName.empty() )
        {
            char type = typedName[0];
            std::string name = root + typedName.substr( 1 );
            name = name[0] == '/' ? name.substr( 1 ) : name;
            try
            {
                auto variant = CreateVariant( settings, name, type );
                if( !Has( name ) )
                {
                    // At this point, if we dont have 'name' already, it's either an
                    // old registry entry, or a user-created option.
                    if( itDynamicKey != allowedDynamicKeys.end() )
                        Create( name, variant, itDynamicKey->second );
                    else
                        MT_LOG_WARNING_MSG( "Deprecated option '" + name + "'" );
                }
                else
                    Set( name, variant );
            }
            catch( std::exception& e )
            {
                MT_LOG_WARNING_MSG( "Failed to load option '" + name + "': " + tools::GetExceptionMsg( e ) );
            }
        }
    }
    const auto subEntries = settings.SubEntriesList( root.c_str() );
    for( auto it = subEntries.begin(); it != subEntries.end(); ++it )
        Load( settings, ( root + (*it).toStdString() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: Options::Save
// Created: ABR 2014-07-16
// -----------------------------------------------------------------------------
void Options::Save( Settings_ABC& settings )
{
    Apply( [&]( const std::string& name, const OptionVariant& option, bool ) {
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
    Create( "Elevation/Gradients/" + xis.attribute< std::string >( "name" ), values.join( ";" ), true );
}

// -----------------------------------------------------------------------------
// Name: Options::InitializeGeneral
// Created: ABR 2014-07-24
// -----------------------------------------------------------------------------
void Options::InitializeGeneral()
{
    // int
    Create( "BookmarkCoordSystem",  static_cast< int >( eCoordinateSystem_Mgrs ), false );
    Create( "CoordSystem",          static_cast< int >( eCoordinateSystem_Mgrs ), true );
    Create( "RefreshRate",          50,                                           true );

    // replay
    Create( "Replay/SmoothPositions", true, true );

    // color
    Create( "Color/ActiveViewFrame",     QString( "#f00f0f" ), true ); // not used yet
    Create( "Color/Contaminated",        QString( "#00ff00" ), true );
    Create( "Color/Dead",                QString( "#000000" ), true );
    Create( "Color/Healthy",             QString( "#2180ff" ), true );
    Create( "Color/MostlyHealthy",       QString( "#ffff00" ), true );
    Create( "Color/Neutralized",         QString( "#ebe665" ), true );
    Create( "Color/Phantom",             QString( "#3cb45a" ), true );
    Create( "Color/TacticallyDestroyed", QString( "#ebb965" ), true );
    Create( "Color/TotallyDestroyed",    QString( "#eb6a65" ), true );
    Create( "Color/Wounded",             QString( "#ff0000" ), true );

    // sound
    tools::Path soundPath;
    soundPath = soundPath.Absolute( tools::GeneralConfig::BuildResourceChildFile( "sounds" ) );
    Create( "Sound/Directory",                   QString( soundPath.Normalize().ToUTF8().c_str() ), true );
    Create( "Sound/Mute",                        false, true );
    Create( "Sound/Volume/directfire",           50,    true );
    Create( "Sound/Volume/indirectsmoke",        50,    true );
    Create( "Sound/Volume/indirectexplosive",    50,    true );
    Create( "Sound/Volume/indirectillumination", 50,    true );
    Create( "Sound/Volume/indirecteffect",       50,    true );
}

// -----------------------------------------------------------------------------
// Name: Options::InitializeView
// Created: ABR 2014-07-24
// -----------------------------------------------------------------------------
void Options::InitializeView()
{
    // bool
    Create( "3D",                    false, false );
    Create( "DisplayDestroyedUnits", true,  false );
    Create( "FogOfWar",              false, true );
    Create( "Infra",                 true,  true );

    Create( "3DElevationRatio", 10.f, false );
    // tri state
    Create( "BigText",   TristateOption::Auto(), false );
    Create( "SmallText", TristateOption::Auto(), false );

    // four state
    Create( "ConvexHulls",          FourStateOption::Selected(), false );
    Create( "DecisionalState",      FourStateOption::Selected(), false );
    Create( "Direction",            FourStateOption::Selected(), false );
    Create( "LogisticLinks",        FourStateOption::Selected(), false );
    Create( "MissingLogisticLinks", FourStateOption::Selected(), false );
    Create( "MissionParameters",    FourStateOption::Selected(), false );
    Create( "OldPaths",             FourStateOption::Selected(), false );
    Create( "Paths",                FourStateOption::Selected(), false );
    Create( "RealTimeLogistic",     FourStateOption::Selected(), false );
    Create( "TacticalLines",        FourStateOption::Selected(), false );
    Create( "UnitDetails",          FourStateOption::On()      , false );
    Create( "VisionCones",          FourStateOption::Selected(), false );
    Create( "VisionLines",          FourStateOption::Selected(), false );
    Create( "VisionSurfaces",       FourStateOption::Selected(), false );
    Create( "WeaponRanges",         FourStateOption::Selected(), false );

    // Accommodation
    Create( "Accommodation/Enabled",    false,                            true );
    Create( "Accommodation/Displayed",  QString( "" ),                    true );
    Create( "Accommodation/Gradient",   QString( "0,#00ff00;1,#ff0000" ), true );
    Create( "Accommodation/Max",        1.f,                              true );
    Create( "Accommodation/Min",        0.f,                              true );
    Create( "Accommodation/Unoccupied", QString( "#000000" ),             true );

    // Contour lines
    Create( "ContourLines/Color",   QString( "#f5f5dc" ), false );
    Create( "ContourLines/Enabled", false,                false );
    Create( "ContourLines/Height",  100,                  false );

    // Density
    Create( "Density/Enabled",    false,                            true );
    Create( "Density/Gradient",   QString( "0,#00ff00;1,#ff0000" ), true );
    Create( "Density/Max",        1.f,                              true );
    Create( "Density/Min",        0.f,                              true );
    Create( "Density/Unoccupied", QString( "#000000" ),             true );

    // Efficient range
    Create( "EfficientRange/Ph",                   50,                   true );
    Create( "EfficientRange/Volume",               0,                    true );
    Create( "EfficientRange/FilterIndirectWeapon", false,                true );
    Create( "EfficientRange/IndirectWeapon",       QString( "" ),        true );
    Create( "EfficientRange/UseCustomColor",       false,                true );
    Create( "EfficientRange/CustomColor",          QString( "#ffffff" ), true );

    // Elevation
    Create( "Elevation/FitToViewPort", false,                true );
    Create( "Elevation/Gradient",      QString( "default" ), true );
    tools::Xifstream xisGradients( "gradients.xml" );
    xisGradients >> xml::start( "gradients" )
                 >> xml::list( "gradient", *this, &Options::ReadGradient );

    // Fires
    // FireRules are empty by default
    Create( "FireRules/indicators", 0, false );
    Create( "FireRules/direct", QString(), true );
    Create( "FireRules/indirect", QString(), true );

    // Grid
    Create( "Grid/Enabled", false, true );
    Create( "Grid/Size", 0.5f, true );
    Create( "Grid/Type", static_cast< int >( eCoordinateSystem_Local ), true );
    Create( "Grid/Color", QString( "#ffffff" ), true );


    // HillShade
    Create( "HillShade/Enabled",   false, true );
    Create( "HillShade/Direction", 335,   true );
    Create( "HillShade/Strength",  1,     true );

    // Lighting
    Create( "Lighting/Type",      0,                    true );
    Create( "Lighting/Direction", QString( "23:23" ),   true );
    Create( "Lighting/Ambient",   QString( "#323232" ), true );
    Create( "Lighting/Diffuse",   QString( "#c8c8c8" ), true );

    // Resource networks
    Create( "ResourceNetworks", 0, false );

    // Symbol size
    Create( "SymbolSize/CurrentFactor", 3.f, false );
    for( int i = eNatureLevel_c; i < eNbrNatureLevel; ++i )
        Create( "SymbolSize/" + ENT_Tr::ConvertFromNatureLevel( static_cast< E_NatureLevel >( i ) ), 1.f, true );
    Create( "SymbolSize/Headquarters", 1.f, true );

    // Terrain
    QStringList order;
    tools::Xifstream xisPreferences( "preferences.xml" );
    xisPreferences >> xml::start( "preferences" ) >> xml::start( "terrains" )
                   >> xml::list( "terrain", [&]( xml::xistream& x ) {
                       const auto type = x.attribute< std::string >( "type" );
                       const QString category = x.attribute< QString >( "category" );
                       Create( "Terrains/" + type + "/Category", category,                                       true );
                       Create( "Terrains/" + type + "/Name",     x.attribute< QString >( "name", type.c_str() ), true );
                       Create( "Terrains/" + type + "/Shown",    true,                                           true );
                       Create( "Terrains/" + type + "/Width",    x.content< float >( "width", 1.f ),             true );
                       Create( "Terrains/" + type + "/Color",    x.content< QString >( "color", "#000000" ),     true );
                       if( !category.isEmpty() && !order.contains( category ) )
                           order << category;
                       } );
    Create( "Terrains/Order", order.join( ";" ), true );

    // Visualisation scales
    for( size_t i = 0; i < DefaultVisualisationScales::size_; ++i )
    {
        const auto& scale = DefaultVisualisationScales::data_[i];
        const auto name = "VisualisationScales/" + ENT_Tr::ConvertFromVisualisationScale( scale.type_ );
        Create( name + "/Min", scale.min_, true );
        Create( name + "/Max", scale.max_, true );
    }

    // Watershed
    Create( "Watershed/Color",   QString( "#14a4da" ), false );
    Create( "Watershed/Enabled", false,                false );
    Create( "Watershed/Height",  0,                    false );
    Create( "Watershed/Inverse", false,                false );
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
        float alpha = 1.f;
        if( types[ i ] == eLayerTypes_Grid )
            alpha = 0.3f;
        else if( types[ i ] == eLayerTypes_VisionCones )
            alpha = 0.5f;
        Create( name + "/Alpha", alpha, true );
        Create( name + "/Position", i, true );
    }
}
