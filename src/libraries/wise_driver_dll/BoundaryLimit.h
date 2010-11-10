// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __BoundaryLimit_h_
#define __BoundaryLimit_h_

#include "Entity_ABC.h"
#include <list>
#include <wisevec3.h>

namespace MsgsMessengerToClient { class MsgLimitCreation; }

class Model;

// =============================================================================
/** @class  BoundaryLimit
    @brief  BoundaryLimit
*/
// Created: SEB 2010-10-27
// =============================================================================
class BoundaryLimit : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             BoundaryLimit( const Model& model, const MsgsMessengerToClient::MsgLimitCreation& message );
    virtual ~BoundaryLimit();
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
    BoundaryLimit( const BoundaryLimit& );            //!< Copy constructor
    BoundaryLimit& operator=( const BoundaryLimit& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    std::wstring MakeIdentifier() const;
    //@}

private:
    //! @name Member data
    //@{
    const unsigned long id_;
    const std::wstring name_;
    const Entity_ABC* superior_;
    const std::list< CWISEVec3 > points_;
    mutable WISE_HANDLE handle_;
    //@}
};

#endif // __BoundaryLimit_h_
