// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "AarToClient.h"
#include "Tools.h"

using namespace shield;

// -----------------------------------------------------------------------------
// Name: AarToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void AarToClient::Convert( const sword::AarInformation& from, MsgsAarToClient::MsgAarInformation* to )
{
    CONVERT( information );
}

// -----------------------------------------------------------------------------
// Name: AarToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void AarToClient::Convert( const sword::PlotResult& from, MsgsAarToClient::MsgPlotResult* to )
{
    CONVERT( identifier );
    CONVERT( error );
    to->mutable_values()->MergeFrom( from.values() );
    CONVERT( begin_tick );
}

// -----------------------------------------------------------------------------
// Name: AarToClient::Convert
// Created: MCO 2010-11-08
// -----------------------------------------------------------------------------
void AarToClient::Convert( const sword::Indicator& from, MsgsAarToClient::MsgIndicator* to )
{
    CONVERT( name );
    CONVERT( value );
}
