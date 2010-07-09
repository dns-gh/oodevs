// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Effect_HLAIndirectFire_h_
#define __MIL_Effect_HLAIndirectFire_h_

#include "MIL_Effect_ABC.h"

class PHY_DotationCategory;
class MIL_Agent_ABC;

// =============================================================================
/** @class  MIL_Effect_HLAIndirectFire
    @brief  HLA indirect fire effect
*/
// Created: AGE 2004-11-29
// =============================================================================
class MIL_Effect_HLAIndirectFire : public MIL_Effect_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Effect_HLAIndirectFire( const PHY_DotationCategory& ammunition, MIL_Agent_ABC& target );
    virtual ~MIL_Effect_HLAIndirectFire();
    //@}

    //! @name Operations
    //@{
    virtual bool Execute();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MIL_Effect_HLAIndirectFire( const MIL_Effect_HLAIndirectFire& );            //!< Copy constructor
    MIL_Effect_HLAIndirectFire& operator=( const MIL_Effect_HLAIndirectFire& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const PHY_DotationCategory& ammunition_;
    MIL_Agent_ABC&              target_;
    //@}
};

#endif // __MIL_Effect_HLAIndirectFire_h_
