// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Personnel_h_
#define __Personnel_h_

#include "protocol/Simulation.h"
#include <list>

class CWISEAttributeGroup;
class IWISEStringCache;

// =============================================================================
/** @class  Personnel
    @brief  Personnel
*/
// Created: SEB 2010-10-13
// =============================================================================
class Personnel
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Personnel( const sword::HumanDotations::HumanDotation& message );
    virtual ~Personnel();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::HumanDotations::HumanDotation& message );
    void AddAttributeGroup( std::list< CWISEAttributeGroup >& list, IWISEStringCache& cache ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Personnel( const Personnel& );            //!< Copy constructor
    Personnel& operator=( const Personnel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const unsigned long rank_;
    unsigned long total_;
    unsigned long available_;
    unsigned long dead_;
    unsigned long wounded_;
    unsigned long mentalDiseased_;
    unsigned long contaminated_;
    unsigned long inTreatment_;
    unsigned long assignedToMaintenance_;
    //@}
};

#endif // __Personnel_h_
