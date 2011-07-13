// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_ObjectAttributeContainer_h__
#define __dispatcher_ObjectAttributeContainer_h__

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace sword
{
    class ObjectAttributes;
}

namespace dispatcher
{
    class ObjectAttribute_ABC;
    class Model_ABC;

// =============================================================================
/** @class  Object_ABC
    @brief  Object_ABC
*/
// Created: SBO 2010-06-07
// =============================================================================
class ObjectAttributeContainer : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectAttributeContainer( const Model_ABC& model );
    virtual ~ObjectAttributeContainer();
    //@}

    //! @name Operations
    //@{
    void Update( const sword::ObjectAttributes& message );
    void Send  ( sword::ObjectAttributes& message ) const;
    //@}

private:
    //! @name Helpers
    //@{
    template< typename T > void CreateOrUpdate( const sword::ObjectAttributes& message );
    template< typename T > void CreateOrUpdate( const sword::ObjectAttributes& message, const Model_ABC& model );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::ptr_vector< ObjectAttribute_ABC > T_ObjectAttributes;
    //@}

private:
    const Model_ABC& model_;
    T_ObjectAttributes attributes_;
};

}

#endif // __dispatcher_ObjectAttributeContainer_h__
