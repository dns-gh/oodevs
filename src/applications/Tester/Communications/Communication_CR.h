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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/Communication_CR.h $
// $Author: Sbo $
// $Modtime: 12/07/05 18:39 $
// $Revision: 6 $
// $Workfile: Communication_CR.h $
//
// *****************************************************************************

#ifndef __Communication_CR_h_
#define __Communication_CR_h_

#include "Types.h"
#include "Communication_ABC.h"
#include "Messages/ASN_Messages.h"

namespace TIC
{
    class Pawn;
    class CommunicationManager;
    class CommunicationLink;

// =============================================================================
/** @class  Communication_CR
    @brief  Communication_CR
    @par    Using example
    @code
    Communication_CR;
    @endcode
*/
// Created: SBO 2005-05-24
// =============================================================================
class Communication_CR : public Communication_ABC
{

public:
    //! @name Operations
    //@{
    static void OnReceiveFragOrder( const ASN1T_MsgOrderConduiteAck& asnMsg );
    static void OnReceivePawnOrder( const ASN1T_MsgPionOrder&        asnMsg );
    static void OnReceiveCR       ( const ASN1T_MsgCR&               asnMsg );
    //@}

    //! @name Accessors
    //@{
    const std::string  GetSubject   () const;
    const std::string& GetMessage   () const;
          bool         IsSameSubject( T_EntityId nSubjectId ) const;
    //@}

    //! @name Modifiers
    //@{
    bool ResetStatus();
    //@}

private:
    //! @name Constructors/Destructor
    //@{
             Communication_CR( CommunicationLink&            link, 
                               T_EntityId                    nSubjectId, 
                               const CommunicationType&      type, 
                               CommunicationManager::ESource eSource );
    virtual ~Communication_CR();
    //@}

private:
    //! @name Member data
    //@{
    T_EntityId  nSubjectId_;
    //@}
};

} // end namespace TIC

#include "Communication_CR.inl"

#endif // __Communication_CR_h_
