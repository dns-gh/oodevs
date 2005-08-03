// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-11 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/CommunicationManager.h $
// $Author: Sbo $
// $Modtime: 20/07/05 18:24 $
// $Revision: 6 $
// $Workfile: CommunicationManager.h $
//
// *****************************************************************************

#ifndef __CommunicationManager_h_
#define __CommunicationManager_h_

#include "Types.h"
#include "Messages/ASN_Messages.h"

namespace DIN
{
    class DIN_Input;
}

namespace TIC
{
    class Communication_ABC;
    class CommunicationLink;

// =============================================================================
/** @class  CommunicationManager
    @brief  CommunicationManager
    @par    Using example
    @code
    CommunicationManager;
    @endcode
*/
// Created: SBO 2005-05-24
// =============================================================================
class CommunicationManager
{
    MT_COPYNOTALLOWED( CommunicationManager );

public:
    //! @name Enums
    //@{
    enum ESource
    {
        eSourceAutomat,
        eSourcePawn,
        eSourceUnknown
    };
    //@}

public:
    //! @name Operations
    //@{
    static void Initialize();
    static void Terminate ();
    static void Flush     ();

    // Message handlers
    static void OnReceiveMsgUnitInterVisibility  (       DIN::DIN_Input&            input  );
    static void OnReceiveMsgObjectInterVisibility(       DIN::DIN_Input&            input  );
    static void OnReceiveMsgFragOrder            ( const ASN1T_MsgOrderConduiteAck& asnMsg );
    static void OnReceiveMsgPawnOrder            ( const ASN1T_MsgPionOrder&        asnMsg );
    static void OnReceiveMsgCR                   ( const ASN1T_MsgCR&               asnMsg );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             CommunicationManager();
    virtual ~CommunicationManager();
    //@}

};

} // end namespace TIC

#endif // __CommunicationManager_h_
