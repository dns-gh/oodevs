// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterBool_h_
#define __ActionParameterBool_h_

#include "Parameter.h"
#include <boost/function.hpp>

namespace actions
{
namespace parameters
{
// =============================================================================
/** @class  Bool
    @brief  Bool
*/
// Created: SBO 2007-05-21
// =============================================================================
class Bool : public Parameter< bool >
{
public:
    //! @name Functors
    //@{
    typedef boost::function< void ( const bool& ) > T_Setter;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit Bool( const kernel::OrderParameter& parameter );
             Bool( const kernel::OrderParameter& parameter, bool value );
    virtual ~Bool();
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

#endif // __ActionParameterBool_h_
