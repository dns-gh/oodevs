// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Drawing_h_
#define __Drawing_h_

#include "WiseEntity.h"
#include <list>
#include <wise/wisevec3.h>

namespace sword { class ShapeCreation; }

class Model;

// =============================================================================
/** @class  Drawing
    @brief  Drawing
*/
// Created: SEB 2011-01-07
// =============================================================================
class Drawing : public WiseEntity
{

public:
    //! @name Constructors/Destructor
    //@{
             Drawing( const Model& model, const sword::ShapeCreation& message );
    virtual ~Drawing();
    //@}

    //! @name Operations
    //@{
    virtual void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Drawing( const Drawing& );            //!< Copy constructor
    Drawing& operator=( const Drawing& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const std::wstring category_;
    const std::wstring color_;
    const std::wstring pattern_;
    const std::list< CWISEVec3 > points_;
    //@}
};

#endif // __Drawing_h_
