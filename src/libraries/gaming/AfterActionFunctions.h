// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionFunctions_h_
#define __AfterActionFunctions_h_

#include "clients_kernel/Resolver_ABC.h"
namespace xml
{
    class xistream;
}
class AfterActionFactory;
class AfterActionFunction;

// =============================================================================
/** @class  AfterActionFunctions
    @brief  AfterActionFunctions
*/
// Created: AGE 2007-09-20
// =============================================================================
class AfterActionFunctions
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AfterActionFunctions( kernel::Resolver_ABC< AfterActionFactory, QString >& factories );
    virtual ~AfterActionFunctions();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& functions );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionFunctions( const AfterActionFunctions& );            //!< Copy constructor
    AfterActionFunctions& operator=( const AfterActionFunctions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadFunction( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::vector< AfterActionFunction* >  T_Functions;
    typedef T_Functions::const_iterator        CIT_Functions;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Resolver_ABC< AfterActionFactory, QString >& factories_;
    T_Functions functions_;
    //@}
};

#endif // __AfterActionFunctions_h_
