// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_FormationCreater_h
#define plugins_hla_FormationCreater_h

#include "tools/Resolver_ABC.h"
#include <boost/noncopyable.hpp>

namespace dispatcher
{
    class Team_ABC;
}

namespace sword
{
    class FormationCreation;
}

namespace plugins
{
namespace hla
{
    template< typename ResponseMessage > class ContextHandler_ABC;

// =============================================================================
/** @class  FormationCreater
    @brief  Formation creater
*/
// Created: SLI 2011-09-13
// =============================================================================
class FormationCreater : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             FormationCreater( const tools::Resolver_ABC< dispatcher::Team_ABC >& sides,
                               ContextHandler_ABC< sword::FormationCreation >& contextHandler );
    virtual ~FormationCreater();
    //@}
};

}
}

#endif // FormationCreater_h
