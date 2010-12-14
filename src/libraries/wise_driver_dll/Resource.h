// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Resource_h_
#define __Resource_h_

#include "protocol/Simulation.h"
#include <list>

class CWISEAttributeGroup;
class IWISEStringCache;

// =============================================================================
/** @class  Resource
    @brief  Resource
*/
// Created: SEB 2010-10-13
// =============================================================================
class Resource
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Resource( const sword::ResourceDotations::ResourceDotation& message );
    virtual ~Resource();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::ResourceDotations::ResourceDotation& message );
    void AddAttributeGroup( std::list< CWISEAttributeGroup >& list, IWISEStringCache& cache ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Resource( const Resource& );            //!< Copy constructor
    Resource& operator=( const Resource& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const unsigned long type_;
    unsigned long available_;
    //@}
};

#endif // __Resource_h_
