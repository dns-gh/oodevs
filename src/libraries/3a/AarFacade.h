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

#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}
namespace dispatcher
{
    class ClientPublisher_ABC;
}

class Task;
class FunctionFactory;

// =============================================================================
/** @class  AarFacade
    @brief  AarFacade
*/
// Created: AGE 2008-08-04
// =============================================================================
class AarFacade
{

public:
    //! @name Constructors/Destructor
    //@{
             AarFacade( dispatcher::ClientPublisher_ABC& publisher, int context );
    virtual ~AarFacade();
    //@}

    //! @name Operations
    //@{
    boost::shared_ptr< Task > CreateTask( xml::xistream& xis );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AarFacade( const AarFacade& );            //!< Copy constructor
    AarFacade& operator=( const AarFacade& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< FunctionFactory > factory_;
    //@}
};

#endif // __AarFacade_h_
