//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZonePoserHelicoptere.h $
// $Author: Jvt $
// $Modtime: 14/04/05 12:35 $
// $Revision: 5 $
// $Workfile: MIL_ZonePoserHelicoptere.h $
//
//*****************************************************************************

#ifndef __MIL_ZonePoserHelicoptere_h_
#define __MIL_ZonePoserHelicoptere_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_Agent_ABC;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ZonePoserHelicoptere : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_ZonePoserHelicoptere );

public:
             MIL_ZonePoserHelicoptere( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_ZonePoserHelicoptere();
    virtual ~MIL_ZonePoserHelicoptere();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
    //@}
};

#include "MIL_ZonePoserHelicoptere.inl"

#endif // __MIL_ZonePoserHelicoptere_h_
