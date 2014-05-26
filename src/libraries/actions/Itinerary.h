// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterItinerary_h_
#define __ActionParameterItinerary_h_

#include "Parameter.h"

namespace sword
{
    class PathfindRequest;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace actions
{
    namespace parameters
    {

// =============================================================================
/** @class  Itinerary
    @brief  Itinerary
*/
// =============================================================================
class Itinerary : public Parameter< QString >
{
public:
    //! @name Constructors/Destructor
    //@{
             Itinerary( const kernel::OrderParameter& parameter,
                        const kernel::CoordinateConverter_ABC& converter );
             Itinerary( const kernel::OrderParameter& parameter,
                        const kernel::CoordinateConverter_ABC& converter,
                        const sword::PathfindRequest& parameters );
    virtual ~Itinerary();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual void Serialize( xml::xostream& xos ) const;
    virtual std::string SerializeType() const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC&    converter_;
    std::unique_ptr< sword::PathfindRequest > request_;
    //@}
};

    }
}

#endif // __ActionParameterItinerary_h_
