// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************
#include "orbat_generator_pch.h"
#include "IdNameGenerator.h"
#include <iostream>
#include <xeumeuleu/xml.hpp>

using namespace orbat_generator;

// -----------------------------------------------------------------------------
// Name: IdNameGenerator constructor
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
IdNameGenerator::IdNameGenerator( xml::xisubstream xis )
    : partyId_    ( 0 )
    , formationId_( 0 )
    , automateId_ ( 0 )
    , pionId_     ( 0 )
{
    xis >> xml::start( "structure" )
          >> xml::attribute( "pattern", pattern_ )
          >> xml::start( "party" )
            >> xml::attribute( "pattern", partyName_ )
          >> xml::end
          >> xml::start( "formation" )
            >> xml::attribute( "pattern", formationName_ )
          >> xml::end
          >> xml::start( "automat" )
              >> xml::attribute( "pattern", automateName_ )
          >> xml::end
          >> xml::start( "unit" )
              >> xml::attribute( "pattern", pionName_ )
          >> xml::end
          >> xml::start( "crowd" )
              >> xml::attribute( "pattern", crowdName_ );
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator destructor
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
IdNameGenerator::~IdNameGenerator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::Init
// Created: RCD 2011-03-22
// -----------------------------------------------------------------------------
void IdNameGenerator::RewindAll() const
{
    Rewind( crowdName_ );
    Rewind( partyName_ );
    Rewind( formationName_ );
    Rewind( automateName_ );
    Rewind( pionName_ );
    partyId_ = 0;
    formationId_ = 0;
    automateId_ = 0;
    pionId_ = 0 ;
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputeAutomateId
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
unsigned int IdNameGenerator::ComputeAutomateId() const
{
    pionId_ = 0;
    return partyId_ + formationId_ * 100 + ++automateId_ * 10000;
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputeAutomateName
// Created: RCD 2011-03-07
// -----------------------------------------------------------------------------
std::string IdNameGenerator::ComputeAutomateName() const
{
    std::string completeName;
    Increment( automateName_ );
    Rewind( pionName_ );
    completeName = pattern_;
    std::string criteria;
    criteria ="[$P]";
    RegReplace( completeName, criteria, partyName_ + "." );
    criteria ="[$F]";
    RegReplace( completeName, criteria, formationName_ + "." );
    criteria ="[$A]";
    RegReplace( completeName, criteria, automateName_ );
    criteria ="[$U]";
    RegReplace( completeName, criteria, "" );
    return completeName;
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputeFormationId
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
unsigned int IdNameGenerator::ComputeFormationId() const
{
    automateId_ = pionId_ = 0;
    return partyId_ + ( ++formationId_ ) * 100;;
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputeFormationName
// Created: RCD 2011-03-07
// -----------------------------------------------------------------------------
std::string IdNameGenerator::ComputeFormationName() const
{
    std::string completeName;
    Increment( formationName_ );
    Rewind( automateName_ );
    Rewind( pionName_ );
    completeName = pattern_;
    std::string criteria;
    criteria ="[$P]";
    RegReplace( completeName, criteria, partyName_ + "." );
    criteria ="[$F]";
    RegReplace( completeName, criteria, formationName_ );
    criteria ="[$A]";
    RegReplace( completeName, criteria, "" );
    criteria ="[$U]";
    RegReplace( completeName, criteria, "" );
    return completeName;
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputePartyId
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
unsigned int IdNameGenerator::ComputePartyId() const
{
    automateId_ = formationId_ = pionId_ = 0;
    return ++partyId_;
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputePartyName
// Created: RCD 2011-03-07
// -----------------------------------------------------------------------------
std::string IdNameGenerator::ComputePartyName() const
{
    std::string completeName;
    Increment( partyName_ );
    Rewind( automateName_ );
    Rewind( pionName_ );
    Rewind( formationName_ );
    completeName = pattern_;
    std::string criteria;
    criteria ="[$P]";
    RegReplace( completeName, criteria, partyName_ );
    criteria ="[$F]";
    RegReplace( completeName, criteria, "" );
    criteria ="[$A]";
    RegReplace( completeName, criteria, "" );
    criteria ="[$U]";
    RegReplace( completeName, criteria, "" );
    return completeName;
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputePionId
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
unsigned int IdNameGenerator::ComputePionId() const
{
    return partyId_ + formationId_ * 100 + automateId_ * 10000 + ++pionId_ * 10000000;
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputePionName
// Created: RCD 2011-03-07
// -----------------------------------------------------------------------------
std::string IdNameGenerator::ComputePionName() const
{
    std::string completeName;
    Increment( pionName_ );
    completeName = pattern_;
    std::string criteria;
    criteria ="[$P]";
    RegReplace( completeName, criteria, partyName_ + "." );
    criteria ="[$F]";
    RegReplace( completeName, criteria, formationName_ + "."  );
    criteria ="[$A]";
    RegReplace( completeName, criteria, automateName_ + "."  );
    criteria ="[$U]";
    RegReplace( completeName, criteria, pionName_ );
    return completeName;
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputeCrowdName
// Created: RCD 2011-03-07
// -----------------------------------------------------------------------------
std::string IdNameGenerator::ComputeCrowdName() const
{
    Increment( crowdName_ );
    std::string completeName = pattern_;
    std::string criteria;
    criteria ="[$P]";
    RegReplace( completeName, criteria, partyName_ + "." );
    criteria ="[$F]";
    RegReplace( completeName, criteria, "" );
    criteria ="[$A]";
    RegReplace( completeName, criteria, "" );
    criteria ="[$U]";
    RegReplace( completeName, criteria, crowdName_ );
    return completeName;
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::Replace
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
void IdNameGenerator::RegReplace( std::string& name, std::string& toReplace, std::string replaceBy ) const
{
    unsigned int pos = name.find( toReplace );
    if ( !( pos > name.length() - 4 ) )
        name.replace( pos, 4, replaceBy );
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::Increment
// Created: RCD 2011-03-08
// -----------------------------------------------------------------------------
void IdNameGenerator::Increment( std::string& name ) const
{
    bool done = false;
    int pos = name.length() - 1;
    while( !done && pos >= 0 )
    {
        switch( name[ pos ] )
        {
        case '9' : name[ pos ] = '0';
                   --pos;
                   break;
        case 'z' : name[ pos ] = 'a';
                   --pos;
                   break;
        default  : ++name[ pos ];
                   done = true;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::Rewind
// Created: RCD 2011-03-08
// -----------------------------------------------------------------------------
void IdNameGenerator::Rewind( std::string& name ) const
{
    for( unsigned int pos = 0; pos < name.length(); ++pos )
    {
        if( name[ pos ] >= '0' && name[ pos ] <= '9' )
            name[ pos ] = '0';
        else
            name[ pos ] = 'z';
    }
}