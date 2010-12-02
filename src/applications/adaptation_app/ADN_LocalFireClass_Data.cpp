// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_LocalFireClass_Data.h"
#include "ADN_DataException.h"
#include "ADN_Project_Data.h"

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::LocalFireClassInfos
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_LocalFireClass_Data::LocalFireClassInfos::LocalFireClassInfos()
    : agents_      ( ADN_Workspace::GetWorkspace().GetEquipements().GetData().GetDotation( eDotationFamily_AgentExtincteur ).categories_ )
    , initialHeat_ ( 0 )
    , maxHeat_     ( 0 )
    , increaseRate_( 0 )
    , decreaseRate_( 0 )
{
    initialHeat_.SetParentNode( *this );
    maxHeat_.SetParentNode( *this );
    increaseRate_.SetParentNode( *this );
    decreaseRate_.SetParentNode( *this );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::~LocalFireClassInfos
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_LocalFireClass_Data::LocalFireClassInfos::~LocalFireClassInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::GetNodeName
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
std::string ADN_LocalFireClass_Data::LocalFireClassInfos::GetNodeName()
{
    std::string strResult( "de la classe d'incendie " );
    return strResult + strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::GetItemName
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
std::string ADN_LocalFireClass_Data::LocalFireClassInfos::GetItemName()
{
    return strName_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::CreateCopy
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_LocalFireClass_Data::LocalFireClassInfos* ADN_LocalFireClass_Data::LocalFireClassInfos::CreateCopy()
{
    LocalFireClassInfos* pCopy = new LocalFireClassInfos();
    pCopy->strName_ = strName_.GetData();
    pCopy->initialHeat_ = initialHeat_.GetData();
    pCopy->maxHeat_ = maxHeat_.GetData();
    pCopy->increaseRate_ = increaseRate_.GetData();
    pCopy->decreaseRate_ = decreaseRate_.GetData();
    for( uint n = 0; n < agents_.size(); ++n )
        pCopy->agents_[ n ]->CopyFrom( *agents_[ n ] );
    return pCopy;
}
    
// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::ReadArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::LocalFireClassInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::attribute( "name", strName_ )
          >> xml::attribute( "initial-heat", initialHeat_ )
          >> xml::attribute( "max-heat", maxHeat_ )
          >> xml::attribute( "increase-rate", increaseRate_ )
          >> xml::attribute( "decrease-rate", decreaseRate_ )
          >> xml::start( "extinguisher-agents" )
              >> xml::list( "extinguisher-agent", *this, &ADN_LocalFireClass_Data::LocalFireClassInfos::ReadAgent )
          >> xml::end
          >> xml::start( "weather-effects" )
              >> xml::list( "weather-effect", *this, &ADN_LocalFireClass_Data::LocalFireClassInfos::ReadWeatherEffect )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::ReadAgent
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::LocalFireClassInfos::ReadAgent( xml::xistream& input )
{
    std::string agent = input.attribute< std::string >( "agent" );
    IT_ExtinguisherAgentInfos_Vector itAgent = std::find_if( agents_.begin(), agents_.end(), ADN_ExtinguisherAgentInfos::Cmp( agent ));
    if( itAgent == agents_.end() )
        throw ADN_DataException( tr( "Invalid data" ).ascii(), tr( "Fire class - Invalid extinguisher agent '%1'" ).arg( agent.c_str() ).ascii() );
    ( *itAgent )->ReadArchive( input );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::ReadWeatherEffect
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::LocalFireClassInfos::ReadWeatherEffect( xml::xistream& /*input*/ )
{
    int todo = 0;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::LocalFireClassInfos::WriteArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::LocalFireClassInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "local-fire-class" )
               << xml::attribute( "name", strName_ )
               << xml::attribute( "initial-heat", initialHeat_ )
               << xml::attribute( "max-heat", maxHeat_ )
               << xml::attribute( "increase-rate", increaseRate_ )
               << xml::attribute( "decrease-rate", decreaseRate_ )
               << xml::start( "extinguisher-agents" );
    for( IT_ExtinguisherAgentInfos_Vector itAgent = agents_.begin(); itAgent != agents_.end(); ++itAgent )
        ( *itAgent )->WriteArchive( output );
    output     << xml::end
               << xml::start( "weather-effects" );
    /*for( IT_ExtinguisherAgentInfos_Vector itAgent = agents_.begin(); itAgent != agents_.end(); ++itAgent )
        ( *itAgent )->WriteArchive( output );*/
    output     << xml::end
           << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data constructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_LocalFireClass_Data::ADN_LocalFireClass_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data destructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_LocalFireClass_Data::~ADN_LocalFireClass_Data()
{
    Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::FilesNeeded
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::FilesNeeded( T_StringList& files ) const
{
    files.push_back( ADN_Workspace::GetWorkspace().GetProject().GetDataInfos().szLocalFireClasses_.GetData() );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::Reset
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::Reset()
{
    localFireClasses_.Reset();
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::ReadArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "local-fire-classes" )
            >> xml::list( "local-fire-class", *this, &ADN_LocalFireClass_Data::ReadLocalFireClass )
          >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::ReadLocalFireClass
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::ReadLocalFireClass( xml::xistream& input )
{
    std::auto_ptr< LocalFireClassInfos > spNew( new LocalFireClassInfos() );
    spNew->ReadArchive( input );
    localFireClasses_.AddItem( spNew.release() );
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::WriteArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_LocalFireClass_Data::WriteArchive( xml::xostream& output )
{
    output << xml::start( "local-fire-classes" );
    ADN_Tools::AddSchema( output, "LocalFireClasses" );
    for( IT_LocalFireClassInfosVector it = localFireClasses_.begin(); it != localFireClasses_.end(); ++it )
        ( *it )->WriteArchive( output );
    output << xml::end;
}

// -----------------------------------------------------------------------------
// Name: ADN_LocalFireClass_Data::GetLocalFireClassesInfos
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
ADN_LocalFireClass_Data::T_LocalFireClassInfosVector& ADN_LocalFireClass_Data::GetLocalFireClassesInfos()
{
    return localFireClasses_;
}
