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

#include "game_asn/Asn.h"
#include "ActionParameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Entity_ABC;
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
             ActionParameterLocationList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_ListLocalisation& asn );
             ActionParameterLocationList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_ListLocalisation& asn, const kernel::Entity_ABC& entity );
    virtual ~ActionParameterLocationList();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterLocationList( const ActionParameterLocationList& );            //!< Copy constructor
    ActionParameterLocationList& operator=( const ActionParameterLocationList& ); //!< Assignment operator
    //@}
};

#endif // __ActionParameterLocationList_h_
