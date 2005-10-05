// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_Agent_ABC_h_
#define __MOS_Agent_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  MOS_Agent_ABC
    @brief  MOS_Agent_ABC
    @par    Using example
    @code
    MOS_Agent_ABC;
    @endcode
*/
// Created: HME 2005-10-05
// =============================================================================
class MOS_Agent_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MOS_Agent_ABC();
    virtual ~MOS_Agent_ABC();
    //@}

	virtual const uint GetID() =0;
	virtual const std::string GetName() const  =0;
    virtual const MT_Vector2D& GetPos() const = 0;

private:

private:
    //! @name Member data
    //@{
    //@}
};



#include "MOS_Agent_ABC.inl"

#endif // __MOS_Agent_ABC_h_
