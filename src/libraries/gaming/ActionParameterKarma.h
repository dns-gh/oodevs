// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterKarma_h_
#define __ActionParameterKarma_h_

#include "ActionParameter.h"

namespace kernel
{
    class Karma;
}

// =============================================================================
/** @class  ActionParameterKarma
    @brief  ActionParameterKarma
*/
// Created: SBO 2007-10-24
// =============================================================================
class ActionParameterKarma : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterKarma( const kernel::OrderParameter& parameter, const kernel::Karma& karma );
             ActionParameterKarma( const kernel::OrderParameter& parameter, const ASN1T_EnumDiplomacy& asn );
             ActionParameterKarma( const kernel::OrderParameter& parameter, xml::xistream& xis );
    virtual ~ActionParameterKarma();
    //@}

    //! @name Operations
    //@{
    void CommitTo( ASN1T_EnumDiplomacy& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterKarma( const ActionParameterKarma& );            //!< Copy constructor
    ActionParameterKarma& operator=( const ActionParameterKarma& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Karma& karma_;
    //@}
};

#endif // __ActionParameterKarma_h_
