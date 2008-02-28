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
#include "ActionParameter.h"
#include "clients_kernel/Resolver_ABC.h"

namespace kernel
{
    class Automat_ABC;
}

// =============================================================================
/** @class  ActionParameterAutomatList
    @brief  ActionParameterAutomatList
*/
// Created: SBO 2007-05-23
// =============================================================================
class ActionParameterAutomatList : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterAutomatList( const kernel::OrderParameter& parameter );
             ActionParameterAutomatList( const kernel::OrderParameter& parameter, const ASN1T_AutomatList& asn, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver );
             ActionParameterAutomatList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver );
    virtual ~ActionParameterAutomatList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterAutomatList( const ActionParameterAutomatList& );            //!< Copy constructor
    ActionParameterAutomatList& operator=( const ActionParameterAutomatList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAutomat( xml::xistream& xis, const kernel::Resolver_ABC< kernel::Automat_ABC >& resolver );
    //@}
};

#endif // __ActionParameterAutomatList_h_
