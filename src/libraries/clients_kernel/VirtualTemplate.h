// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __VirtualTemplate_h_
#define __VirtualTemplate_h_

#include "InterfaceContainer.h"

namespace kernel
{

template< typename T >
struct Caller
{
    virtual void Call( const T& ) = 0;
};

// =============================================================================
/** @class  VirtualTemplate
    @brief  Virtual template. Basically, uses the InterfaceContainer to create a
            dynamic, variable-length vft
*/
// Created: AGE 2006-02-09
// =============================================================================
template< typename DefaultCaller >
class VirtualTemplate : public InterfaceContainer< VirtualTemplate< DefaultCaller >  >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit VirtualTemplate( DefaultCaller* forward )
                 : forward_( forward )
             {
                 InterfaceContainer< VirtualTemplate >::Register( *this );
             };
    virtual ~VirtualTemplate() {};
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void Call( const T& argument )
    {
        if( Apply( & Caller< T >::Call, argument ) == 0 && forward_ )
            forward_->Call( argument );
    };
    //@}

private:
    //! @name Copy/Assignement
    //@{
    VirtualTemplate( const VirtualTemplate& );            //!< Copy constructor
    VirtualTemplate& operator=( const VirtualTemplate& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    DefaultCaller* forward_;
    //@}
};

}

#endif // __VirtualTemplate_h_
