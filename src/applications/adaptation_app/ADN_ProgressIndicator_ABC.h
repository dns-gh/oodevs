// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_ProgressIndicator_ABC_h_
#define __ADN_ProgressIndicator_ABC_h_

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  ADN_ProgressIndicator_ABC
    @brief  ADN_ProgressIndicator_ABC
*/
// Created: APE 2005-03-18
// =============================================================================
class ADN_ProgressIndicator_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ProgressIndicator_ABC();
    virtual ~ADN_ProgressIndicator_ABC();
    //@}

    //! @name Abstract operations
    //@{
    virtual void SetNbrOfSteps( int n ) = 0;
    virtual void Increment( int n = 1 ) = 0;
    virtual void Increment( const char* szText, int n = 1 ) = 0;
    virtual void Reset( const char* szMsg = 0) = 0;
    //@}

    //! @name Operations
    //@{
    void SetVisible( bool visible );
    //@}
};

#endif // __ADN_ProgressIndicator_ABC_h_
