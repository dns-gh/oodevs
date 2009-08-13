// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_NBC_h_
#define __PHY_RoleHLA_NBC_h_

#include "PHY_RoleInterface_NBC.h"

// =============================================================================
/** @class  PHY_RoleHLA_NBC
    @brief  HLA role NBC
*/
// Created: AGE 2004-11-09
// =============================================================================
class PHY_RoleHLA_NBC : public PHY_RoleInterface_NBC
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
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}

private:
    //! @name Member data
    //@{
    bool bContaminated_;
    //@}
};

#endif // __PHY_RoleHLA_NBC_h_
