// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_NBC_h_
#define __PHY_RoleHLA_NBC_h_

#include "PHY_RoleInterface_NBC.h"
#include <string>

// =============================================================================
/** @class  PHY_RoleHLA_NBC
    @brief  HLA role NBC
*/
// Created: AGE 2004-11-09
// =============================================================================
class PHY_RoleHLA_NBC : public nbc::PHY_RoleInterface_NBC
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleHLA_NBC();
    virtual ~PHY_RoleHLA_NBC();
    //@}

    //! @name Operations
    //@{
    virtual bool IsContaminated() const;
    virtual void Poison        ( const MIL_ToxicEffectManipulator& nbcAgent );
    virtual void Contaminate   ( const MIL_ToxicEffectManipulator& nbcAgent );
    virtual void Decontaminate ();
    virtual void Decontaminate ( MT_Float rRatioAgentsWorking );
    virtual MT_Float GetContaminationQuantity() const;  
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}

    //! @name Operations
    //@{
    virtual void Update    ( bool /*bIsDead*/ ) {}
    virtual void Clean     () {}
    virtual bool HasChanged() const { return false; }
    //@}

    //! @name Main
    //@{
    virtual void WearNbcProtectionSuit  () {}
    virtual void RemoveNbcProtectionSuit() {}

    virtual MT_Float ModifyMaxSpeed( MT_Float /*rSpeed*/    ) const { return 0; }
    //@}

    //! @name Network
    //@{
//    virtual void SendChangedState( client::UnitAttributes& msg ) const {};
//    virtual void SendFullState   ( client::UnitAttributes& msg ) const {};
    //@}

private:
    //! @name Member data
    //@{
    bool bContaminated_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RoleHLA_NBC )

#endif // __PHY_RoleHLA_NBC_h_
