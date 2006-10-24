//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneImplantationLRM.h $
// $Author: Jvt $
// $Modtime: 11/05/05 15:11 $
// $Revision: 6 $
// $Workfile: MIL_ZoneImplantationLRM.h $
//
//*****************************************************************************

#ifndef __MIL_ZoneImplantationLRM_h_
#define __MIL_ZoneImplantationLRM_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ZoneImplantationLRM : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_ZoneImplantationLRM );

public:
             MIL_ZoneImplantationLRM( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_ZoneImplantationLRM();
    virtual ~MIL_ZoneImplantationLRM();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
    //@}

protected:
    virtual void ProcessAgentInside ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting( MIL_Agent_ABC& agent );
};

#include "MIL_ZoneImplantationLRM.inl"

#endif // __MIL_ZoneImplantationLRM_h_
