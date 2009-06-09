// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ObjectPrototype_h_
#define __ObjectPrototype_h_

#include "MIL_ObjectType_ABC.h"
#include "MIL_ObjectBuilder_ABC.h"

class Object;

// =============================================================================
/** @class  ObjectPrototype
    @brief  ObjectPrototype
*/
// Created: JCR 2008-05-23
// =============================================================================
class ObjectPrototype : public MIL_ObjectType_ABC
                      , public MIL_ObjectBuilder_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectPrototype( const std::string& type, unsigned int id );
    virtual ~ObjectPrototype();
    //@}

    //! @name Operations
    //@{
    template< typename Capacity >
    void AddCapacity( Capacity* capacity );
    void Build( Object& object ) const;
    //@}

    //! @name Accessors
    //@{
    const MIL_ObjectType_ABC& GetType() const;
    unsigned int GetID() const;
    const std::string& GetName() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectPrototype( const ObjectPrototype& );            //!< Copy constructor
    ObjectPrototype& operator=( const ObjectPrototype& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const std::string type_;
    unsigned int id_;
    std::vector< ObjectCapacity_ABC* > capacities_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: template <typename Capacity> void ObjectPrototype::AddCapacity
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
template< typename Capacity > 
void ObjectPrototype::AddCapacity( Capacity* capacity )
{
    Attach( *capacity );
    capacities_.push_back( capacity );
}

#endif // __ObjectPrototype_h_
