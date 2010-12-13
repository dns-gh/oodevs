// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterFactory_ABC_h_
#define __ActionParameterFactory_ABC_h_

namespace sword
{
    class MsgMissionParameter;
    class MsgMissionParameter_Value;
}

namespace kernel
{
    class Entity_ABC;
    class OrderParameter;
}

namespace xml
{
    class xistream;
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
class ParameterFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ParameterFactory_ABC() {}
    virtual ~ParameterFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Entity_ABC& entity ) const = 0;
    virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis ) const = 0;
    virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, const sword::MsgMissionParameter& message, const kernel::Entity_ABC& entity ) const = 0;
    virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, const sword::MsgMissionParameter_Value& message, const kernel::Entity_ABC& entity ) const = 0;
    //@}
};

}

#endif // __ActionParameterFactory_ABC_h_
