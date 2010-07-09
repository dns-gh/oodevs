// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FunctionFactory_h_
#define __FunctionFactory_h_

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace xml
{
    class xistream;
}

class Task;
class ElementFactory_ABC;

// =============================================================================
/** @class  FunctionFactory
    @brief  FunctionFactory
*/
// Created: AGE 2007-09-11
// =============================================================================
class FunctionFactory
{
public:
    //! @name Constructors/Destructor
    //@{
             FunctionFactory();
    virtual ~FunctionFactory();
    //@}

    //! @name Operations
    //@{
    void Add( std::auto_ptr< ElementFactory_ABC > factory );

    boost::shared_ptr< Task > CreateTask( xml::xistream& xis );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FunctionFactory( const FunctionFactory& );            //!< Copy constructor
    FunctionFactory& operator=( const FunctionFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateFunction( const std::string& type, xml::xistream& xis, Task& result );
    //@}

private:
    //! @name Member data
    //@{
    boost::ptr_vector< ElementFactory_ABC > factories_;
    //@}
};

#endif // __FunctionFactory_h_
