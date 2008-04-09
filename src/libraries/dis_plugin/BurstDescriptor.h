// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BurstDescriptor_h_
#define __BurstDescriptor_h_

#include "hla_plugin/EntityType.h"

namespace dis
{

// =============================================================================
/** @class  BurstDescriptor
    @brief  BurstDescriptor
*/
// Created: AGE 2008-04-08
// =============================================================================
class BurstDescriptor
{

public:
    //! @name Constructors/Destructor
    //@{
             BurstDescriptor();
    virtual ~BurstDescriptor();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BurstDescriptor( const BurstDescriptor& );            //!< Copy constructor
    BurstDescriptor& operator=( const BurstDescriptor& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    EntityType type_;
    unsigned short warhead_;
    unsigned short fuse_;
    unsigned short quantity_;
    unsigned short rate_;
    //@}
};

}

#endif // __BurstDescriptor_h_
