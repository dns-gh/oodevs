// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Obstacle_h_
#define __Obstacle_h_

#include "Entity_ABC.h"
#include <list>
#include <wise/wisevec3.h>

namespace MsgsSimToClient { class MsgObjectCreation; }

class Model;

// =============================================================================
/** @class  Obstacle
    @brief  Obstacle
*/
// Created: SEB 2010-10-27
// =============================================================================
class Obstacle : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Obstacle( const Model& model, const MsgsSimToClient::MsgObjectCreation& message );
    virtual ~Obstacle();
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
    Obstacle( const Obstacle& );            //!< Copy constructor
    Obstacle& operator=( const Obstacle& ); //!< Assignment operator
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
    const std::wstring type_;
    const Entity_ABC* party_;
    const unsigned char geometry_;
    const std::list< CWISEVec3 > points_;
    mutable WISE_HANDLE handle_;
    //@}
};

#endif // __Obstacle_h_
