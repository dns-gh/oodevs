// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Sensors_Modificators.h"
#include "ADN_Categories_Data.h"
#include "ADN_WorkspaceElement.h"

//-----------------------------------------------------------------------------
// Name: SizeInfos::SizeInfos
// Created: JDY 03-08-28
//-----------------------------------------------------------------------------
ADN_Sensors_Modificators::SizeInfos::SizeInfos( ADN_Volumes_Data::VolumeInfos* ptr )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Volumes_Data >( eVolumes ).GetVolumesInfos(), ptr, true )
    , rCoeff_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SizeInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::SizeInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: SizeInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::SizeInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "distance-modifier" )
             << xml::attribute( "type", *this )
             << xml::attribute( "value", rCoeff_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: SizeInfos::NeedsSaving
// Created: JSR 2014-03-28
// -----------------------------------------------------------------------------
bool ADN_Sensors_Modificators::SizeInfos::NeedsSaving() const
{
    return rCoeff_ != 0;
}

//-----------------------------------------------------------------------------
// Name: IlluminationInfos::IlluminationInfos
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Sensors_Modificators::IlluminationInfos::IlluminationInfos( const E_LightingType& e )
    : eType_( e )
    , rCoeff_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IlluminationInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::IlluminationInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: IlluminationInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::IlluminationInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "distance-modifier" )
            << xml::attribute( "type", ENT_Tr::ConvertFromLightingType( eType_ ) )
            << xml::attribute( "value", rCoeff_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: IlluminationInfos::NeedsSaving
// Created: JSR 2014-03-28
// -----------------------------------------------------------------------------
bool ADN_Sensors_Modificators::IlluminationInfos::NeedsSaving() const
{
    return rCoeff_ != 0;
}

//-----------------------------------------------------------------------------
// Name: MeteoInfos::MeteoInfos
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Sensors_Modificators::MeteoInfos::MeteoInfos( const E_SensorWeatherModifiers& e )
    : eType_( e )
    , rCoeff_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MeteoInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::MeteoInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: MeteoInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::MeteoInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "distance-modifier" )
            << xml::attribute( "type", ADN_Tools::Scriptify( ADN_Tr::ConvertFromSensorWeatherModifiers( eType_ ) ) )
            << xml::attribute( "value", rCoeff_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MeteoInfos::NeedsSaving
// Created: JSR 2014-03-28
// -----------------------------------------------------------------------------
bool ADN_Sensors_Modificators::MeteoInfos::NeedsSaving() const
{
    return rCoeff_ != 0;
}

//-----------------------------------------------------------------------------
// Name: EnvironmentInfos::EnvironmentInfos
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ADN_Sensors_Modificators::EnvironmentInfos::EnvironmentInfos( const E_VisionObject& e )
    : eType_( e )
    , rCoeff_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EnvironmentInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::EnvironmentInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: EnvironmentInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::EnvironmentInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "distance-modifier" )
            << xml::attribute( "value", rCoeff_ )
            << xml::attribute( "type", ADN_Tr::ConvertFromVisionObject( eType_ ) )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: EnvironmentInfos::NeedsSaving
// Created: JSR 2014-03-28
// -----------------------------------------------------------------------------
bool ADN_Sensors_Modificators::EnvironmentInfos::NeedsSaving() const
{
    return rCoeff_ != ( eType_ == eVisionEmpty ? 1 : 0 );
}

//-----------------------------------------------------------------------------
// Name: EnvironmentInfos::UrbanBlockInfos
// Created: SLG 2010-03-02
//-----------------------------------------------------------------------------
ADN_Sensors_Modificators::UrbanBlockInfos::UrbanBlockInfos( ADN_Urban_Data::UrbanMaterialInfos* ptr )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos(), ptr, true )
    , rCoeff_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockInfos::ReadArchive
