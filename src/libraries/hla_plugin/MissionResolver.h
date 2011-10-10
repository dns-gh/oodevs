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
    explicit MissionResolver( const tools::Resolver_ABC< kernel::MissionType, unsigned long >& missions );
    virtual ~MissionResolver();
    //@}

    //! @name Operations
    //@{
    virtual unsigned int Resolve( const std::string& name ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, unsigned int > T_Missions;
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
