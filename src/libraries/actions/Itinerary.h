// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterItinerary_h_
#define __ActionParameterItinerary_h_

#include "Parameter.h"

namespace sword
{
    class Pathfind;
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
             Itinerary( const kernel::OrderParameter& parameter,
                        const kernel::CoordinateConverter_ABC& converter );
             Itinerary( const kernel::OrderParameter& parameter,
                        const kernel::CoordinateConverter_ABC& converter,
                        const sword::Pathfind& pathfind );
    virtual ~Itinerary();

    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual std::string SerializeType() const;
    virtual bool IsSet() const;
    virtual geometry::Point2f GetPosition() const;

    unsigned int GetId() const;
    const sword::Pathfind& GetPathfind() const;

protected:
    virtual void DisplayInToolTip( kernel::Displayer_ABC& displayer ) const;

private:
    const kernel::CoordinateConverter_ABC& converter_;
    std::unique_ptr< sword::Pathfind > pathfind_;
};

}
}

#endif // __ActionParameterItinerary_h_
