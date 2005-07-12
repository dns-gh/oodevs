// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-24 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_InteractionManager_ABC.h $
// $Author: Age $
// $Modtime: 6/12/04 16:49 $
// $Revision: 4 $
// $Workfile: HLA_InteractionManager_ABC.h $
//
// *****************************************************************************

#ifndef __HLA_InteractionManager_ABC_h_
#define __HLA_InteractionManager_ABC_h_

class HLA_DirectFire;
class HLA_IndirectFire;
class HLA_Neutralization;
class HLA_Activation;
class HLA_Bypassing;
class HLA_Construction;
class HLA_Mining;

// =============================================================================
/** @class  HLA_InteractionManager_ABC
    @brief  HLA interaction manager definition
*/
// Created: AGE 2004-11-24
// =============================================================================
class HLA_InteractionManager_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_InteractionManager_ABC();
    virtual ~HLA_InteractionManager_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const HLA_DirectFire& interaction ) = 0;
    virtual void Send( const HLA_IndirectFire& interaction ) = 0;
    virtual void Send( const HLA_Neutralization& interaction ) = 0;

    virtual void Send( const HLA_Activation& interaction ) = 0;
    virtual void Send( const HLA_Bypassing& interaction ) = 0;
    virtual void Send( const HLA_Construction& interaction ) = 0;
    virtual void Send( const HLA_Mining& interaction ) = 0;
    //@}
};

#endif // __HLA_InteractionManager_ABC_h_
