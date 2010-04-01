// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_FieldVariant_h_
#define __crossbow_FieldVariant_h_

#pragma warning( push )
#pragma warning( disable : 4512 4702 )
#include <boost/variant.hpp>
#pragma warning( pop )

namespace plugins
{
namespace crossbow
{
    typedef boost::variant< int, long, std::string, bool > FieldVariant;
}
}

#endif // __crossbow_FieldVariant_h_
