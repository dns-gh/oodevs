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
#include "Parameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  LocationList
    @brief  LocationList
*/
// Created: SBO 2007-04-20
// =============================================================================
class LocationList : public Parameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LocationList( const kernel::OrderParameter& parameter );
             LocationList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_LocationList& asn );
             LocationList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~LocationList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    void CommitTo( ASN1T_LocationList& asn ) const;
    void Clean( ASN1T_LocationList& asn ) const;
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
    LocationList( const LocationList& );            //!< Copy constructor
    LocationList& operator=( const LocationList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadLocation( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    //@}
};

    }
}

#endif // __ActionParameterLocationList_h_
