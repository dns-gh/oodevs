// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef shield_AarToClient_h
#define shield_AarToClient_h

#include "protocol/Protocol.h"
#pragma warning( push, 0 )
#include "proto/AarToClient.pb.h"
#pragma warning( pop )

namespace shield
{
// =============================================================================
/** @class  AarToClient
    @brief  Aar to client converter
*/
// Created: MCO 2010-11-08
// =============================================================================
class AarToClient
{
public:
    //! @name Operations
    //@{
    static void Convert( const sword::AarInformation& from, MsgsAarToClient::MsgAarInformation* to );
    static void Convert( const sword::PlotResult& from, MsgsAarToClient::MsgPlotResult* to );
    static void Convert( const sword::Indicator& from, MsgsAarToClient::MsgIndicator* to );
    //@}
};

}

#endif // shield_AarToClient_h
