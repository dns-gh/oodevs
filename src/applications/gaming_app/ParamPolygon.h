// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPolygon_h_
#define __ParamPolygon_h_

#include "ParamLocation.h"

// =============================================================================
/** @class  ParamPolygon
    @brief  ParamPolygon
*/
// Created: SBO 2007-03-14
// =============================================================================
class ParamPolygon : public ParamLocation
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamPolygon( const kernel::OrderParameter& parameter, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ParamPolygon();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}
};

#endif // __ParamPolygon_h_
