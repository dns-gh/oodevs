// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-11-19 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Weapons_Data.cpp $
// $Author: Nld $
// $Modtime: 3/05/05 10:03 $
// $Revision: 13 $
// $Workfile: ADN_Weapons_Data.cpp $
//
// *****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Weapons_Data_PhInfos.h"
#include "ADN_Categories_Data.h"
#include "ADN_Workspace.h"
#include "ADN_Tr.h"
#include "ADN_WorkspaceElement.h"

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_PhInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Weapons_Data_PhInfos::ADN_Weapons_Data_PhInfos()
    : nDistance_(0)
    , rPerc_(0.0)
    , nModifiedDistance_( 0 )
    , rModifiedPerc_( 0.0 )
    , distModifier_( 1.0 )
    , phModifier_( 1.0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_CreateCopy
// Created: APE 2005-01-12
// -----------------------------------------------------------------------------
ADN_Weapons_Data_PhInfos* ADN_Weapons_Data_PhInfos::CreateCopy()
{
    ADN_Weapons_Data_PhInfos* pCopy = new ADN_Weapons_Data_PhInfos();
    pCopy->nDistance_ = nDistance_.GetData();
    pCopy->rPerc_ = rPerc_.GetData();
    pCopy->nModifiedDistance_ = nModifiedDistance_.GetData();
    pCopy->rModifiedPerc_ = rModifiedPerc_.GetData();

    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_PhInfos::SetPhModifiers
// Created: JSR 2010-04-27
// -----------------------------------------------------------------------------
void ADN_Weapons_Data_PhInfos::SetPhModifiers( double distModifier, double phModifier )
{
    distModifier_ = distModifier;
    phModifier_ = phModifier;
    ApplyPhModifiers();
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_PhInfos::ApplyPhModifiers
// Created: JSR 2010-04-27
// -----------------------------------------------------------------------------
void ADN_Weapons_Data_PhInfos::ApplyPhModifiers()
{
    nModifiedDistance_ = ( int ) ( nDistance_.GetData() * distModifier_ );
    rModifiedPerc_ = rPerc_.GetData() * phModifier_;
    if( rModifiedPerc_.GetData() > 100. )
        rModifiedPerc_ = 100.;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_PhInfos::ReadArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data_PhInfos::ReadArchive( xml::xistream& input )
{
    double rPerc;
    input >> xml::attribute( "distance", nDistance_ )
        >> xml::attribute( "percentage", rPerc );
    rPerc_ = rPerc * 100.;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_PhInfos::WriteArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data_PhInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "hit-probability" )
        << xml::attribute( "distance", nDistance_ )
        << xml::attribute( "percentage", rPerc_.GetData() / 100.0 )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_PhSizeInfos::ADN_Weapons_Data_PhSizeInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Weapons_Data_PhSizeInfos::ADN_Weapons_Data_PhSizeInfos( ADN_Volumes_Data::VolumeInfos* ptr )
    : ADN_CrossedRef< ADN_Volumes_Data::VolumeInfos >( ADN_Workspace::GetWorkspace().GetCategories().GetData().GetElement< ADN_Volumes_Data >( eVolumes ).GetVolumesInfos(), ptr, true )
    , vPhs_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_PhSizeInfos::~ADN_Weapons_Data_PhSizeInfos
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
ADN_Weapons_Data_PhSizeInfos::~ADN_Weapons_Data_PhSizeInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_PhSizeInfos::ReadHp
// Created: AGE 2007-08-17
// -----------------------------------------------------------------------------
void ADN_Weapons_Data_PhSizeInfos::ReadHp( xml::xistream& input )
{
    std::unique_ptr< ADN_Weapons_Data_PhInfos > spNew( new ADN_Weapons_Data_PhInfos() );
    spNew->ReadArchive( input );
    vPhs_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Weapons_Data_PhSizeInfos::ReadArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data_PhSizeInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::list( "hit-probability", *this, &ADN_Weapons_Data_PhSizeInfos::ReadHp );
}

// -----------------------------------------------------------------------------
// Name: PhSizeInfos::WriteArchive
// Created: APE 2004-11-22
// -----------------------------------------------------------------------------
void ADN_Weapons_Data_PhSizeInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "hit-probabilities" )
        << xml::attribute( "target", *this );
    for( auto it = vPhs_.begin(); it != vPhs_.end(); ++it )
        (*it)->WriteArchive( output );
    output << xml::end;
}
