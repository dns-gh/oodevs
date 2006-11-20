// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NET_AsnException_h_
#define __NET_AsnException_h_

#include "MIL.h"

// =============================================================================
/** @class  NET_AsnException
    @brief  NET_AsnException
*/
// Created: NLD 2006-11-13
// =============================================================================
template< typename T >
class NET_AsnException : public std::exception
{
public:
    //! @name Constructors/Destructor
    //@{
             NET_AsnException( T nErrorID );
    virtual ~NET_AsnException();
    //@}

    //! @name Operations
    //@{
            T           GetErrorID() const;
    virtual const char* what      () const;
    //@}

private:
    const T nErrorID_;
};


#include "NET_AsnException.inl"

#endif // __NET_AsnException_h_
