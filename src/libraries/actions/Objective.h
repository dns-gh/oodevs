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
#include "Parameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  Objective
    @brief  Objective
*/
// Created: SBO 2007-05-14
// =============================================================================
class Objective : public Parameter< QString >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Objective( const kernel::OrderParameter& parameter );
             Objective( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
             Objective( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
             Objective( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_MissionObjective& asn );
    virtual ~Objective();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    void CommitTo( ASN1T_MissionObjective& asn ) const;
    void Clean( ASN1T_MissionObjective& asn ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Objective( const Objective& );            //!< Copy constructor
    Objective& operator=( const Objective& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    //@}
};

    }
}

#endif // __ActionParameterObjective_h_
