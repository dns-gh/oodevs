// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __actions_Helpers_h_
#define __actions_Helpers_h_

#include "clients_kernel/Types.h"

#include <string>

#include <boost/function.hpp>

namespace sword
{
    class PointList;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace xml
{
    class xistream;
}

namespace actions
{
    namespace parameters
    {
        typedef boost::function< void( xml::xistream& ) > T_XmlFunctor;

        void FillFromPointList( T_PointVector& vector,
                                const sword::PointList& list,
                                const kernel::CoordinateConverter_ABC& converter );

        void WalkPointList( const std::string& key,
                            const boost::function< void( xml::xistream& ) >& operand,
                            const std::string&,
                            std::string name,
                            xml::xistream& xis );
    }
}

#endif // __actions_Helpers_h_
