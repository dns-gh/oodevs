// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Plotter.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/AarSenders.h"

void SendPlotResult( int context, const std::vector< float >& values,
        dispatcher::ClientPublisher_ABC& pub, unsigned int skippedFrames,
        unsigned int firstTick )
{
    aar::PlotResult result;
    result().set_identifier( context );
    result().set_error( "" );
    for( auto it = values.cbegin(); it != values.cend(); ++it )
    {
        if( skippedFrames == 0 )
            result().mutable_values()->Add( *it );
        else
            --skippedFrames;
    }
    if( firstTick != 0 )
        result().set_begin_tick( firstTick );
    result.Send( pub, 0 );
}
