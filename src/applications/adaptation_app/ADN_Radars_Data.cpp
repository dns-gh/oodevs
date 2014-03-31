// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_Radars_Data.h"
#include "ADN_Project_Data.h"
#include "ADN_Categories_Data.h"
#include "ADN_WorkspaceElement.h"
#include "ADN_Tools.h"
#include "ADN_Tr.h"
#include "ADN_Workspace.h"
#include "clients_kernel/XmlTranslations.h"
#include "ENT/ENT_Tr.h"

// -----------------------------------------------------------------------------
// Name: DetectTimes::DetectTimes
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
ADN_Radars_Data::DetectTimes::DetectTimes()
    : bDetectTime_( false )
    , detectTime_ ( "0s" )
    , bIdentTime_ ( false )
    , recoTime_   ( "0s" )
    , bRecoTime_  ( false )
    , identTime_  ( "0s" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectTimes::~DetectTimes
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
ADN_Radars_Data::DetectTimes::~DetectTimes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DetectTimes::ReadArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Radars_Data::DetectTimes::ReadArchive( xml::xistream& input, bool bHq )
{
    input >> xml::optional
          >> xml::start( "acquisition-times" )
            >> xml::list( "acquisition-time", *this, &ADN_Radars_Data::DetectTimes::ReadAcquisitionTime, bHq )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data::DetectTimes::ReadAcquisitionTime
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Radars_Data::DetectTimes::ReadAcquisitionTime( xml::xistream& input, bool bHq )
{
    const std::string timeKey = bHq ? "command-post-time" : "base-time";
    std::string time, level;
    input >> xml::optional >> xml::attribute( timeKey, time )
          >> xml::attribute( "level", level );
    if( time.empty() )
        return;
    if( level == "identification" )
    {
        bIdentTime_ = true;
        identTime_ = time;
    }
    else if( level == "recognition" )
    {
        bRecoTime_ = true;
        recoTime_ = time;
    }
    else if( level == "detection" )
    {
        bDetectTime_ = true;
        detectTime_ = time;
    }
    else
        throw MASA_EXCEPTION( tools::translate( "Radars_Data", "Sensors - Invalid level '%1'" ).arg( level.c_str() ).toStdString() );
}

// -----------------------------------------------------------------------------
// Name: DetectTimes::WriteArchive
// Created: APE 2005-01-17
// -----------------------------------------------------------------------------
void ADN_Radars_Data::DetectTimes::WriteArchive( xml::xostream& output, bool bHq ) const
{
    const std::string time = bHq ? "command-post-time" : "base-time";
    if( bDetectTime_.GetData() )
        output << xml::start( "acquisition-time" )
                << xml::attribute( "level", "detection" )
                << xml::attribute( time, detectTime_ )
               << xml::end;
    if( bRecoTime_.GetData() )
        output << xml::start( "acquisition-time" )
                << xml::attribute( "level", "recognition" )
                << xml::attribute( time, recoTime_ )
               << xml::end;
    if( bIdentTime_.GetData() )
        output << xml::start( "acquisition-time" )
                << xml::attribute( "level", "identification" )
                << xml::attribute( time, identTime_ )
               << xml::end;
}

// -----------------------------------------------------------------------------
// Name: RadarInfos::RadarInfos
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
ADN_Radars_Data::RadarInfos::RadarInfos()
    : ADN_RefWithLocalizedName( tools::translate( "Radars_Data", "New special sensor" ) )
    , rRange_                  ( 0 )
    , bHasMaxHeight_           ( false )
    , rMaxHeight_              ( 0 )
    , bHasMinHeight_           ( false )
    , rMinHeight_              ( 0 )
    , bHasDetectableActivities_( false )
    , bHasDetectTimes_         ( false )
    , bHasHQDetectTimes_       ( false )
    , modificators_            ( new ADN_Sensors_Modificators( false ) )
{
    strName_.SetContext( ADN_Workspace::GetWorkspace().GetContext( eSensors, "radars" ) );
    for( int n = 0; n < eNbrConsumptionType; ++n )
        detectableActivities_[ n ] = false;
}

// -----------------------------------------------------------------------------
// Name: RadarInfos::~RadarInfos
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
ADN_Radars_Data::RadarInfos::~RadarInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data::CreateCopy
// Created: LGY 2010-08-18
// -----------------------------------------------------------------------------
ADN_Radars_Data::RadarInfos* ADN_Radars_Data::RadarInfos::CreateCopy()
{
    RadarInfos* pCopy = new RadarInfos();
    pCopy->nType_ = nType_.GetData();
    pCopy->rRange_ = rRange_.GetData();
    pCopy->bHasMaxHeight_ = bHasMaxHeight_.GetData();
    pCopy->rMaxHeight_ = rMaxHeight_.GetData();
    pCopy->bHasMinHeight_ = bHasMinHeight_.GetData();
    pCopy->rMinHeight_ = rMinHeight_.GetData();
    pCopy->bHasDetectableActivities_ = bHasDetectableActivities_.GetData();
    for( int n = 0; n < eNbrConsumptionType; ++n )
        pCopy->detectableActivities_[ n ] = detectableActivities_[ n ].GetData();
    pCopy->bHasDetectTimes_ = bHasDetectTimes_.GetData();
    pCopy->detectTimes_.bDetectTime_ = detectTimes_.bDetectTime_.GetData();
    pCopy->detectTimes_.detectTime_ = detectTimes_.detectTime_.GetData();
    pCopy->detectTimes_.bIdentTime_ = detectTimes_.bIdentTime_.GetData();
    pCopy->detectTimes_.recoTime_ = detectTimes_.recoTime_.GetData();
    pCopy->detectTimes_.bRecoTime_ = detectTimes_.bRecoTime_.GetData();
    pCopy->detectTimes_.identTime_ = detectTimes_.identTime_.GetData();
    pCopy->bHasHQDetectTimes_ = bHasHQDetectTimes_.GetData();
    pCopy->hqDetectTimes_.bDetectTime_ = hqDetectTimes_.bDetectTime_.GetData();
    pCopy->hqDetectTimes_.detectTime_ = hqDetectTimes_.detectTime_.GetData();
    pCopy->hqDetectTimes_.bIdentTime_ = hqDetectTimes_.bIdentTime_.GetData();
    pCopy->hqDetectTimes_.recoTime_ = hqDetectTimes_.recoTime_.GetData();
    pCopy->hqDetectTimes_.bRecoTime_ = hqDetectTimes_.bRecoTime_.GetData();
    pCopy->hqDetectTimes_.identTime_ = hqDetectTimes_.identTime_.GetData();
    pCopy->modificators_->CopyFrom( *modificators_ );
    return pCopy;
}

// -----------------------------------------------------------------------------
// Name: RadarInfos::ReadArchive
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Radars_Data::RadarInfos::ReadArchive( xml::xistream& input )
{
    std::string type;
    input >> xml::attribute( "name", *this )
          >> xml::attribute( "type", type );
    nType_ = ADN_Tr::ConvertToRadarType( type );
    if( nType_ == E_RadarType(-1 ) )
        throw MASA_EXCEPTION( tools::translate( "Radars_Data", "Sensors - Invalid radar type '%1'" ).arg( type.c_str() ).toStdString() );

    input >> xml::attribute( "action-range", rRange_ )
          >> xml::optional >> xml::attribute( "min-height", rMinHeight_ )
          >> xml::optional >> xml::attribute( "max-height", rMaxHeight_ );
    bHasMinHeight_ = rMinHeight_ != 0;
    bHasMaxHeight_ = rMaxHeight_ != 0;

    input >> xml::optional
          >> xml::start( "detectable-activities" )
            >> xml::list( "detectable-activity", *this, &ADN_Radars_Data::RadarInfos::ReadDetectableActivity )
          >> xml::end;
    detectTimes_.ReadArchive( input, false );
    hqDetectTimes_.ReadArchive( input, true );
    bHasDetectTimes_ = detectTimes_.bDetectTime_.GetData()
                    || detectTimes_.bIdentTime_.GetData()
                    || detectTimes_.bRecoTime_.GetData();
    bHasHQDetectTimes_ = hqDetectTimes_.bDetectTime_.GetData()
                    || hqDetectTimes_.bIdentTime_.GetData()
                    || hqDetectTimes_.bRecoTime_.GetData();

    if( input.has_child( "distance-modifiers" ) )
    {
        input >> xml::start( "distance-modifiers" );
        modificators_->ReadArchive( input );
        input  >> xml::end; // "distance-modifiers"
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data::RadarInfos::ReadDetectableActivity
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Radars_Data::RadarInfos::ReadDetectableActivity( xml::xistream& input )
{
    std::string consumption;
    bool value = false;
    input >> xml::attribute( "type", consumption )
          >> xml::optional
          >> xml::attribute( "value", value );
    unsigned n = (unsigned)ADN_Tr::ConvertToConsumptionType( consumption );
    if( n == unsigned( -1 ) )
        throw MASA_EXCEPTION( tools::translate( "Radars_Data", "Sensors - Invalid activity '%1'" ).arg( consumption.c_str() ).toStdString() );
    detectableActivities_[n ] = value;
    bHasDetectableActivities_ = bHasDetectableActivities_.GetData() || value;
}

// -----------------------------------------------------------------------------
// Name: RadarInfos::WriteArchive
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Radars_Data::RadarInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "radar" )
            << xml::attribute( "name", *this )
            << xml::attribute( "type", nType_.Convert() )
            << xml::attribute( "action-range", rRange_ );
    if( bHasMinHeight_.GetData() )
        output << xml::attribute( "min-height", rMinHeight_ );
    if( bHasMaxHeight_.GetData() )
        output << xml::attribute( "max-height", rMaxHeight_ );

    if( bHasDetectableActivities_.GetData() )
    {
        output << xml::start( "detectable-activities" );
        for( int n = 0; n < eNbrConsumptionType; ++n )
            if( detectableActivities_[ n ].GetData() )
                output << xml::start( "detectable-activity" )
                        << xml::attribute( "type", ADN_Tr::ConvertFromConsumptionType( (E_ConsumptionType)n ) )
                        << xml::attribute( "value", true )
                       << xml::end;
        output << xml::end;
    }
    if( bHasDetectTimes_.GetData() || bHasHQDetectTimes_.GetData() )
    {
        output << xml::start( "acquisition-times" );
        detectTimes_.WriteArchive( output, false );
        hqDetectTimes_.WriteArchive( output, true );
        output << xml::end;
    }

    if( modificators_->NeedsSaving() )
    {
        output << xml::start( "distance-modifiers" );
        modificators_->WriteArchive( output );
        output << xml::end; // distance-modifiers
    }
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data constructor
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
ADN_Radars_Data::ADN_Radars_Data()
    : ADN_Data_ABC( eSensors )
{
    vRadars_.AddUniquenessChecker( eError, duplicateName_, &ADN_Tools::NameExtractor );
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data destructor
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
ADN_Radars_Data::~ADN_Radars_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data::FilesNeeded
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Radars_Data::FilesNeeded( tools::Path::T_Paths& /*files*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data::FindRadar
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
ADN_Radars_Data::RadarInfos* ADN_Radars_Data::FindRadar( const std::string& strName )
{
    auto it = std::find_if( vRadars_.begin(), vRadars_.end(), ADN_Tools::NameCmp( strName ) );
    if( it == vRadars_.end() )
        return 0;
    return *it;
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data::ReadArchive
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Radars_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "radars" )
            >> xml::list( "radar", *this, &ADN_Radars_Data::ReadRadar )
          >> xml::end;
    vRadars_.CheckValidity();
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data::ReadRadar
// Created: AGE 2007-08-16
// -----------------------------------------------------------------------------
void ADN_Radars_Data::ReadRadar( xml::xistream& input )
{
    std::auto_ptr< RadarInfos > spNew( new RadarInfos() );
    spNew->ReadArchive( input );
    vRadars_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data::WriteArchive
// Created: APE 2005-05-03
// -----------------------------------------------------------------------------
void ADN_Radars_Data::WriteArchive( xml::xostream& output ) const
{
    if( vRadars_.GetErrorStatus() == eError )
        throw MASA_EXCEPTION( tools::translate( "ADN_Radars_Data", "Invalid data on tab '%1', subtab '%2'" )
                              .arg( ADN_Tr::ConvertFromWorkspaceElement( currentTab_ ).c_str() ).arg( tools::translate( "ADN_Radars_Data", "Radars" ) ).toStdString() );

    output << xml::start( "radars" );
    for( auto it = vRadars_.begin(); it != vRadars_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_Radars_Data::GetRadars
// Created: ABR 2012-07-30
// -----------------------------------------------------------------------------
ADN_Radars_Data::T_RadarInfos_Vector& ADN_Radars_Data::GetRadars()
{
    return vRadars_;
}
