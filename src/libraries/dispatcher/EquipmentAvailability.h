 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EquipmentAvailability_h_
#define __EquipmentAvailability_h_

#include "game_asn/Simulation.h"
#include "ModelRefsContainer.h"

namespace dispatcher
{
class Model;

// =============================================================================
/** @class  EquipmentAvailability
    @brief  EquipmentAvailability
*/
// Created: NLD 2006-09-19
// =============================================================================
template< typename T >
class EquipmentAvailability
{
public:
    //! @name Constructors/Destructor
    //@{
     EquipmentAvailability( Model& model, const T& asn );
    ~EquipmentAvailability();
    //@}

    //! @name Operations
    //@{
    void Send( T& asn ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EquipmentAvailability( const EquipmentAvailability& );            //!< Copy constructor
    EquipmentAvailability& operator=( const EquipmentAvailability& ); //!< Assignement operator
    //@}

private:
    const unsigned int nEquipmentType_;
    const unsigned int nNbrTotal_;
    const unsigned int nNbrAvailable_;
    const unsigned int nNbrWorking_;
    const unsigned int nNbrLent_;
    const unsigned int nNbrResting_;
};

}

#include "EquipmentAvailability.inl"

#endif // __EquipmentAvailability_h_