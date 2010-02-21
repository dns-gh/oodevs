// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterDotationType_h_
#define __ActionParameterDotationType_h_

#include "Parameter.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class DotationType;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  DotationType
    @brief  DotationType
*/
// Created: SBO 2007-05-21
// =============================================================================
class DotationType : public Parameter< std::string >
{

public:
    //! @name Constructors/Destructor
    //@{
             DotationType( const kernel::OrderParameter& parameter, unsigned int id, const tools::Resolver_ABC< kernel::DotationType >& resolver );
             DotationType( const kernel::OrderParameter& parameter, xml::xistream& xis, const tools::Resolver_ABC< kernel::DotationType >& resolver );
    virtual ~DotationType();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( Common::MsgMissionParameter& asn ) const;
    virtual void CommitTo( std::string& content ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DotationType( const DotationType& );            //!< Copy constructor
    DotationType& operator=( const DotationType& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DotationType& type_;
    //@}
};
    }
}

#endif // __ActionParameterDotationType_h_
