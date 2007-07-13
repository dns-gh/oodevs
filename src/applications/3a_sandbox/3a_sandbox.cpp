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
#include <ctime>

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

    struct MessageCounter : public dispatcher::MessageHandler_ABC
    {
        MessageCounter() : count_( 0 ) {}
        virtual void Receive( const ASN1T_MsgsSimToClient& ) {
            ++count_;
        }
        unsigned count_;
    };

    void Analyse( const dispatcher::Config& config, const std::string& record, dispatcher::CompositeMessageHandler& handler )
    {
        boost::shared_ptr< MessageCounter > counter( new MessageCounter() );
        handler.Add( counter );
        dispatcher::MessageLoader loader( config, record );
        loader.LoadKeyFrame( 0, handler );

        clock_t start = clock();

        unsigned i = 0;
        while( loader.LoadFrame( i, handler ) )
            ++i;

        loader.Synchronize();

        clock_t end = clock();

        std::cout << "Analysed " 
                    << i << " frames, " 
                    << counter->count_ << " messages in " 
                    << float( end - start ) / float( CLOCKS_PER_SEC ) << "s" << std::endl;
    }

    void Test3a( const dispatcher::Config& config, const std::string& record )
    {
        ValueHandler output;
        dispatcher::CompositeMessageHandler composite;
        ValueEqualsExtractor< UnitIdExtractor > unitIdExtractor( 10 );
//        AttributeExtractor< extractors::Heading > valueExtractor;
        AttributeExtractor< extractors::Humans > valueExtractor( 
            extractors::Humans( EnumHumanRank::mdr, &ASN1T_HumanDotations::nb_operationnels ) );
        composite.Add( new MessageFilter( unitIdExtractor, unitIdExtractor, valueExtractor ) );
        composite.Add( new Ticker( valueExtractor, output ) );

        Analyse( config, record, composite );

        output.Dump();
    }
}

#include "dispatcher/Model.h"
#include "dispatcher/SaverFacade.h"
namespace
{
    void GenerateVolume( const dispatcher::Config& config, const std::string& record )
    {
        boost::shared_ptr< dispatcher::Model > model( new dispatcher::Model() );
        boost::shared_ptr< dispatcher::SaverFacade > saver( new dispatcher::SaverFacade( *model, config ) );

        dispatcher::CompositeMessageHandler composite;
        composite.Add( model );
        composite.Add( saver );

        dispatcher::MessageLoader loader( config, record );
        loader.LoadKeyFrame( 0, composite );
        for( unsigned count = 0; count < 1000; ++count )
        {
            unsigned i = 0;
            while( loader.LoadFrame( i, composite ) )
                ++i;
        }
    }
}

namespace 
{
    const char* base = "20070713T100644";
    const char* big  = "20070713T130947";
    const char* huge = "20070713T112811";
}

int main( int argc, char* argv[] )
{
    dispatcher::Config config;
    config.Parse( argc, argv );

    Test3a( config, big );
//    GenerateVolume( config, base );

    return 0;
}
