// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockPrototype_h_
#define __MockPrototype_h_

//#include "simulation_kernel/Entities/Objects/MIL_ObjectBuilder_ABC.h"
//#include "simulation_kernel/Entities/Objects/MIL_ObjectType_ABC.h"
#include "simulation_kernel/Entities/Objects/ObjectPrototype.h"
#include <set>

class ObjectCapacity_ABC;

// =============================================================================
/** @class  MockPrototype
    @brief  MockPrototype
*/
// Created: JCR 2008-09-01
// =============================================================================
class MockPrototype 
    : public mockpp::ChainableMockObject
    , public ObjectPrototype // MIL_ObjectType_ABC, public MIL_ObjectBuilder_ABC    
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MockPrototype( const std::string& type, uint id ) 
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockPrototype" ), 0 )
        , ObjectPrototype( type, id )
        , Add_mocker                        ( MOCKPP_PCHAR( "AddCapacity" ), this )        
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetNameShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetID )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( GetTypeShadow )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( Build )
    {
    }
    virtual ~MockPrototype()
    {
    }
    //@}
        
    virtual const std::string& GetName() const
    {
        return *GetNameShadow();
    }

    virtual const MIL_ObjectType_ABC& GetType() const
    {
        return *GetTypeShadow();
    }

    void AddCapacity( ObjectCapacity_ABC* capacity )
    {
        Add_mocker.forward( capacity );
        capacities_.insert( capacity );
    }

    MOCKPP_CONST_CHAINABLE0          ( MockPrototype, uint, GetID );
    MOCKPP_CONST_CHAINABLE_EXT0      ( MockPrototype, const std::string*, GetNameShadow, std::string, );   
    MOCKPP_CONST_CHAINABLE_EXT0      ( MockPrototype, const MIL_ObjectType_ABC*, GetTypeShadow, MIL_ObjectType_ABC, );   
    MOCKPP_VOID_CONST_CHAINABLE_EXT1 ( MockPrototype, Build, Object&, , Object );

    mockpp::ChainableMockMethod< void, ObjectCapacity_ABC* > Add_mocker;
    std::set< ObjectCapacity_ABC* >    capacities_;
};

#endif // __MockPrototype_h_
