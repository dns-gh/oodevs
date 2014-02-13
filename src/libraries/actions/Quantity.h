// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Quantity_h_
#define __Quantity_h_

#include "Parameter.h"
#include <boost/function.hpp>

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Quantity
    @brief  Quantity
*/
// Created: JSR 2010-04-14
// =============================================================================
class Quantity : public Parameter< int >
{
public:
    //! @name Functors
    //@{
    typedef boost::function< void ( const int& ) > T_Setter;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Quantity( const kernel::OrderParameter& parameter );
             Quantity( const kernel::OrderParameter& parameter, int value );
    virtual ~Quantity();
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

#endif // __Quantity_h_
