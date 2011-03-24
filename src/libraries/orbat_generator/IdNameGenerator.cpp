// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************
#include "IdNameGenerator.h"
#include <iostream>

using namespace orbat_generator;

// -----------------------------------------------------------------------------
// Name: IdNameGenerator constructor
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
IdNameGenerator::IdNameGenerator()
    : partyId_    ( 0 )
    , formationId_( 0 )
    , automateId_ ( 0 )
    , pionId_     ( 0 )
{
    // NOTHING
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
// Created: RCD 2011-03-14
// -----------------------------------------------------------------------------
void IdNameGenerator::Init( std::string pattern, std::string partyPattern, std::string formationPattern, std::string automatePattern, std::string pionPattern, std::string crowdPattern )
{
    pattern_ = pattern;
    Rewind( crowdName_ = crowdPattern );
    Rewind( partyName_ = partyPattern );
    Rewind( formationName_ = formationPattern );
    Rewind( automateName_ = automatePattern );
    Rewind( pionName_ = pionPattern );
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputeAutomateId
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
unsigned int IdNameGenerator::ComputeAutomateId()
{
    unsigned int id = partyId_ + formationId_ * 100 + ++automateId_ * 10000;
    pionId_ = 0;
    return id;
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputeAutomateName
// Created: RCD 2011-03-07
// -----------------------------------------------------------------------------
std::string IdNameGenerator::ComputeAutomateName()
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
unsigned int IdNameGenerator::ComputeFormationId()
{
    unsigned int id = partyId_ + ++formationId_ * 100;
    automateId_ = pionId_ = 0;
    return id;
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputeFormationName
// Created: RCD 2011-03-07
// -----------------------------------------------------------------------------
std::string IdNameGenerator::ComputeFormationName()
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
unsigned int IdNameGenerator::ComputePartyId()
{
    unsigned int id = ++partyId_;
    automateId_ = formationId_ = pionId_ = 0;
    return id;
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputePartyName
// Created: RCD 2011-03-07
// -----------------------------------------------------------------------------
std::string IdNameGenerator::ComputePartyName()
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
unsigned int IdNameGenerator::ComputePionId()
{
    unsigned int id = partyId_ + formationId_ * 100 + automateId_ * 10000 + ++pionId_ * 10000000;
    return id;
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputePionName
// Created: RCD 2011-03-07
// -----------------------------------------------------------------------------
std::string IdNameGenerator::ComputePionName()
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
std::string IdNameGenerator::ComputeCrowdName()
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
void IdNameGenerator::RegReplace( std::string& name, std::string& toReplace, std::string replaceBy )
{
    unsigned int pos = name.find( toReplace );
    if ( !( pos > name.length() - 4 ) )
        name.replace( pos, 4, replaceBy );
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::Increment
// Created: RCD 2011-03-08
// -----------------------------------------------------------------------------
void IdNameGenerator::Increment( std::string& name )
{
    bool done = false;
    unsigned int pos = name.length() - 1;
    while( !done && pos >= 0 )
    {
        switch( name[ pos ] )
        {
        case '9' : name[ pos ] = '0';
                   ++pos;
                   break;
        case 'z' : name[ pos ] = 'a';
                   ++pos;
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
void IdNameGenerator::Rewind( std::string& name )
{
    for( unsigned int pos = 0; pos < name.length(); ++pos )
    {
        if( name[ pos ] >= '0' && name[ pos ] <= '9' )
            name[ pos ] = '0';
        else
            if( pos == name.length() - 1 )
                name[ pos ] = 'z';
            else
                name[ pos ] = 'a';
    }
}