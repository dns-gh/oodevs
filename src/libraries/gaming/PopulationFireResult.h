// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFireResult_h_
#define __PopulationFireResult_h_

#include <tools/Resolver_ABC.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Population_ABC;
}

namespace sword
{
    class CrowdFireDamages;
}

// =============================================================================
/** @class  PopulationFireResult
    @brief  PopulationFireResult
*/
// Created: AGE 2006-03-10
// =============================================================================
class PopulationFireResult : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    PopulationFireResult( const sword::CrowdFireDamages& message, const tools::Resolver_ABC< kernel::Population_ABC >& resolver, const QDateTime& time, const kernel::Entity_ABC* firer, int id );
    PopulationFireResult( const kernel::Population_ABC& target, const QDateTime& time, const kernel::Entity_ABC* firer, int id );
    ~PopulationFireResult();
    //@}

public:
    //! @name Member data
    //@{
    const kernel::Population_ABC& target_;
    const kernel::Entity_ABC* firer_;
    unsigned int deadPeople_;
    unsigned int woundedPeople_;
    unsigned int scatteredPeople_;
    QDateTime time_;
    const int id_;
    //@}
};

#endif // __PopulationFireResult_h_
