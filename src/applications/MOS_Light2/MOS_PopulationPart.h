// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_PopulationPart_h_
#define __MOS_PopulationPart_h_

#ifdef __GNUG__
#   pragma interface
#endif

namespace MOS {

// =============================================================================
/** @class  MOS_PopulationPart
    @brief  MOS_PopulationPart
    @par    Using example
    @code
    MOS_PopulationPart;
    @endcode
*/
// Created: HME 2005-10-04
// =============================================================================
class MOS_PopulationPart
{

public:
    //! @name Constructors/Destructor
    //@{
             MOS_PopulationPart();
    virtual ~MOS_PopulationPart();
    //@}

    //! @name Operations
    //@{
    //@}

    //! @name Accessors
    //@{
    //@}

    //! @name Modifiers
    //@{
    //@}

    //! @name Operators
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MOS_PopulationPart( const MOS_PopulationPart& );            //!< Copy constructor
    MOS_PopulationPart& operator=( const MOS_PopulationPart& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

} // end namespace MOS

#include "MOS_PopulationPart.inl"

#endif // __MOS_PopulationPart_h_
