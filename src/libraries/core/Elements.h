// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CORE_ELEMENTS_H
#define CORE_ELEMENTS_H

#include "core/Types.h"
#include "Serialization.h"
#include <vector>

namespace core
{
    class Model;

// =============================================================================
/** @class  Elements
    @brief  Elements
*/
// Created: MCO 2012-10-19
// =============================================================================
class Elements
{
public:
    //! @name Constructors/Destructor
    //@{
    Elements();
    //@}

    //! @name Operations
    //@{
    void Clear();
    Model& Add( Model* child ); // $$$$ MCO 2012-10-19: transmits ownership
    void Copy( Model& model ) const;
    const Model& Hide( std::size_t index );
    void Remove( const Model& value );

    template< class Archive >
    void serialize( Archive& archive, const unsigned int /*version*/ )
    {
        archive & size_ & elements_;
    }
    //@}

    //! @name Accessors
    //@{
    std::size_t Size() const;
    std::size_t Index( const Model& value ) const;
    //@}

    //! @name Operators
    //@{
    Model& operator[]( std::size_t index );
    const Model& operator[]( std::size_t index ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Model* > T_Elements;
    //@}

private:
    //! @name Member data
    //@{
    T_Elements elements_;
    std::size_t size_;
    //@}
};

}

#endif // CORE_ELEMENTS_H
