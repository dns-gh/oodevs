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

#include "Entity.h"
#include "clients_kernel/Pathfind_ABC.h"

namespace sword
{
    class Pathfind;
}

namespace kernel
{
    class CoordinateConverter_ABC;
    class EntityResolver_ABC;
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
class Itinerary : public Entity< kernel::Pathfind_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Itinerary( const kernel::OrderParameter& parameter,
                        const kernel::CoordinateConverter_ABC& converter,
                        kernel::Controller& controller );
             Itinerary( const kernel::OrderParameter& parameter,
                        const kernel::CoordinateConverter_ABC& converter,
                        const sword::Pathfind& pathfind,
                        const kernel::EntityResolver_ABC& resolver,
                        kernel::Controller& controller );
    virtual ~Itinerary();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& message ) const;
    virtual void Serialize( xml::xostream& xos ) const;
    virtual void Accept( ParameterVisitor_ABC& visitor ) const;
    virtual std::string SerializeType() const;
    virtual bool IsSet() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    std::unique_ptr< sword::Pathfind > pathfind_;
    //@}
};

    }
}

#endif // __ActionParameterItinerary_h_
