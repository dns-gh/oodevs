//*****************************************************************************
//
// Name     : LAU_Client_ABC.h
//
// $Created : AML 03-04-01 $
//  $Archive: /MVW_v10/Build/SDK/LAU/src/LAU_Client_ABC.h $
//   $Author: Nld $
//  $Modtime: 5/05/04 10:43 $
// $Revision: 9 $
// $Workfile: LAU_Client_ABC.h $
//
//*****************************************************************************

#ifndef __LAU_Client_ABC_h_
#define __LAU_Client_ABC_h_

#include "LAU.h"


#include "DIN\DIN_UserData_ABC.h"

class DIN::DIN_Link;


//*****************************************************************************
// Object : PUT THE COMMENTS ON THE CLASS HERE
// Created: AML 03-04-01
//*****************************************************************************
class LAU_Client_ABC : public DIN::DIN_UserData_ABC
{
    MT_COPYNOTALLOWED( LAU_Client_ABC )

public:
    explicit                LAU_Client_ABC();
    virtual                 ~LAU_Client_ABC();
    
    //-----------------------------------------------------------------------------
    /** @name Accessors */
    //-----------------------------------------------------------------------------
    //@{
    bool32                  HasSession() const;
    DIN::DIN_Link&          GetSession() const;
    DIN::DIN_StationID      GetStationID() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Main methods */
    //-------------------------------------------------------------------------
    //@{
    void Initialize( DIN::DIN_Link& link );
    void Terminate ();
    void SessionIsNoMoreValid(); // A N'UTILISER QUE DANS CERTAIN CAS - La session a etait fermé (la session ne pointe plus une zone valide)
    //@}

protected:

    DIN::DIN_Link*          pSession_;              // (not owned)
};



#ifdef USE_INLINE
#   include "LAU_Client_ABC.inl"
#endif 

#endif // __LAU_Client_ABC_h_
