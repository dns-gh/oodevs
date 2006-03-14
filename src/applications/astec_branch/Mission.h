// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Mission_h_
#define __Mission_h_

#include "Resolver.h"
class FragOrder;

// =============================================================================
/** @class  Mission
    @brief  Mission
*/
// Created: AGE 2006-03-14
// =============================================================================
class Mission : public Resolver< FragOrder >
{

public:
    //! @name Constructors/Destructor
    //@{
             Mission( const std::string& name, unsigned int id, bool automat );
    virtual ~Mission();
    //@}

    //! @name Operations
    //@{
    void AddAvailableOrder( FragOrder& order );
    //@}

public:
    //! @name Member data
    //@{
    std::string name_;
    unsigned int id_;
    bool automat_;
    //@}
};

#endif // __Mission_h_
