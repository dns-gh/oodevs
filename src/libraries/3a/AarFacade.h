// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AarFacade_h_
#define __AarFacade_h_

#include <limits>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace aar
{
    class StaticModel_ABC;
}

class Task;
class FunctionFactory;

// =============================================================================
/** @class  AarFacade
    @brief  After action review facade
*/
// Created: AGE 2008-08-04
// =============================================================================
class AarFacade : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AarFacade( dispatcher::ClientPublisher_ABC& publisher, int context, const aar::StaticModel_ABC& model );
    virtual ~AarFacade();
    //@}

    //! @name Operations
    //@{
    boost::shared_ptr< Task > CreateTask( xml::xistream& xis, unsigned int firstTick = 0, unsigned int lastTick = std::numeric_limits< unsigned int >::max() );
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< FunctionFactory > factory_;
    //@}
};

#endif // __AarFacade_h_
