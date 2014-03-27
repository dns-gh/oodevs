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
// Name: ModificatorSizeInfos::ModificatorSizeInfos
// Created: JDY 03-08-28
//-----------------------------------------------------------------------------
ModificatorSizeInfos::ModificatorSizeInfos( ADN_Volumes_Data::VolumeInfos* ptr )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Volumes_Data >( eVolumes ).GetVolumesInfos(), ptr, true )
    , rCoeff_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModificatorSizeInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ModificatorSizeInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ModificatorSizeInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ModificatorSizeInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "distance-modifier" )
             << xml::attribute( "type", *this )
             << xml::attribute( "value", rCoeff_ )
           << xml::end;
}

//-----------------------------------------------------------------------------
// Name: ModificatorIlluminationInfos::ModificatorIlluminationInfos
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ModificatorIlluminationInfos::ModificatorIlluminationInfos( const E_LightingType& e )
    : eType_( e )
    , rCoeff_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModificatorIlluminationInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ModificatorIlluminationInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ModificatorIlluminationInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ModificatorIlluminationInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "distance-modifier" )
            << xml::attribute( "type", ENT_Tr::ConvertFromLightingType( eType_ ) )
            << xml::attribute( "value", rCoeff_ )
           << xml::end;
}

//-----------------------------------------------------------------------------
// Name: ModificatorMeteoInfos::ModificatorMeteoInfos
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ModificatorMeteoInfos::ModificatorMeteoInfos( const E_SensorWeatherModifiers& e )
    : eType_( e )
    , rCoeff_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModificatorMeteoInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ModificatorMeteoInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ModificatorMeteoInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ModificatorMeteoInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "distance-modifier" )
            << xml::attribute( "type", ADN_Tools::Scriptify( ADN_Tr::ConvertFromSensorWeatherModifiers( eType_ ) ) )
            << xml::attribute( "value", rCoeff_ )
           << xml::end;
}

//-----------------------------------------------------------------------------
// Name: ModificatorEnvironmentInfos::ModificatorEnvironmentInfos
// Created: JDY 03-07-24
//-----------------------------------------------------------------------------
ModificatorEnvironmentInfos::ModificatorEnvironmentInfos( const E_VisionObject& e )
    : eType_( e )
    , rCoeff_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModificatorEnvironmentInfos::ReadArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ModificatorEnvironmentInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ModificatorEnvironmentInfos::WriteArchive
// Created: APE 2004-11-23
// -----------------------------------------------------------------------------
void ModificatorEnvironmentInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "distance-modifier" )
            << xml::attribute( "value", rCoeff_ )
            << xml::attribute( "type", ADN_Tr::ConvertFromVisionObject( eType_ ) )
           << xml::end;
}

//-----------------------------------------------------------------------------
// Name: ModificatorEnvironmentInfos::ModificatorUrbanBlockInfos
// Created: SLG 2010-03-02
//-----------------------------------------------------------------------------
ModificatorUrbanBlockInfos::ModificatorUrbanBlockInfos( ADN_Urban_Data::UrbanMaterialInfos* ptr )
    : ADN_CrossedRef( ADN_Workspace::GetWorkspace().GetUrban().GetData().GetMaterialsInfos(), ptr, true )
    , rCoeff_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModificatorUrbanBlockInfos::ReadArchive
// Created: SLG 2010-03-02
// -----------------------------------------------------------------------------
void ModificatorUrbanBlockInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "value", rCoeff_ );
}

// -----------------------------------------------------------------------------
// Name: ModificatorUrbanBlockInfos::WriteArchive
// Created: SLG 2010-03-02
// -----------------------------------------------------------------------------
void ModificatorUrbanBlockInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "distance-modifier" )
             << xml::attribute( "type", *this )
             << xml::attribute( "value", rCoeff_ )
           << xml::end;
}
