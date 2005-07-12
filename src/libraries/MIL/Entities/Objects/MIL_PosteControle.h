//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_PosteControle.h $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 4 $
// $Workfile: MIL_PosteControle.h $
//
//*****************************************************************************

#ifndef __MIL_PosteControle_h_
#define __MIL_PosteControle_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_PosteControle : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_PosteControle );

public:
             MIL_PosteControle();
    virtual ~MIL_PosteControle();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
   //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();
    //@}

private:
    //! @name Events
    //@{
    virtual void ProcessAgentInside( MIL_Agent_ABC& agent );
    //@}
};

#include "MIL_PosteControle.inl"

#endif // __MIL_PosteControle_h_
