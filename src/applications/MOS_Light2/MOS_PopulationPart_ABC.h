// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_PopulationPart_ABC_h_
#define __MOS_PopulationPart_ABC_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_ASN_Types.h"
#include <string>

// =============================================================================
/** @class  MOS_PopulationPart_ABC
    @brief  MOS_PopulationPart_ABC
    @par    Using example
    @code
    MOS_PopulationPart_ABC;
    @endcode
*/
// Created: HME 2005-10-04
// =============================================================================
class MOS_PopulationPart_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MOS_PopulationPart_ABC();
    virtual ~MOS_PopulationPart_ABC();
    //@}
	

	virtual uint GetLivingHumans()	= 0;
	virtual uint GetDeadHumans()		= 0;
	virtual uint	GetID()	=0;
	virtual std::string GetName() = 0;
	virtual std::string GetStringAttitude() =0;

	bool HasLivingHumans;
	bool HasAttitude;
	bool HasDeadAttitude;


private:


};



#include "MOS_PopulationPart_ABC.inl"

#endif // __MOS_PopulationPart_ABC_h_
