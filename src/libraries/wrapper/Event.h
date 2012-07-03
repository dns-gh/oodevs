// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef WRAPPER_EVENT_H
#define WRAPPER_EVENT_H

#include "Node.h"
#include <boost/noncopyable.hpp>
#include <string>

namespace sword
{
namespace wrapper
{
// =============================================================================
/** @class  Event
    @brief  Event
*/
// Created: SLI 2011-12-21
// =============================================================================
class Event : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Event( const std::string& name );
            ~Event();
    //@}

    //! @name Operations
    //@{
    void Post(); // $$$$ MCO : should it be done by the destructor (it cannot throw) ?

    Node AddElement();

    template< typename T >
    Event& operator=( T value )
    {
        node_ = value;
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
    SWORD_Model* event_;
    Node node_;
    const std::string name_;
    //@}
};

}
}

#endif // WRAPPER_EVENT_H