// Created: SLG 2010-03-02
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::UrbanBlockInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockInfos::WriteArchive
// Created: SLG 2010-03-02
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::UrbanBlockInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "distance-modifier" )
             << xml::attribute( "type", *this )
             << xml::attribute( "value", rCoeff_ )
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: UrbanBlockInfos::NeedsSaving
// Created: JSR 2014-03-28
// -----------------------------------------------------------------------------
bool ADN_Sensors_Modificators::UrbanBlockInfos::NeedsSaving() const
{
    return rCoeff_ != 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Modificators constructor
// Created: JSR 2014-03-27
// -----------------------------------------------------------------------------
ADN_Sensors_Modificators::ADN_Sensors_Modificators()
    : vModifSizes_( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Volumes_Data >( eVolumes ).GetVolumesInfos() )
    , vModifWeather_( false )
    , vModifIlluminations_( false )
    , vModifEnvironments_( false )
    , vModifUrbanBlocks_( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos() )
{
    for( int i = 0; i < eNbrLightingType; ++i )
        vModifIlluminations_.AddItem( new IlluminationInfos( static_cast< E_LightingType >( i ) ) );
    for( int i = 0; i < eNbrSensorWeatherModifiers; ++i )
        vModifWeather_.AddItem( new MeteoInfos( static_cast< E_SensorWeatherModifiers >( i ) ) );
    for( int i = 0; i < eNbrVisionObject; ++i )
        vModifEnvironments_.AddItem( new EnvironmentInfos( static_cast< E_VisionObject >( i ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Modificators destructor
// Created: JSR 2014-03-27
// -----------------------------------------------------------------------------
ADN_Sensors_Modificators::~ADN_Sensors_Modificators()
{
    // NOTHING
}

namespace
{
    template< typename T >
    void CopyModificators( T& dst, const T& src )
    {
        for( std::size_t i = 0; i < src.size(); ++i )
            dst[ i ]->rCoeff_ = src[ i ]->rCoeff_.GetData();
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Modificators::CopyFrom
// Created: JSR 2014-03-27
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::CopyFrom( const ADN_Sensors_Modificators& src )
{
    CopyModificators( vModifIlluminations_, src.vModifIlluminations_ );
    CopyModificators( vModifWeather_, src.vModifWeather_ );
    CopyModificators( vModifEnvironments_, src.vModifEnvironments_ );
    CopyModificators( vModifSizes_, src.vModifSizes_ );
    CopyModificators( vModifUrbanBlocks_, src.vModifUrbanBlocks_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Modificators::ReadSizeModifiers
// Created: JSR 2014-03-27
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::ReadSizeModifiers( xml::xistream& xis )
{
    xis >> xml::start( "size-modifiers" )
        >> xml::list( "distance-modifier", [&]( xml::xistream& xis )
        {
            const std::string type = xis.attribute< std::string >( "type" );
            auto it = std::find_if( vModifSizes_.begin(), vModifSizes_.end(),
                boost::bind( &ADN_Tools::CrossedRefNameCompare< ADN_Sensors_Modificators::SizeInfos >, _1, boost::cref( type ) ) );
            if( it == vModifSizes_.end() )
                throw MASA_EXCEPTION( tools::translate( "Sensor_Data", "Sensors - Invalid unit volume '%1'" ).arg( type.c_str() ).toStdString() );
            ( *it )->ReadArchive( xis );
        })
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Modificators::ReadMeteoModifiers
// Created: JSR 2014-03-27
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::ReadMeteoModifiers( xml::xistream& xis )
{
    xis >> xml::start( "precipitation-modifiers" )
        >> xml::list( "distance-modifier", [&]( xml::xistream& xis )
        {
            const std::string type = xis.attribute< std::string >( "type" );
            for( unsigned i = 0; i < eNbrSensorWeatherModifiers; ++i )
                if( ADN_Tools::Scriptify( ADN_Tr::ConvertFromSensorWeatherModifiers( static_cast< E_SensorWeatherModifiers >( i ) ) ) == type )
                {
                    vModifWeather_.at( i )->ReadArchive( xis );
                    return;
                }
            throw MASA_EXCEPTION( tools::translate( "Sensor_Data", "Sensors - Invalid weather '%1'" ).arg( type.c_str() ).toStdString() );
        })
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Modificators::ReadIlluminationModifiers
// Created: JSR 2014-03-27
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::ReadIlluminationModifiers( xml::xistream& xis )
{
    xis >> xml::start( "visibility-modifiers" )
        >> xml::list( "distance-modifier", [&]( xml::xistream& xis )
        {
            E_LightingType n = ENT_Tr::ConvertToLightingType( xis.attribute< std::string >( "type" ) );
            vModifIlluminations_.at( n )->ReadArchive( xis );
        })
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Modificators::ReadEnvironmentModifiers
// Created: JSR 2014-03-27
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::ReadEnvironmentModifiers( xml::xistream& xis )
{
    xis >> xml::start( "terrain-modifiers" )
        >> xml::list( "distance-modifier", [&]( xml::xistream& xis )
        {
            E_VisionObject n = ADN_Tr::ConvertToVisionObject( xis.attribute< std::string >( "type" ) );
            vModifEnvironments_.at( n )->ReadArchive( xis );
        })
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Modificators::ReadUrbanBlocksModifiers
// Created: JSR 2014-03-27
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::ReadUrbanBlocksModifiers( xml::xistream& xis )
{
    xis >> xml::start( "urbanBlock-material-modifiers" )
        >> xml::list( "distance-modifier", [&]( xml::xistream& xis )
        {
            const std::string type = xis.attribute< std::string >( "type" );
            auto it = std::find_if( vModifUrbanBlocks_.begin(), vModifUrbanBlocks_.end(), ADN_Sensors_Modificators::UrbanBlockInfos::Cmp( type ) );
            if( it == vModifUrbanBlocks_.end() )
                throw MASA_EXCEPTION( tools::translate( "Sensor_Data", "Sensors - Invalid unit volume '%1'" ).arg( type.c_str() ).toStdString() );
            ( *it )->ReadArchive( xis );
        })
        >> xml::end;
}

namespace
{
    template< typename T >
    void Write( xml::xostream& xos, const std::string& tag, const T& modificators )
    {
        xos << xml::start( tag );
        for( auto it = modificators.begin(); it != modificators.end(); ++it )
            ( *it )->WriteArchive( xos );
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Modificators::WriteSizeModifiers
// Created: JSR 2014-03-27
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::WriteSizeModifiers( xml::xostream& xos ) const
{
    Write( xos, "size-modifiers", vModifSizes_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Modificators::WriteMeteoModifiers
// Created: JSR 2014-03-27
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::WriteMeteoModifiers( xml::xostream& xos ) const
{
    Write( xos, "precipitation-modifiers", vModifWeather_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Modificators::WriteIlluminationModifiers
// Created: JSR 2014-03-27
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::WriteIlluminationModifiers( xml::xostream& xos ) const
{
    Write( xos, "visibility-modifiers", vModifIlluminations_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Modificators::WriteEnvironmentModifiers
// Created: JSR 2014-03-27
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::WriteEnvironmentModifiers( xml::xostream& xos ) const
{
    Write( xos, "terrain-modifiers", vModifEnvironments_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Modificators::WriteUrbanBlocksModifiers
// Created: JSR 2014-03-27
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::WriteUrbanBlocksModifiers( xml::xostream& xos ) const
{
    Write( xos, "urbanBlock-material-modifiers", vModifUrbanBlocks_ );
}

namespace
{
    template< typename T >
    bool SavingNeeded( const T& modificators )
    {
        for( auto it = modificators.begin(); it != modificators.end(); ++it )
            if( ( *it )->NeedsSaving() )
                return true;
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Modificators::NeedsSaving
// Created: JSR 2014-03-28
// -----------------------------------------------------------------------------
bool ADN_Sensors_Modificators::NeedsSaving() const
{
    return SavingNeeded( vModifSizes_ ) || SavingNeeded( vModifWeather_ ) || SavingNeeded( vModifIlluminations_ )
        || SavingNeeded( vModifEnvironments_ ) || SavingNeeded( vModifUrbanBlocks_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Sensors_Modificators::CheckDatabaseValidity
// Created: JSR 2014-03-27
// -----------------------------------------------------------------------------
void ADN_Sensors_Modificators::CheckDatabaseValidity( ADN_ConsistencyChecker& checker, const std::string& name ) const
{
    for( auto it = vModifUrbanBlocks_.begin(); it != vModifUrbanBlocks_.end(); ++it )
        ( *it )->CheckValidity( checker, name, eSensors, -1, tools::translate( "ADN_Sensors_Data", "UrbanBlock material modifiers" ).toStdString() );
    for( auto it = vModifSizes_.begin(); it != vModifSizes_.end(); ++it )
        ( *it )->CheckValidity( checker, name, eSensors, -1, tools::translate( "ADN_Sensors_Data", "Target size" ).toStdString() );
}
