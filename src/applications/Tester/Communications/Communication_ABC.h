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
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/Communication_ABC.h $
// $Author: Sbo $
// $Modtime: 20/07/05 18:17 $
// $Revision: 8 $
// $Workfile: Communication_ABC.h $
//
// *****************************************************************************

#ifndef __Communication_ABC_h_
#define __Communication_ABC_h_

#include "Types.h"
#include "Communications/CommunicationManager.h"

namespace TIC
{
    class CommunicationLink;
    class CommunicationType;

// =============================================================================
/** @class  Communication_ABC
    @brief  Communication_ABC
    @par    Using example
    @code
    Communication_ABC;
    @endcode
*/
// Created: SBO 2005-05-24
// =============================================================================
class Communication_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Communication_ABC();
    virtual ~Communication_ABC();
    //@}

    //! @name Accessors
    //@{
            const CommunicationType& GetType      () const;
            //const std::string&       GetSource    () const;
    virtual       uint               GetArity     () const;
    virtual const std::string        GetSubject   () const = 0;
    virtual       bool               IsSameSubject( T_EntityId               nSubjectId ) const = 0;
    virtual       bool               IsSameType   ( const CommunicationType& type       ) const;

    // cr messages
                  bool               IsReport  () const;
    virtual const std::string&       GetMessage() const = 0;
    //@}

    //! @name Modifiers
    //@{
    virtual bool ResetStatus() = 0;
    //@}

protected:
    //! @name Member data
    //@{
    CommunicationLink*             pLink_;
    const CommunicationType*       pType_;
    CommunicationManager::ESource  eSource_;
          uint                     nArity_;
    //@}
};

} // end namespace TIC

#include "Communication_ABC.inl"

#endif // __Communication_ABC_h_
