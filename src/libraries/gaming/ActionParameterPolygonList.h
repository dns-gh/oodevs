// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterPolygonList_h_
#define __ActionParameterPolygonList_h_

#include "ActionParameterLocationList.h"

// =============================================================================
/** @class  ActionParameterPolygonList
    @brief  ActionParameterPolygonList
*/
// Created: SBO 2007-05-22
// =============================================================================
class ActionParameterPolygonList : public ActionParameterLocationList
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterPolygonList( const kernel::OrderParameter& parameter );
             ActionParameterPolygonList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_ListPolygon& asn );
             ActionParameterPolygonList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~ActionParameterPolygonList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterPolygonList( const ActionParameterPolygonList& );            //!< Copy constructor
    ActionParameterPolygonList& operator=( const ActionParameterPolygonList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __ActionParameterPolygonList_h_
