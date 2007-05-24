// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterAutomat_h_
#define __ActionParameterAutomat_h_

#include "ActionParameterEntity.h"

namespace kernel
{
    class Automat_ABC;
}

// =============================================================================
/** @class  ActionParameterAutomat
    @brief  ActionParameterAutomat
*/
// Created: SBO 2007-05-22
// =============================================================================
class ActionParameterAutomat : public ActionParameterEntity< kernel::Automat_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterAutomat( const QString& name );
    explicit ActionParameterAutomat( const kernel::OrderParameter& parameter );
             ActionParameterAutomat( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver );
             ActionParameterAutomat( const QString& name, unsigned int id, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver );
             ActionParameterAutomat( const kernel::OrderParameter& parameter, unsigned int id, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver );
             ActionParameterAutomat( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver );
    virtual ~ActionParameterAutomat();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Accept( ActionParameterVisitor_ABC& visitor ) const;
    void CommitTo( ASN1T_Automate& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterAutomat( const ActionParameterAutomat& );            //!< Copy constructor
    ActionParameterAutomat& operator=( const ActionParameterAutomat& ); //!< Assignment operator
    //@}
};

#endif // __ActionParameterAutomat_h_
