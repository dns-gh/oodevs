// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterEnumeration_h_
#define __ActionParameterEnumeration_h_

#include "Parameter.h"

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Enumeration
    @brief  Enumeration
*/
// Created: SBO 2007-05-21
// =============================================================================
class Enumeration : public Parameter< std::string >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Enumeration( const kernel::OrderParameter& parameter );
             Enumeration( const kernel::OrderParameter& parameter, unsigned int value );
    virtual ~Enumeration();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
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
    kernel::OrderParameterValue* value_;
    //@}
};

}
}

#endif // __ActionParameterEnumeration_h_
