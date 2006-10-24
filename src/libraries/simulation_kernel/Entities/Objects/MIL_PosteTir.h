//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_PosteTir.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:20 $
// $Revision: 5 $
// $Workfile: MIL_PosteTir.h $
//
//*****************************************************************************

#ifndef __MIL_PosteTir_h_
#define __MIL_PosteTir_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_PosteTir : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_PosteTir );

public:
             MIL_PosteTir( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_PosteTir();
    virtual ~MIL_PosteTir();

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

#include "MIL_PosteTir.inl"

#endif // __MIL_PosteTir_h_
