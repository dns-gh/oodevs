// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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
             Enumeration( const kernel::OrderParameter& parameter, xml::xistream& xis );
             Enumeration( const kernel::OrderParameter& parameter, unsigned int value );
    virtual ~Enumeration();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( Common::MsgMissionParameter& message ) const;
    virtual void CommitTo( Common::MsgMissionParameter_Value& message ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Enumeration( const Enumeration& );            //!< Copy constructor
    Enumeration& operator=( const Enumeration& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameterValue value_;
    //@}
};

    }
}

#endif // __ActionParameterEnumeration_h_
