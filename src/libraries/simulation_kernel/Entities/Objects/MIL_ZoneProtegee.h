//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneProtegee.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:31 $
// $Revision: 5 $
// $Workfile: MIL_ZoneProtegee.h $
//
//*****************************************************************************

#ifndef __MIL_ZoneProtegee_h_
#define __MIL_ZoneProtegee_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ZoneProtegee : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_ZoneProtegee );

public:
             MIL_ZoneProtegee( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_ZoneProtegee();
    virtual ~MIL_ZoneProtegee();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
    //@}

private:
    //! @name Events
    //@{
    virtual void ProcessAgentExiting ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentInside  ( MIL_Agent_ABC& agent );
    //@}
};

#include "MIL_ZoneProtegee.inl"

#endif // __MIL_ZoneProtegee_h_
