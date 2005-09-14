// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-18 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Export/CommunicationLink.h $
// $Author: Sbo $
// $Modtime: 20/07/05 17:27 $
// $Revision: 3 $
// $Workfile: CommunicationLink.h $
//
// *****************************************************************************

#ifndef __TICExport_CommunicationLink_h_
#define __TICExport_CommunicationLink_h_

#include "TIC_Export.h"

namespace TIC
{
    class CommunicationLink;
}

namespace TICExport
{
    class Communication;

// =============================================================================
/** @class  CommunicationLink
    @brief  CommunicationLink
    @par    Using example
    @code
    CommunicationLink;
    @endcode
*/
// Created: SBO 2005-07-11
// =============================================================================
class ENGINE_API CommunicationLink
{
public:
    //! @name Types
    //@{
    enum ESource
    {
        eSourceAutomat,
        eSourcePawn,
        eSourceUnknown
    };
    //@}

public:
	//! @name Constructors/Destructor
    //@{
             CommunicationLink();
             CommunicationLink( unsigned int nIdx );
    virtual ~CommunicationLink();
    //@}

    //! @name Accessors
    //@{
          unsigned int  GetAutomatId         () const;
          unsigned int  GetPawnId            () const;
          unsigned int  GetCommunicationCount( ESource eSource ) const;
    const Communication GetCommunicationAt   ( ESource eSource, unsigned int nIdx ) const;
    //@}

private:
	//! @name Member data
    //@{
    const TIC::CommunicationLink* pLink_;
	//@}
};

} // end namespace TICExport

#endif // __TICExport_CommunicationLink_h_
