// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPointList_h_
#define __ParamPointList_h_

#include "ParamLocationList.h"

// =============================================================================
/** @class  ParamPointList
    @brief  ParamPointList
*/
// Created: SBO 2007-03-15
// =============================================================================
class ParamPointList : public ParamLocationList
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamPointList( QObject* parent, const kernel::OrderParameter& parameter, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ParamPointList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}
};

#endif // __ParamPointList_h_
