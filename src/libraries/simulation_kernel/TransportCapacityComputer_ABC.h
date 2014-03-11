// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __transport_TransportCapacityComputer_ABC_H__
#define __transport_TransportCapacityComputer_ABC_H__

class PHY_ComposantePion;

namespace transport
{
// =============================================================================
/** @class  ConsumptionChangeRequestHandler_ABC
    @brief  ConsumptionChangeRequestHandler_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================
class TransportCapacityComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             TransportCapacityComputer_ABC() {}
    virtual ~TransportCapacityComputer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void ApplyOnComposante( PHY_ComposantePion& comp ) = 0;
    //@}
};

}

#endif /* __transport_TransportCapacityComputer_ABC_H__ */
