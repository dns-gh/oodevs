// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DinMessageWrapper_h_
#define __DinMessageWrapper_h_

#include "DIN/DIN_Input.h"

#define DECLARE_MESSAGE_WRAPPER( name )         \
struct name : public DinMessageWrapper< name >   \
{                                               \
    name( DIN::DIN_Input& input )               \
        : DinMessageWrapper< name >( input )    \
    {};                                         \
}

// =============================================================================
/** @class  DinMessageWrapper
    @brief  DIN_Input wrapper
*/
// Created: AGE 2006-02-07
// =============================================================================
template< typename ConcreteWrapper >
class DinMessageWrapper
{

public:
    //! @name Operations
    //@{
    template< typename T >
    const ConcreteWrapper& operator>>( T& value ) const
    {
        (*input_) >> value;
        return *(const ConcreteWrapper* )( this );
    };
    //@}

protected:
    //! @name Constructors/Destructor
    //@{
             DinMessageWrapper( DIN::DIN_Input& input ) : input_( & input ) {};
    virtual ~DinMessageWrapper() {};
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DinMessageWrapper( const DinMessageWrapper& );            //!< Copy constructor
    DinMessageWrapper& operator=( const DinMessageWrapper& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    DIN::DIN_Input* input_;
    //@}
};

#endif // __DinMessageWrapper_h_
