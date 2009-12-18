// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __transport_TransportCapacityComputerFactory_H__
#define __transport_TransportCapacityComputerFactory_H__

#include "TransportCapacityComputerFactory_ABC.h"

namespace transport {

// =============================================================================
/** @class  TransportComputerFactory
    @brief  TransportComputerFactory
*/
// Created: AHC 2009-10-01
// =============================================================================
class TransportComputerFactory : public TransportComputerFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    TransportComputerFactory();
    virtual ~TransportComputerFactory();
    //@}
    
    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const uint ) {}
    //@}
    
    //! @name Operations
    //@{
    virtual std::auto_ptr< TransportCapacityComputer_ABC > CreateCapacityComputer() const ;
    virtual std::auto_ptr< TransportWeightComputer_ABC > CreateWeightComputer( const TransportStrategy_ABC* ) const;
    virtual std::auto_ptr< TransportPermissionComputer_ABC > CreatePermissionComputer() const;
    //@}
};

}

#endif /* __transport_TransportCapacityComputerFactory_H__ */
