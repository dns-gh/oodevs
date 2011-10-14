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

class MIL_Object_ABC;

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
             ObjectPrototype( const std::string& name, const std::string& type, unsigned int id, double pointSize );
    virtual ~ObjectPrototype();
    //@}

    //! @name Operations
    //@{
    template< typename Capacity >
    void AddCapacity( Capacity* capacity );
    void Build( MIL_Object_ABC& object ) const;
    //@}

    //! @name Accessors
    //@{
    const MIL_ObjectType_ABC& GetType() const;
    virtual unsigned int GetID() const;
    virtual const std::string& GetName() const;
    virtual const std::string& GetRealName() const;
    virtual double GetPointSize() const;
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
    const std::string name_;
    const std::string type_;
    unsigned int id_;
    double pointSize_;
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
