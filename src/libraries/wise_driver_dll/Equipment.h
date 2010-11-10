// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Equipment_h_
#define __Equipment_h_

#include "protocol/Simulation.h"
#include <list>

class CWISEAttributeGroup;
class IWISEStringCache;

// =============================================================================
/** @class  Equipment
    @brief  Equipment
*/
// Created: SEB 2010-10-13
// =============================================================================
class Equipment
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Equipment( const MsgsSimToClient::EquipmentDotations::EquipmentDotation& message );
    virtual ~Equipment();
    //@}

    //! @name Operations
    //@{
    void Update( const MsgsSimToClient::EquipmentDotations::EquipmentDotation& message );
    void AddAttributeGroup( std::list< CWISEAttributeGroup >& list, IWISEStringCache& cache ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Equipment( const Equipment& );            //!< Copy constructor
    Equipment& operator=( const Equipment& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const unsigned long type_;
    unsigned long available_;
    unsigned long unAvailable_;
    unsigned long repairable_;
    unsigned long inMaintenance_;
    unsigned long captured_;
    //@}
};

#endif // __Equipment_h_
