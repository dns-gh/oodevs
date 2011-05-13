// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterString_h_
#define __ActionParameterString_h_

#include "Parameter.h"

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  String
    @brief  String
*/
// Created: SBO 2007-10-23
// =============================================================================
class String : public Parameter< QString >
{
public:
    //! @name Constructors/Destructor
    //@{
             String( const kernel::OrderParameter& parameter, const std::string& value );
             String( const kernel::OrderParameter& parameter, xml::xistream& xis );
    virtual ~String();
    //@}

    //! @name Operations
    //@{
    void CommitTo( std::string& asn ) const;
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    String( const String& );            //!< Copy constructor
    String& operator=( const String& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{    
    virtual std::string SerializeType() const;
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

    }
}

#endif // __ActionParameterString_h_
