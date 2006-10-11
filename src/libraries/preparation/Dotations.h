// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dotations_h_
#define __Dotations_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Resolver.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Controller;
    class DotationType;
}

namespace xml
{
    class xistream;
}

class Dotation;

// =============================================================================
/** @class  Dotations
    @brief  Dotations
*/
// Created: SBO 2006-10-11
// =============================================================================
class Dotations : public kernel::Extension_ABC
                , public kernel::Resolver< Dotation >
                , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Dotations( kernel::Controller& controller, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver );
             Dotations( xml::xistream& xis, kernel::Controller& controller, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver );
    virtual ~Dotations();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Dotations( const Dotations& );            //!< Copy constructor
    Dotations& operator=( const Dotations& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoSerialize( xml::xostream& xos ) const;
    void ReadDotation( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver_;
    //@}
};

#endif // __Dotations_h_
