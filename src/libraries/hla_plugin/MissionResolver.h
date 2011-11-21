// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MissionResolver_h
#define plugins_hla_MissionResolver_h

#include "MissionResolver_ABC.h"
#include <map>

namespace tools
{
    template< typename T, typename Identifier > class Resolver_ABC;
}

namespace kernel
{
    class MissionType;
    class FragOrderType;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  MissionResolver
    @brief  Mission resolver
*/
// Created: SLI 2011-10-10
// =============================================================================
class MissionResolver : public MissionResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MissionResolver( const tools::Resolver_ABC< kernel::MissionType, unsigned long >& missions,
                              const tools::Resolver_ABC< kernel::FragOrderType, unsigned long >& fragOrders );
    virtual ~MissionResolver();
    //@}

    //! @name Operations
    //@{
    virtual unsigned int ResolveAutomat( const std::string& name ) const;
    virtual unsigned int ResolveUnit( const std::string& name ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< unsigned int, unsigned int > T_Mission;
    typedef std::map< std::string, T_Mission > T_Missions;
    //@}

private:
    //! @name Member data
    //@{
    T_Missions missions_;
    //@}
};

}
}

#endif // plugins_hla_MissionResolver_h
