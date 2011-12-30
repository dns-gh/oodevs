// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __VariantPointer_h_
#define __VariantPointer_h_

//#include <QMetaType>

namespace kernel
{

// =============================================================================
/** @class  VariantPointer
    @brief  Variant pointer
*/
// Created: ABR 2011-09-23
// =============================================================================
class VariantPointer
{
public:
    //! @name Constructors/Destructor
    //@{
    VariantPointer() : ptr_( 0 ) {}
    VariantPointer( const VariantPointer& other ) : ptr_( other.ptr_ ) {}
    VariantPointer( const void* ptr ) : ptr_( ptr ) {}
    //@}

    //! @name Member data
    //@{
    const void* ptr_;
    //@}
};
Q_DECLARE_METATYPE( VariantPointer );

}


#endif // __VariantPointer_h_
