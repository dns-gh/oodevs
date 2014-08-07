// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Identifier_h_
#define __Identifier_h_

#include "Parameter.h"

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Identifier
    @brief  Identifier
*/
// Created: JSR 2010-04-15
// =============================================================================
class Identifier : public Parameter< unsigned int >
{
public:
    //! @name Functors
    //@{
    typedef std::function< void ( const unsigned int& ) > T_Setter;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit Identifier( const kernel::OrderParameter& parameter );
             Identifier( const kernel::OrderParameter& parameter, unsigned int value );
    virtual ~Identifier();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    void CommitTo( T_Setter setter ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

}
}

#endif // __Identifier_h_
