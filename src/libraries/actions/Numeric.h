// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterNumeric_h_
#define __ActionParameterNumeric_h_

#include "Parameter.h"
#include <boost/function.hpp>

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Numeric
    @brief  Numeric
*/
// Created: SBO 2007-05-21
// =============================================================================
class Numeric : public Parameter< float >
{
public:
    //! @name Functors
    //@{
    typedef boost::function< void ( const float& ) > T_Setter;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Numeric( const kernel::OrderParameter& parameter, float value );
             Numeric( const kernel::OrderParameter& parameter, xml::xistream& xis );
    virtual ~Numeric();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    void CommitTo( T_Setter message ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Numeric( const Numeric& );            //!< Copy constructor
    Numeric& operator=( const Numeric& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

    }
}

#endif // __ActionParameterNumeric_h_
