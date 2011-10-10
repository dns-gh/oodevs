// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_MissionResolver_ABC_h
#define plugins_hla_MissionResolver_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  MissionResolver_ABC
    @brief  MissionResolver definition
*/
// Created: SLI 2011-10-06
// =============================================================================
class MissionResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MissionResolver_ABC() {}
    virtual ~MissionResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual unsigned int Resolve( const std::string& name ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_MissionResolver_ABC_h
