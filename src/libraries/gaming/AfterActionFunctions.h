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

#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Controller;
}
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
class AfterActionFunctions : public kernel::Resolver< AfterActionFunction, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionFunctions( kernel::Controller& controller, kernel::Resolver_ABC< AfterActionFactory, QString >& factories );
    virtual ~AfterActionFunctions();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& functions );
    AfterActionFunction* Create();
    QString Rename( AfterActionFunction& function, const QString& name );
    void Erase( const AfterActionFunction& function );
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

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Resolver_ABC< AfterActionFactory, QString >& factories_;
    //@}
};

#endif // __AfterActionFunctions_h_
