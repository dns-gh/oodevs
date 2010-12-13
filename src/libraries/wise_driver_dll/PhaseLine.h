// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PhaseLine_h_
#define __PhaseLine_h_

#include "WiseEntity.h"
#include <list>
#include <wise/wisevec3.h>

namespace MsgsMessengerToClient { class MsgLimaCreation; }

class Model;

// =============================================================================
/** @class  PhaseLine
    @brief  PhaseLine
*/
// Created: SEB 2010-10-27
// =============================================================================
class PhaseLine : public WiseEntity
{

public:
    //! @name Constructors/Destructor
    //@{
             PhaseLine( const Model& model, const MsgsMessengerToClient::MsgLimaCreation& message );
    virtual ~PhaseLine();
    //@}

    //! @name Operations
    //@{
    virtual void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PhaseLine( const PhaseLine& );            //!< Copy constructor
    PhaseLine& operator=( const PhaseLine& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual std::wstring MakeIdentifier() const;
    //@}

private:
    //! @name Member data
    //@{
    const std::wstring name_;
    const Entity_ABC* superior_;
    const std::list< CWISEVec3 > points_;
    //@}
};

#endif // __PhaseLine_h_
