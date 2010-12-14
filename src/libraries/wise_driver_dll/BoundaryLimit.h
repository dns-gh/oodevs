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

#include "WiseEntity.h"
#include <list>
#include <wise/wisevec3.h>

namespace sword { class LimitCreation; }

class Model;

// =============================================================================
/** @class  BoundaryLimit
    @brief  BoundaryLimit
*/
// Created: SEB 2010-10-27
// =============================================================================
class BoundaryLimit : public WiseEntity
{

public:
    //! @name Constructors/Destructor
    //@{
             BoundaryLimit( const Model& model, const sword::LimitCreation& message );
    virtual ~BoundaryLimit();
    //@}

    //! @name Operations
    //@{
    virtual void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BoundaryLimit( const BoundaryLimit& );            //!< Copy constructor
    BoundaryLimit& operator=( const BoundaryLimit& ); //!< Assignment operator
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

#endif // __BoundaryLimit_h_
