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
        const NameResolver_ABC& names, int tick, const std::string& time )
{
    tick_ = boost::lexical_cast< std::string >( tick );
    simTime_ = time;
    return DoUpdateConsign( msg, names );
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

ConsignState& ConsignData_ABC::PushState()
{
    const int tick = GetTick();
    ConsignState state;
    if( !history_.empty() )
    {
        // There is no undefined state, each new state terminates the previous one
        history_.back().endTick_ = tick;
        state = history_.back();
        state.id_++;
    }
    else
    {
        state.id_ = 1;
    }
    state.startTick_ = tick;
    state.endTick_ = -1;
    history_.push_back( state );
    return history_.back();
}

const std::deque< ConsignState >& ConsignData_ABC::GetHistory() const
{
    return history_;
}

