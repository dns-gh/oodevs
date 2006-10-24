//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneBrouillageBrod.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:23 $
// $Revision: 3 $
// $Workfile: MIL_ZoneBrouillageBrod.h $
//
//*****************************************************************************

#ifndef __MIL_ZoneBrouillageBrod_h_
#define __MIL_ZoneBrouillageBrod_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ZoneBrouillageBrod : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_ZoneBrouillageBrod );

public:
             MIL_ZoneBrouillageBrod( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_ZoneBrouillageBrod();
    virtual ~MIL_ZoneBrouillageBrod();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
    //@}

protected:
    //! @name Events
    //@{
    virtual void ProcessAgentInside ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting( MIL_Agent_ABC& agent );
    //@}
};

#include "MIL_ZoneBrouillageBrod.inl"

#endif // __MIL_ZoneBrouillageBrod_h_
