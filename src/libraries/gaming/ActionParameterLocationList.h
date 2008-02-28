// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLocationList_h_
#define __ActionParameterLocationList_h_

#include "game_asn/Simulation.h"
#include "ActionParameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

// =============================================================================
/** @class  ActionParameterLocationList
    @brief  ActionParameterLocationList
*/
// Created: SBO 2007-04-20
// =============================================================================
class ActionParameterLocationList : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterLocationList( const kernel::OrderParameter& parameter );
             ActionParameterLocationList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_LocationList& asn );
             ActionParameterLocationList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~ActionParameterLocationList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    void CommitTo( ASN1T_LocationList& asn ) const;
    void Clean( ASN1T_LocationList& asn ) const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterLocationList( const ActionParameterLocationList& );            //!< Copy constructor
    ActionParameterLocationList& operator=( const ActionParameterLocationList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadLocation( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    //@}
};

#endif // __ActionParameterLocationList_h_
