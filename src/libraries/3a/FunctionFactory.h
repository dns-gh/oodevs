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

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace xml
{
    class xistream;
    class xostream;
}

class Task;

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
    boost::shared_ptr< Task > CreateTask( int context, xml::xistream& xis );
    void Describe( xml::xostream& xos );
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

    void Extract( xml::xistream& xis, Task& result );
    template< typename Value >
    void Extract( const std::string& name, xml::xistream& xis, Task& result );

    void Reduce( xml::xistream& xis, Task& result );
    template< typename T >
    void Reduce( const std::string& name, xml::xistream& xis, Task& result );
    template< typename F >
    void ReduceFunction( const std::string& name, xml::xistream& xis, Task& result );

    void Transform( xml::xistream& xis, Task& result );
    template< typename T >
    void Transform( const std::string& name, xml::xistream& xis, Task& result );
    template< typename T >
    void Transform1( const std::string& name, xml::xistream& xis, Task& result );
    template< typename T >
    void Transform2( const std::string& name, xml::xistream& xis, Task& result );

    template< typename T >
    void Compose( const std::string& name, xml::xistream& xis, Task& result );

    void MakeConstant( xml::xistream& xis, Task& result );
    template< typename T >
    void MakeConstant( const std::string& name, xml::xistream& xis, Task& result );

    void Plot( xml::xistream& xis, Task& result );
    template< typename T >
    void Plot( const std::string& name, xml::xistream& xis, Task& result );

    void ValueError( const std::string& name );
    void ReductionError( const std::string& name );
    void TransformationError( const std::string& name );

    struct ReduceDispatcher;
    struct TransformDispatcher;
    struct PlotDispatcher;
    struct MakeConstantDispatcher;
    //@}

private:
    //! @name Member data
    //@{
    int currentContext_;
    //@}
};

#endif // __FunctionFactory_h_
