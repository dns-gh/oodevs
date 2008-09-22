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

#include "LocationList.h"

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  PointList
    @brief  PointList
*/
// Created: SBO 2007-05-22
// =============================================================================
class PointList : public LocationList
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PointList( const kernel::OrderParameter& parameter );
             PointList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_PointList& asn );
             PointList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~PointList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PointList( const PointList& );            //!< Copy constructor
    PointList& operator=( const PointList& ); //!< Assignment operator
    //@}
};

    }
}

#endif // __ActionParameterPointList_h_
