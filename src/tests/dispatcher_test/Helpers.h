// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Helpers_h_
#define __Helpers_h_

#include "protocol/ClientSenders.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace kernel
{
    class DecisionalModel;
    class AgentType;
    class AutomatType;
}

// =============================================================================
/** @class  Helpers
    @brief  Helpers
*/
// Created: SBO 2010-06-09
// =============================================================================
namespace sword
{
    inline bool operator==( const sword::SimToClient& lhs, const sword::SimToClient& rhs )
    {
        BOOST_CHECK_EQUAL( lhs.DebugString(), rhs.DebugString() );
        return true;
    }
}

namespace dispatcher_test
{
    class StaticModel
    {
    public:
        boost::shared_ptr< kernel::DecisionalModel > MakeDecisionalModel();
        boost::shared_ptr< kernel::AgentType > MakeAgentType();
        boost::shared_ptr< kernel::AutomatType > MakeAutomatType();
    private:
        std::vector< boost::shared_ptr< kernel::DecisionalModel > > decisionalModels_;
        std::vector< boost::shared_ptr< kernel::AgentType > > agents_;
    };
}

#endif // __Helpers_h_
