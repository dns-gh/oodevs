// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef WRAPPER_EFFECT_H
#define WRAPPER_EFFECT_H

#include "Node.h"
#include <boost/noncopyable.hpp>

namespace sword
{
namespace wrapper
{
    class View;

// =============================================================================
/** @class  Effect
    @brief  Effect
*/
// Created: SLI 2011-12-21
// =============================================================================
class Effect : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Effect( const View& target );
            ~Effect();
    //@}

    //! @name Operations
    //@{
    void Post() const; // $$$$ MCO : should it be done by the destructor (it cannot throw) ?

    Node AddElement();
    Node MarkForRemove();

    template< typename T >
    Effect& operator=( T value )
    {
        node_ = value;
        return *this;
    }

    template< typename T >
    Node operator[]( T key )
    {
        return node_[ key ];
    }
    //@}

private:
    //! @name Member data
    //@{
    SWORD_Model* effect_;
    Node node_;
    const SWORD_Model* target_;
    //@}
};

}
}

#endif // WRAPPER_EFFECT_H
