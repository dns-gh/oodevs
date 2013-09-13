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
             ADN_ProgressIndicator_ABC() {}
    //@}

    //! @name Abstract operations
    //@{
    virtual void SetMaximum( int n ) = 0;
    virtual void Increment( const QString& text ) = 0;
    virtual void SetVisible( bool visible ) = 0;
    //@}
};

#endif // __ADN_ProgressIndicator_ABC_h_
