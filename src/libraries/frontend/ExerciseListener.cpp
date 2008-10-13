// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "ExerciseListener.h"
#include <string.h> 
#pragma warning( push, 0 )
#include <boost/asio.hpp> 
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#pragma warning( pop )

const size_t max_length = 1024;

using namespace frontend; 

// -----------------------------------------------------------------------------
// Name: ExerciseListener constructor
// Created: RDS 2008-07-23
// -----------------------------------------------------------------------------
ExerciseListener::ExerciseListener( const std::string& strHost, unsigned int nPort )
    : strHost_( strHost )
    , nPort_ ( nPort)
    , percentage_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseListener destructor
// Created: RDS 2008-07-23
// -----------------------------------------------------------------------------
ExerciseListener::~ExerciseListener()
{
	// NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseListener::Wait
// Created: RDS 2008-07-23
// -----------------------------------------------------------------------------
bool ExerciseListener::Wait()
{
    percentage_ = 0;
    for(;;)
	{
        boost::asio::ip::tcp::iostream stream( strHost_, boost::lexical_cast< std::string >( nPort_ ) );
		if( stream.good() )
		{
            unsigned int count = 5 ; //!< tick count
            unsigned int lineCount = 0;
            while( count > 0 && stream.good() )
            {
                std::string line;
                std::getline( stream, line );
                ++lineCount;
                percentage_ = lineCount * 100 / 300;
                if( line.find( "tick" ) != std::string::npos )
                    --count;
    		}
            return true;
        }
        else
        {
	        boost::xtime t;
	        boost::xtime_get( &t, boost::TIME_UTC );
            t.nsec += 1;
	        boost::thread::sleep( t );
        }
	}
    return false;
}

// -----------------------------------------------------------------------------
// Name: ExerciseListener::GetPercentage
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
unsigned int ExerciseListener::GetPercentage() const
{
    return percentage_;
}
