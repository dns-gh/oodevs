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

#include "LocationList.h"

namespace Common
{
    class MsgPolygonList;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  PolygonList
    @brief  PolygonList
*/
// Created: SBO 2007-05-22
// =============================================================================
class PolygonList : public LocationList
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PolygonList( const kernel::OrderParameter& parameter );
             PolygonList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const Common::MsgPolygonList& asn );
             PolygonList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~PolygonList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( Common::MsgMissionParameter& asn ) const;
    virtual void Clean( Common::MsgMissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PolygonList( const PolygonList& );            //!< Copy constructor
    PolygonList& operator=( const PolygonList& ); //!< Assignment operator
    //@}
};

    }
}

#endif // __ActionParameterPolygonList_h_
