// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef WRAPPER_NODE_H
#define WRAPPER_NODE_H

#include <module_api/Model.h>
#include <string>

namespace sword
{
namespace wrapper
{
    class View;

// =============================================================================
/** @class  Node
    @brief  Node
*/
// Created: SLI 2011-12-07
// =============================================================================
class Node
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Node( SWORD_Model* node );
    //@}

    //! @name Operations
    //@{
    Node& operator=( int8_t value );
    Node& operator=( int16_t value );
    Node& operator=( int32_t value );
    Node& operator=( int64_t value );
    Node& operator=( uint8_t value );
    Node& operator=( uint16_t value );
    Node& operator=( uint32_t value );
    Node& operator=( uint64_t value );
    Node& operator=( float value );
    Node& operator=( double value );
    Node& operator=( const std::string& value );
    Node& operator=( const View& value );

    Node operator[]( const std::string& key );
    Node operator[]( const char* key );
    Node operator[]( size_t key );

    Node AddElement();
    //@}

private:
    //! @name Member data
    //@{
    SWORD_Model* node_;
    //@}
};

}
}

#endif // WRAPPER_NODE_H
