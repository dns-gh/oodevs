// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionItem_ABC_h_
#define __AfterActionItem_ABC_h_

#include "clients_kernel/Resolver_ABC.h"
class AfterActionBuilder_ABC;

namespace xml
{
    class xistream;
    class xostream;
}

// =============================================================================
/** @class  AfterActionItem_ABC
    @brief  After action item definition
*/
// Created: AGE 2007-09-19
// =============================================================================
class AfterActionItem_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionItem_ABC() {};
    virtual ~AfterActionItem_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Commit( xml::xostream& xos ) const = 0;

    virtual void Connect   ( xml::xistream& xis, kernel::Resolver_ABC< AfterActionItem_ABC, std::string >& items ) = 0;
    virtual void Connect   ( int input, AfterActionItem_ABC& rhs ) = 0;
    virtual bool CanConnect( int input, const AfterActionItem_ABC* = 0 ) const = 0;
    virtual void Build     ( AfterActionBuilder_ABC& builder ) const = 0;
    virtual void Disconnect( AfterActionItem_ABC* item ) = 0;
    virtual void Disconnect( AfterActionItem_ABC* item, int input ) = 0;

    virtual void ConnectOutput( AfterActionItem_ABC& rhs, int input ) = 0;
    virtual bool IsCompatible( const std::string& type, const AfterActionItem_ABC* connection ) const = 0;
    virtual std::string Resolve( const AfterActionItem_ABC* connection ) const = 0;
    virtual std::string Resolve( int input, const AfterActionItem_ABC* ignore ) const = 0;
    //@}
};

#endif // __AfterActionItem_ABC_h_
