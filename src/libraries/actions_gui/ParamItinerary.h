// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef ParamItinerary_h
#define ParamItinerary_h

#include "EntityParameter.h"
#include "clients_kernel/Pathfind_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class OrderParameter;
    class Pathfind_ABC;
}

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamItinerary
    @brief  ParamItinerary
*/
// Created: ABR 2014-05-16
// =============================================================================
class ParamItinerary : public EntityParameter< kernel::Pathfind_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamItinerary( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamItinerary();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void Visit( const actions::parameters::Itinerary& param );
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    //@}
};

    } //! namespace gui
} //! namespace actions

#endif // ParamItinerary_h
