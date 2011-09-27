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
#include "stdlib.h"
#include <iostream>
#include <xeumeuleu/xml.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

using namespace orbat_generator;

namespace
{
    std::string ReadCoordinates( xml::xisubstream xis, const std::string& attribute )
    {
        std::string result;
        xis >> xml::start( "structure" )
                >> xml::attribute( attribute, result );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator constructor
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
IdNameGenerator::IdNameGenerator( xml::xisubstream xis )
    : partyId_    ( 0 )
    , formationId_( 0 )
    , automateId_ ( 0 )
    , pionId_     ( 0 )
	, knowledgeId_( 0 )
	, upperLeft_  ( ReadCoordinates( xis, "point1" ) )
	, lowerRight_ ( ReadCoordinates( xis, "point2" ) )
{
    srand( time( NULL ) );
    std::string firstCoord;
	std::string secondCoord;
	xis >> xml::start( "structure" )
          >> xml::attribute( "point1", firstCoord )
          >> xml::attribute( "point2", secondCoord )
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
// Name: IdNameGenerator::RandomChar
// Created: RCD 2011-04-20
// -----------------------------------------------------------------------------

char IdNameGenerator::RandomChar( char upLeft, char downRight ) const
	{
        if ( upLeft >= downRight )
            return upLeft;
        else
            return ( char( rand() % ( downRight + 1 - upLeft ) + upLeft ) );
	}

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::RandomInt
// Created: RCD 2011-04-20
// -----------------------------------------------------------------------------

std::string IdNameGenerator::RandomInt( unsigned int min, unsigned int max ) const
	{
        std::string  res;
        if ( min >= max )
            res = boost::lexical_cast< std::string >( min );
        else
            res = boost::lexical_cast< std::string >( rand() % ( max - min ) + min );
        unsigned int it = 5;
        while( it > res.size() )
            res = '0' + res;
        return res;
	}
// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputeCoord
// Created: RCD 2011-04-20
// -----------------------------------------------------------------------------

std::string IdNameGenerator::ComputeCoord() const
    {
        std::string prefixe = upperLeft_.prefixe;
        char alphaX = RandomChar( upperLeft_.alphaX, lowerRight_.alphaX );
        char alphaY = RandomChar( upperLeft_.alphaY, lowerRight_.alphaY );
        unsigned int max = 99999;
        unsigned int min = 0;
        if ( alphaX == upperLeft_.alphaX )
            min = upperLeft_.x;
        if ( alphaX == lowerRight_.alphaX )
            max = lowerRight_.x;
        std::string x = RandomInt( min, max );
        if ( alphaY == upperLeft_.alphaY )
            min = upperLeft_.y;
        if ( alphaY == lowerRight_.alphaY )
            max = lowerRight_.y;
        std::string y = RandomInt( min, max );
        return ( prefixe + alphaX + alphaY + x + y );
    }

// -----------------------------------------------------------------------------
// Name: IdNameGenerator::ComputeAutomateId
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
unsigned int IdNameGenerator::ComputeKnowledgeId() const
{
    return ++knowledgeId_;
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