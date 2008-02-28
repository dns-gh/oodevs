// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterObjective_h_
#define __ActionParameterObjective_h_

#include "game_asn/Simulation.h"
#include "ActionParameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  ActionParameterObjective
    @brief  ActionParameterObjective
*/
// Created: SBO 2007-05-14
// =============================================================================
class ActionParameterObjective : public ActionParameter< QString >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterObjective( const kernel::OrderParameter& parameter );
             ActionParameterObjective( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
             ActionParameterObjective( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
             ActionParameterObjective( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_MissionObjective& asn );
    virtual ~ActionParameterObjective();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    void CommitTo( ASN1T_MissionObjective& asn ) const;
    void Clean( ASN1T_MissionObjective& asn ) const;
    virtual void Accept( ActionParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterObjective( const ActionParameterObjective& );            //!< Copy constructor
    ActionParameterObjective& operator=( const ActionParameterObjective& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    //@}
};

#endif // __ActionParameterObjective_h_
