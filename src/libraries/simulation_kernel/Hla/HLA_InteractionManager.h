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
#include <hla/InteractionNotification_ABC.h>

namespace hla
{
    class Federate;
    template< typename InteractionClass > class Interaction;
}

// =============================================================================
/** @class  HLA_InteractionManager
    @brief  HLA interaction manager implementation
*/
// Created: AGE 2004-11-29
// =============================================================================
class HLA_InteractionManager : public HLA_InteractionManager_ABC
                             , public hla::InteractionNotification_ABC< HLA_DirectFire >
                             , public hla::InteractionNotification_ABC< HLA_IndirectFire >
                             , public hla::InteractionNotification_ABC< HLA_Neutralization >
                             , public hla::InteractionNotification_ABC< HLA_Activation >
                             , public hla::InteractionNotification_ABC< HLA_Bypassing >
                             , public hla::InteractionNotification_ABC< HLA_Construction >
                             , public hla::InteractionNotification_ABC< HLA_Mining >
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_InteractionManager( hla::Federate& federate );
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
    hla::Interaction< HLA_DirectFire >*     pDirectFireInteraction_;
    hla::Interaction< HLA_IndirectFire >*   pIndirectFireInteraction_;
    hla::Interaction< HLA_Neutralization >* pNeutralizationInteraction_;
    hla::Interaction< HLA_Activation >*     pActivationInteraction_;
    hla::Interaction< HLA_Bypassing >*      pBypassingInteraction_;
    hla::Interaction< HLA_Construction >*   pConstructionInteraction_;
    hla::Interaction< HLA_Mining >*         pMiningInteraction_;
    //@}
};

#endif // __HLA_InteractionManager_h_
