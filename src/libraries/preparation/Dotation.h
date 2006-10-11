// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dotation_h_
#define __Dotation_h_

#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class DotationType;
}

namespace xml
{
    class xistream;
    class xostream;
}

// =============================================================================
/** @class  Dotation
    @brief  Dotation
*/
// Created: SBO 2006-10-11
// =============================================================================
class Dotation
{

public:
    //! @name Constructors/Destructor
    //@{
             Dotation( const kernel::DotationType& type, unsigned int quantity );
             Dotation( xml::xistream& xis, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver );
    virtual ~Dotation();
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    //@}

public:
    //! @name Member data
    //@{
    const kernel::DotationType* type_;
    unsigned int quantity_;
    //@}
};

#endif // __Dotation_h_
