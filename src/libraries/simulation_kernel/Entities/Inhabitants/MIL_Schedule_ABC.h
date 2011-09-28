// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Schedule_ABC_h
#define __MIL_Schedule_ABC_h

#include <boost/noncopyable.hpp>

namespace client
{
    class PopulationUpdate;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  MIL_Schedule_ABC
    @brief  MIL schedule declaration
*/
// Created: LGY 2011-01-19
// =============================================================================
class MIL_Schedule_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Schedule_ABC() {}
    virtual ~MIL_Schedule_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Configure( xml::xistream& xis ) = 0;
    virtual void Update( unsigned int date, unsigned int duration ) = 0;
    virtual bool IsMoving() const = 0;
    virtual void RestartLastEvent() = 0;
    virtual void SendFullState( client::PopulationUpdate& msg ) const = 0;
    virtual void UpdateNetwork( client::PopulationUpdate& msg ) const = 0;
    //@}
};

#endif // __MIL_Schedule_ABC_h
