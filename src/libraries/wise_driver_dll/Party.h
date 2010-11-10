// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Party_h_
#define __Party_h_

#include "Entity_ABC.h"

namespace MsgsSimToClient { class MsgPartyCreation; }

class Model;

// =============================================================================
/** @class  Party
    @brief  Party
*/
// Created: SEB 2010-10-13
// =============================================================================
class Party : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Party( const Model& model, const MsgsSimToClient::MsgPartyCreation& message );
    virtual ~Party();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId() const;
    virtual WISE_HANDLE GetHandle() const;
    //@}

    //! @name Operations
    //@{
    virtual void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const;
    virtual void Destroy( CWISEDriver& driver, const WISE_HANDLE& database ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Party( const Party& );            //!< Copy constructor
    Party& operator=( const Party& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    std::wstring name_;
    unsigned char alignment_;
    mutable WISE_HANDLE handle_;
    //@}
};

#endif // __Party_h_
