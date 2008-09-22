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
#include "Parameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class OrderParameter;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  ObjectiveList
    @brief  ObjectiveList
*/
// Created: SBO 2007-05-14
// =============================================================================
class ObjectiveList : public Parameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectiveList( const kernel::OrderParameter& parameter );
             ObjectiveList( const kernel::OrderParameter& parameter, xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
             ObjectiveList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_MissionObjectiveList& asn );
    virtual ~ObjectiveList();
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
    ObjectiveList( const ObjectiveList& );            //!< Copy constructor
    ObjectiveList& operator=( const ObjectiveList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    //@}
};

    }
}

#endif // __ActionParameterObjectiveList_h_
