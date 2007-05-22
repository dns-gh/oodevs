// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPointList_h_
#define __ActionParameterPointList_h_

#include "ActionParameterLocationList.h"

// =============================================================================
/** @class  ActionParameterPointList
    @brief  ActionParameterPointList
*/
// Created: SBO 2007-05-22
// =============================================================================
class ActionParameterPointList : public ActionParameterLocationList
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterPointList( const kernel::OrderParameter& parameter );
             ActionParameterPointList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_ListPoint& asn );
             ActionParameterPointList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~ActionParameterPointList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterPointList( const ActionParameterPointList& );            //!< Copy constructor
    ActionParameterPointList& operator=( const ActionParameterPointList& ); //!< Assignment operator
    //@}
};

#endif // __ActionParameterPointList_h_
