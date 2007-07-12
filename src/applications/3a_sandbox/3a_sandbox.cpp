// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "3a_sandbox_pch.h"
#include "Ticker.h"
#include "SimpleValueExtractor_ABC.h"
#include "UnitIdExtractor.h"
#include "AttributeExtractor.h"
#include "dispatcher/Config.h"
#include "dispatcher/MessageLoader.h"
#include "dispatcher/CompositeMessageHandler.h"
#include <iostream>

namespace
{
    struct ValueHandler : public ValueHandler_ABC
    {
        ValueHandler() { values_.reserve( 100 ); }
        virtual void Handle( float value )
        {
            values_.push_back( value );
        }
        void Dump()
        {
            for( std::vector< float >::const_iterator it = values_.begin(); it != values_.end(); ++it )
                std::cout << *it << ", ";
            std::cout << std::endl;
        }
        std::vector< float > values_;
    };
}

int main( int argc, char* argv[] )
{
    dispatcher::Config config;
    config.Parse( argc, argv );

    ValueHandler output;

    dispatcher::CompositeMessageHandler composite;
    
    ValueEqualsExtractor< UnitIdExtractor > unitIdExtractor( 111 );
    AttributeExtractor< Speed > valueExtractor;

    composite.Add( new MessageFilter( unitIdExtractor, unitIdExtractor, valueExtractor ) );
    composite.Add( new Ticker( valueExtractor, output ) );

    dispatcher::MessageLoader loader( config, "20070712T125753" );

    loader.LoadKeyFrame( 0, composite );
    unsigned i = 0;
    while( loader.LoadFrame( i, composite ) )
        ++i;

    output.Dump();

    return 0;
}
