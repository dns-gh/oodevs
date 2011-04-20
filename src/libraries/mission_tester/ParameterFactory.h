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
#include <vector>

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
    //! @name Helpers
    //@{
    std::auto_ptr< actions::Parameter_ABC > CreatePhaselineParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreateLimitParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreatePointParameter( const kernel::OrderParameter& parameter ) const;
    std::auto_ptr< actions::Parameter_ABC > CreatePathParameter( const kernel::OrderParameter& parameter ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< std::string > T_Points;
    typedef T_Points::const_iterator CIT_Points;
    //@}

    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    T_Points points_;
    //@}
};
}

#endif // __ParameterFactory_h_
