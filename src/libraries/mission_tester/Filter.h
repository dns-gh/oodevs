// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Filter_h_
#define __Filter_h_

#include "Filter_ABC.h"

namespace mission_tester
{
// =============================================================================
/** @class  Filter
    @brief  Filter
*/
// Created: PHC 2011-03-31
// =============================================================================
class Filter : public Filter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Filter( const std::string& criterion );
    virtual ~Filter();
    //@}

    //! @name Operations
    //@{
    virtual bool Accepts( const std::string& criterion ) const;
    //@}

private:
    //! @name Member data
    //@{
    const std::string criterion_;
    //@}
};

}

#endif // __Filter_h_
