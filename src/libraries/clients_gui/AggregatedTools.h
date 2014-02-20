// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AggregatedTools_h_
#define __AggregatedTools_h_

#include <boost/function.hpp>

namespace kernel
{
    class Entity_ABC;
}

bool IsAggregated( const kernel::Entity_ABC& entity );
bool HasAggregatedSubordinate( const kernel::Entity_ABC& entity, boost::function< bool( const kernel::Entity_ABC& ) > fun );
bool HasAggregatedSubordinate( const kernel::Entity_ABC& entity );

#endif // __AggregatedTools_h_
