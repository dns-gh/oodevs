// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __crossbow_OrderParameterTypeResolver_h_
#define __crossbow_OrderParameterTypeResolver_h_

#include "game_asn/Simulation.h"
#include <map>

namespace plugins
{
namespace crossbow
{
    class OrderParameterTypeResolver
    {
    public:
        OrderParameterTypeResolver& Register( const std::string& type, int value );
        int Resolve( const std::string& type ) const;

    private:
        typedef std::map< std::string, int > T_TypeMap;

    private:
        T_TypeMap map_;
    };
}
}

#endif // __crossbow_OrderParameterSerializer_h_
