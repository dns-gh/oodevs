// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ParameterFactory_ABC_h_
#define __ParameterFactory_ABC_h_

#include "actions/ParameterFactory_ABC.h"
#include "protocol/Protocol.h"

using namespace Common;


// =============================================================================
/** @class  ParameterFactory_ABC
    @brief  ParameterFactory_ABC
*/
// Created: AGE 2008-07-16
// =============================================================================
class ParameterFactory_ABC : public actions::ParameterFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParameterFactory_ABC() {};
    virtual ~ParameterFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Entity_ABC& entity ) const = 0;

    virtual actions::Parameter_ABC* CreateParameter( const kernel::OrderParameter& parameter, const Common::MsgMissionParameter& message, const kernel::Entity_ABC& entity ) const = 0;
    //@}
};

#endif // __ParameterFactory_ABC_h_
