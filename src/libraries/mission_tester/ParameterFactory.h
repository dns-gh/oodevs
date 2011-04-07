// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ParameterFactory_h_
#define __ParameterFactory_h_

#include <boost/noncopyable.hpp>

namespace actions
{
    class Parameter_ABC;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class OrderParameter;
}

namespace mission_tester
{
// =============================================================================
/** @class  ParameterFactory
    @brief  ParameterFactory
*/
// Created: PHC 2011-04-07
// =============================================================================
class ParameterFactory : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ParameterFactory( const kernel::CoordinateConverter_ABC& converter );
    virtual ~ParameterFactory();
    //@}

    //! @name Operations
    //@{
    std::auto_ptr< actions::Parameter_ABC > CreateParameter( const kernel::OrderParameter& parameter ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};
}

#endif // __ParameterFactory_h_
