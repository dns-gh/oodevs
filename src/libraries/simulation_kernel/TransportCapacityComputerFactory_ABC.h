// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __transport_TransportCapacityComputerFactory_ABC_H__
#define __transport_TransportCapacityComputerFactory_ABC_H__

namespace transport {

class TransportCapacityComputer_ABC;
class TransportWeightComputer_ABC;
class TransportPermissionComputer_ABC;
class TransportStrategy_ABC;

// =============================================================================
/** @class  TransportComputerFactory_ABC
    @brief  TransportComputerFactory_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================
class TransportComputerFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    TransportComputerFactory_ABC() {}
    virtual ~TransportComputerFactory_ABC() {}
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}
        
    //! @name Operations
    //@{
    virtual std::auto_ptr< TransportCapacityComputer_ABC > CreateCapacityComputer( ) const = 0;
    virtual std::auto_ptr< TransportWeightComputer_ABC > CreateWeightComputer( const TransportStrategy_ABC* ) const = 0;
    virtual std::auto_ptr< TransportPermissionComputer_ABC > CreatePermissionComputer() const = 0;
    //@}
};

}

#endif /* __transport_TransportCapacityComputerFactory_ABC_H__ */
