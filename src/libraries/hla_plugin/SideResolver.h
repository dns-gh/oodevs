// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_SideResolver_H_
#define plugins_hla_SideResolver_H_

#include "SideResolver_ABC.h"

#include <boost/noncopyable.hpp>
#pragma warning( push, 0 )
#include <boost/bimap.hpp>
#pragma warning( pop )

namespace dispatcher
{
    class Model_ABC;
    class Logger_ABC;
}

namespace plugins
{
namespace hla
{

/// =============================================================================
/// @class hla::SideResolver
/// @brief hla::SideResolver
/// @thread This type is not thread safe
///
/// Created: ahc 7 sept. 2012
/// =============================================================================
class SideResolver : public SideResolver_ABC, private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    SideResolver( dispatcher::Model_ABC& dynamicModel, dispatcher::Logger_ABC& logger );
    virtual ~SideResolver();
    //@}

    //! @name Operations
    //@{
    virtual const dispatcher::Team_ABC& ResolveTeam( rpr::ForceIdentifier f ) const;
    virtual rpr::ForceIdentifier ResolveForce( const dispatcher::Team_ABC& ) const;
    //@}

private:
    //! @name Typedefs
    //@{
    typedef boost::bimap< const dispatcher::Team_ABC*, rpr::ForceIdentifier > T_Teams;
    typedef T_Teams::left_const_iterator CIT_Teams;
    typedef T_Teams::right_const_iterator CIT_Forces;
    //@}

    //! @name Attributes
    //@{
    dispatcher::Logger_ABC& logger_;
    T_Teams teams_;
    //@}

};

}
}
#endif // plugins_hla_SideResolver_H_
