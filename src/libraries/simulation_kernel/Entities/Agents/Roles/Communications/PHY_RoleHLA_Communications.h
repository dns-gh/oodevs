// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Communications_h_
#define __PHY_RoleHLA_Communications_h_

#include "PHY_RoleInterface_Communications.h"
#include <string>

// =============================================================================
/** @class  PHY_RoleHLA_Communications
    @brief  HLA role communications
*/
// Created: AGE 2004-11-09
// =============================================================================
class PHY_RoleHLA_Communications : public PHY_RoleInterface_Communications
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleHLA_Communications();
    virtual ~PHY_RoleHLA_Communications();
    //@}

    //! @name Operations
    //@{
    virtual bool CanCommunicate() const;
    virtual void Jam           ( const MIL_Object_ABC& jammer );
    virtual void Unjam         ( const MIL_Object_ABC& jammer );
    virtual void ActivateBlackout  ();
    virtual void DeactivateBlackout();
    virtual void UpdateKnowledgesFromObjectPerception( const DEC_Knowledge_ObjectPerception& perception );
    virtual void UpdateKnowledgesFromObjectCollision ( const DEC_Knowledge_ObjectCollision& collision );

    virtual MT_Float ModifySpeed( MT_Float rSpeed ) const;
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}

    //! @name Operations
    //@{
    virtual void Update( bool /*bIsDead*/ ) {}
    virtual void Clean () {}
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( client::UnitAttributes& /*msg*/ ) const {};
    virtual void SendFullState   ( client::UnitAttributes& /*msg*/ ) const {};
    //@}

private:
    //! @name Member data
    //@{
    bool bJammed_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PHY_RoleHLA_Communications )

#endif // __PHY_RoleHLA_Communications_h_
