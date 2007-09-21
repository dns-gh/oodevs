// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_EmergencyShelter_h_
#define __MIL_EmergencyShelter_h_

#include "MIL.h"
#include "MIL_RealObject_ABC.h"

class MIL_Agent_ABC;

// =============================================================================
/** @class  MIL_EmergencyShelter
    @brief  MIL_EmergencyShelter    
*/
// Created: JCR 2007-09-19
// =============================================================================
class MIL_EmergencyShelter : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_EmergencyShelter );

public:
    //! @name Constructors/Destructor
    //@{
             MIL_EmergencyShelter( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_EmergencyShelter();
    virtual ~MIL_EmergencyShelter();
    //@}

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
    //@}
};


#endif // __MIL_EmergencyShelter_h_
