// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterFactory_ABC_h_
#define __ActionParameterFactory_ABC_h_

#include <boost/noncopyable.hpp>

namespace sword
{
    class MissionParameter;
    class MissionParameter_Value;
}

namespace kernel
{
    class Entity_ABC;
    class OrderParameter;
}

namespace actions
{

class Parameter_ABC;

// =============================================================================
/** @class  ParameterFactory_ABC
    @brief  ParameterFactory_ABC
*/
// Created: SBO 2007-03-19
// =============================================================================
class ParameterFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ParameterFactory_ABC() {}
    virtual ~ParameterFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter,
                                                     const sword::MissionParameter& message,
                                                     const kernel::Entity_ABC* entity ) const = 0;
    virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter,
                                                     const sword::MissionParameter_Value& message,
                                                     const kernel::Entity_ABC* entity,
                                                     bool nullValue = false ) const = 0;
    //@}
};

} //! namespace actions

#endif // __ActionParameterFactory_ABC_h_
