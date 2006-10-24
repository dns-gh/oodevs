//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneBrouillageBromure.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:24 $
// $Revision: 3 $
// $Workfile: MIL_ZoneBrouillageBromure.h $
//
//*****************************************************************************

#ifndef __MIL_ZoneBrouillageBromure_h_
#define __MIL_ZoneBrouillageBromure_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ZoneBrouillageBromure : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_ZoneBrouillageBromure );

public:
             MIL_ZoneBrouillageBromure( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_ZoneBrouillageBromure();
    virtual ~MIL_ZoneBrouillageBromure();

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

#include "MIL_ZoneBrouillageBromure.inl"

#endif // __MIL_ZoneBrouillageBromure_h_
