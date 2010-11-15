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

#include "Parameter.h"

namespace Common
{
    class MsgMissionObjective;
}

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
             Objective( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const Common::MsgMissionObjective& message );
    virtual ~Objective();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( Common::MsgMissionParameter& message ) const;
    virtual void CommitTo( Common::MsgMissionParameter_Value& message ) const;
    void CommitTo( Common::MsgMissionObjective& message ) const;
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
