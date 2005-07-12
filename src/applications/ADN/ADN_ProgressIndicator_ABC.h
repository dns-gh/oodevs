// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ProgressIndicator_ABC.h $
// $Author: Ape $
// $Modtime: 18/03/05 11:41 $
// $Revision: 1 $
// $Workfile: ADN_ProgressIndicator_ABC.h $
//
// *****************************************************************************

#ifndef __ADN_ProgressIndicator_ABC_h_
#define __ADN_ProgressIndicator_ABC_h_


// =============================================================================
/** @class  ADN_ProgressIndicator_ABC
    @brief  ADN_ProgressIndicator_ABC
*/
// Created: APE 2005-03-18
// =============================================================================
class ADN_ProgressIndicator_ABC
{
    MT_COPYNOTALLOWED( ADN_ProgressIndicator_ABC )

public:
    //! @name Constructors/Destructor
    //@{
    ADN_ProgressIndicator_ABC();
    virtual ~ADN_ProgressIndicator_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void SetNbrOfSteps( int n ) = 0;
    virtual void Increment( int n = 1 ) = 0;
    virtual void Increment( const char* szText, int n = 1 ) = 0;
    virtual void Reset( const char* szMsg = 0) = 0;
    //@}
};

#endif // __ADN_ProgressIndicator_ABC_h_
