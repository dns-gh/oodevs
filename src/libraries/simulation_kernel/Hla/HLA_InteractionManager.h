// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-29 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_InteractionManager.h $
// $Author: Age $
// $Modtime: 6/12/04 16:54 $
// $Revision: 3 $
// $Workfile: HLA_InteractionManager.h $
//
// *****************************************************************************

#ifndef __HLA_InteractionManager_h_
#define __HLA_InteractionManager_h_

#include "HLA_InteractionManager_ABC.h"
#include "hla/InteractionNotification_ABC.h"

class Federate;
template< typename InteractionClass > class Interaction;

// =============================================================================
/** @class  HLA_InteractionManager
    @brief  HLA interaction manager implementation
*/
// Created: AGE 2004-11-29
// =============================================================================
class HLA_InteractionManager
  : public HLA_InteractionManager_ABC
  , public InteractionNotification_ABC< HLA_DirectFire >
  , public InteractionNotification_ABC< HLA_IndirectFire >
  , public InteractionNotification_ABC< HLA_Neutralization >
  , public InteractionNotification_ABC< HLA_Activation >
  , public InteractionNotification_ABC< HLA_Bypassing >
  , public InteractionNotification_ABC< HLA_Construction >
  , public InteractionNotification_ABC< HLA_Mining >
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_InteractionManager( Federate& federate );
    virtual ~HLA_InteractionManager();
    //@}

    //! @name Operations
    //@{
    virtual void Send( const HLA_DirectFire& interaction );
    virtual void Send( const HLA_IndirectFire& interaction );
    virtual void Send( const HLA_Neutralization& interaction );

    virtual void Send( const HLA_Activation& interaction );
    virtual void Send( const HLA_Bypassing& interaction );
    virtual void Send( const HLA_Construction& interaction );
    virtual void Send( const HLA_Mining& interaction );
    //@}

private:
    //! @name Operations
    //@{
    virtual void Receive( HLA_DirectFire& interaction );
    virtual void Receive( HLA_IndirectFire& interaction );
    virtual void Receive( HLA_Neutralization& interaction );
    virtual void Receive( HLA_Activation& interaction );
    virtual void Receive( HLA_Bypassing& interaction );
    virtual void Receive( HLA_Construction& interaction );
    virtual void Receive( HLA_Mining& interaction );
    //@}

private:
    //! @name Member data
    //@{
    Interaction< HLA_DirectFire >*     pDirectFireInteraction_;
    Interaction< HLA_IndirectFire >*   pIndirectFireInteraction_;
    Interaction< HLA_Neutralization >* pNeutralizationInteraction_;
    Interaction< HLA_Activation >*     pActivationInteraction_;
    Interaction< HLA_Bypassing >*      pBypassingInteraction_;
    Interaction< HLA_Construction >*   pConstructionInteraction_;
    Interaction< HLA_Mining >*         pMiningInteraction_;
    //@}
};

#endif // __HLA_InteractionManager_h_
