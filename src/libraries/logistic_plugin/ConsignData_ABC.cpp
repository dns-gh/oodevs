// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "ConsignData_ABC.h"
#include "ConsignWriter.h"
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>
#include <sstream>

using namespace plugins::logistic;

ConsignData_ABC::ConsignData_ABC( LogisticPlugin::E_LogisticType type, const std::string& requestId )
    : type_( type )
    , requestId_( requestId )
{
}

ConsignData_ABC::~ConsignData_ABC()
{
}

bool ConsignData_ABC::UpdateConsign( const sword::SimToClient& msg,
        const NameResolver_ABC& names, int tick, const std::string& time,
        std::vector< uint32_t >& entities )
{
    tick_ = boost::lexical_cast< std::string >( tick );
    simTime_ = time;
    entry_.set_tick( tick );
    return DoUpdateConsign( msg, names, entities );
}

LogisticPlugin::E_LogisticType ConsignData_ABC::GetType() const
{
    return type_;
}

int ConsignData_ABC::GetTick() const
{
    if( tick_.empty() )
        return 0;
    return std::atoi( tick_.c_str() );
}

std::string ConsignData_ABC::ToString() const
{
    ConsignWriter w;
    WriteConsign( w );
    return w.GetLine();
}

const sword::LogHistoryEntry& ConsignData_ABC::GetHistoryEntry() const
{
    return entry_;
}
