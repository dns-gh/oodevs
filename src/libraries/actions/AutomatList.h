// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterAutomatList_h_
#define __ActionParameterAutomatList_h_

#include "game_asn/Simulation.h"
#include "Parameter.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Automat_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  AutomatList
    @brief  AutomatList
*/
// Created: SBO 2007-05-23
// =============================================================================
class AutomatList : public Parameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AutomatList( const kernel::OrderParameter& parameter );
             AutomatList( const kernel::OrderParameter& parameter, const ASN1T_AutomatList& asn, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver );
             AutomatList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver );
    virtual ~AutomatList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual bool IsSet() const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatList( const AutomatList& );            //!< Copy constructor
    AutomatList& operator=( const AutomatList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAutomat( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver );
    //@}
};
    }
}

#endif // __ActionParameterAutomatList_h_
