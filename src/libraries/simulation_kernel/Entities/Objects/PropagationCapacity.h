// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PROPAGATION_CAPACITY_h
#define SWORD_PROPAGATION_CAPACITY_h

#include "ObjectCapacity_ABC.h"

namespace sword
{
namespace capacity
{
// =============================================================================
/** @class  PropagationCapacity
    @brief  Propagation capacity
*/
// Created: LGY 2012-09-19
// =============================================================================
class PropagationCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PropagationCapacity();
    explicit PropagationCapacity( xml::xistream& xis );
    virtual ~PropagationCapacity();
    //@}

    //! @name Serialization
    //@{
    template< typename Archive >
    void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Register( MIL_Object_ABC& object );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    //@}

private:
    PropagationCapacity( const PropagationCapacity& other );

private:
    //! @name Member Data
    //@{
    std::size_t command_;
    //@}
};

}

}

BOOST_CLASS_EXPORT_KEY( sword::capacity::PropagationCapacity )

#endif // SWORD_PROPAGATION_CAPACITY_h
