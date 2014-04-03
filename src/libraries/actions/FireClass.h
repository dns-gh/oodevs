// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ActionParameter_FireClass_h_
#define __ActionParameter_FireClass_h_

#include "Parameter.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class FireClass;
}

namespace actions
{
    namespace parameters {

// =============================================================================
/** @class  FireClass
    @brief  FireClass
*/
// Created: LGY 2013-11-26
// =============================================================================
class FireClass : public Parameter< std::string >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FireClass( const kernel::OrderParameter& parameter );
             FireClass( const kernel::OrderParameter& parameter, const std::string name,
                        const tools::StringResolver< kernel::FireClass >& resolver );
    virtual ~FireClass();
    //@}

    //! @name Operations
    //@{
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::FireClass* type_;
    //@}
};

}
}

#endif // __ActionParameter_FireClass_h_
