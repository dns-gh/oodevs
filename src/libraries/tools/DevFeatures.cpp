// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "tools_pch.h"
#include "DevFeatures.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>
#include <boost/assign.hpp>
#pragma warning( pop )

using namespace tools;

DevFeatures::DevFeatures()
{
    // NOTHING
}

DevFeatures::~DevFeatures()
{
    // NOTHING
}

DevFeatures& DevFeatures::Instance()
{
    static DevFeatures instance;
    return instance;
}

void DevFeatures::Initialize( const std::string& features )
{
    features_ = SplitFeatures( features );
}

bool DevFeatures::HasFeature( const std::string& feature ) const
{
    return features_.count( feature ) > 0;
}

std::unordered_set< std::string > tools::SplitFeatures( const std::string& s )
{
    std::vector< std::string > features;
    boost::algorithm::split( features, s, boost::algorithm::is_any_of( ";" ) );
    std::unordered_set< std::string > featuresSet;
    for( auto it = features.begin(); it != features.end(); ++it )
    {
        auto f = *it;
        boost::algorithm::trim( f );
        if( !f.empty() )
            featuresSet.insert( f ); 
    }
    return featuresSet;
}

std::string tools::JoinFeatures( const std::unordered_set< std::string >& features )
{
    std::vector< std::string > sorted;
    for( auto it = features.begin(); it != features.end(); ++it )
    {
        auto f = *it;
        boost::algorithm::trim( f );
        if( !f.empty() )
            sorted.push_back( f );
    }
    std::sort( sorted.begin(), sorted.end() );
    return boost::algorithm::join( sorted, ";" );
}

const std::vector< std::string >& tools::GetAvailableFeatures()
{
    // Fill this with experimental feature switches
    static const std::vector< std::string > features =
        boost::assign::list_of< std::string >( "pathfind" );
    return features;
}
