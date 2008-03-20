// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterObjectiveList_h_
#define __ActionParameterObjectiveList_h_

#include "game_asn/Simulation.h"
#include "ActionParameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class OrderParameter;
}

// =============================================================================
/** @class  ActionParameterObjectiveList
    @brief  ActionParameterObjectiveList
*/
// Created: SBO 2007-05-14
// =============================================================================
class ActionParameterObjectiveList : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterObjectiveList( const kernel::OrderParameter& parameter );
             ActionParameterObjectiveList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
             ActionParameterObjectiveList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_MissionObjectiveList& asn );
    virtual ~ActionParameterObjectiveList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual bool IsSet() const;
    //@}
    
private:
    //! @name Copy/Assignment
    //@{
    ActionParameterObjectiveList( const ActionParameterObjectiveList& );            //!< Copy constructor
    ActionParameterObjectiveList& operator=( const ActionParameterObjectiveList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    //@}
};

#endif // __ActionParameterObjectiveList_h_
