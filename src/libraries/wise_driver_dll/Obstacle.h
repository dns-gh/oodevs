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

#include "WiseEntity.h"
#include <list>
#include <wise/wisevec3.h>

namespace sword { class ObjectCreation; }

class Model;

// =============================================================================
/** @class  Obstacle
    @brief  Obstacle
*/
// Created: SEB 2010-10-27
// =============================================================================
class Obstacle : public WiseEntity
{

public:
    //! @name Constructors/Destructor
    //@{
             Obstacle( const Model& model, const sword::ObjectCreation& message );
    virtual ~Obstacle();
    //@}

    //! @name Operations
    //@{
    virtual void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Obstacle( const Obstacle& );            //!< Copy constructor
    Obstacle& operator=( const Obstacle& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const std::wstring name_;
    const std::wstring type_;
    const Entity_ABC* party_;
    const unsigned char geometry_;
    const std::list< CWISEVec3 > points_;
    //@}
};

#endif // __Obstacle_h_
