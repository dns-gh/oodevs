// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterAutomat_h_
#define __ActionParameterAutomat_h_

#include "Entity.h"
#include "clients_kernel/Automat_ABC.h"
#include <boost/function.hpp>

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Automat
    @brief  Automat
*/
// Created: SBO 2007-05-22
// =============================================================================
class Automat : public Entity< kernel::Automat_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Automat( const kernel::OrderParameter& parameter, kernel::Controller& controller );
             Automat( const kernel::OrderParameter& parameter, const kernel::Automat_ABC& automat, kernel::Controller& controller );
             Automat( const kernel::OrderParameter& parameter, unsigned int id, const tools::Resolver_ABC< kernel::Automat_ABC >& resolver, kernel::Controller& controller );
             Automat( const kernel::OrderParameter& parameter, xml::xistream& xis, const tools::Resolver_ABC< kernel::Automat_ABC >& resolver, kernel::Controller& controller );
             Automat( xml::xistream& xis, const tools::Resolver_ABC< kernel::Automat_ABC >& resolver, kernel::Controller& controller );
    virtual ~Automat();
    //@}

    //! @name Operations
    //@{
    using Entity< kernel::Automat_ABC >::CommitTo;
    virtual void CommitTo( Common::MsgMissionParameter& message ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Automat( const Automat& );            //!< Copy constructor
    Automat& operator=( const Automat& ); //!< Assignment operator
    //@}
};

    }
}

#endif // __ActionParameterAutomat_h_
