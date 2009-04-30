// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Score.h"
#include "Services.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_gui/Tools.h"
#include "game_asn/AarSenders.h"
#include <boost/algorithm/string.hpp>

namespace
{
    QString ExtractRoot( const QString& name )
    {
        return name.section( '/', 0, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: Score constructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
Score::Score( const ASN1T_MsgIndicator& message, kernel::Controller& controller, Publisher_ABC& publisher )
    : controller_( controller )
    , publisher_( publisher )
    , name_( ExtractRoot( message.name ) )
    , value_( 0 )
    , tendency_( 0 )
    , normalized_( 0 )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Score destructor
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
Score::~Score()
{
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: Score::GetName
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
QString Score::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Score::Update
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void Score::Update( const ASN1T_MsgIndicator& message )
{
    if( boost::ends_with( message.name, "/Tendency" ) )
        tendency_ = message.value;
    else if( boost::ends_with( message.name, "/Gauge" ) )
        normalized_ = message.value;
    else
        value_ = message.value;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Score::Display
// Created: SBO 2009-03-12
// -----------------------------------------------------------------------------
void Score::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Score", "Name" ), name_ )
             .Display( tools::translate( "Score", "Value" ), value_ )
             .Display( tools::translate( "Score", "Tendency" ), tendency_ )
             .Display( tools::translate( "Score", "Gauge" ), normalized_ );
}

// -----------------------------------------------------------------------------
// Name: Score::Commit
// Created: SBO 2009-04-30
// -----------------------------------------------------------------------------
std::string Score::Commit( const T_Parameters& ) const
{
    return "indicator://" + name_;
}
