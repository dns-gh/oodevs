// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPathList_h_
#define __ActionParameterPathList_h_

#include "game_asn/Asn.h"
#include "ActionParameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class Entity_ABC;
}

// =============================================================================
/** @class  ActionParameterPathList
    @brief  ActionParameterPathList
*/
// Created: SBO 2007-05-21
// =============================================================================
class ActionParameterPathList : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterPathList( const kernel::OrderParameter& parameter );
             ActionParameterPathList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_ListItineraire& asn, const kernel::Entity_ABC& entity );
             ActionParameterPathList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~ActionParameterPathList();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterPathList( const ActionParameterPathList& );            //!< Copy constructor
    ActionParameterPathList& operator=( const ActionParameterPathList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadPath( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    //@}
};

#endif // __ActionParameterPathList_h_
