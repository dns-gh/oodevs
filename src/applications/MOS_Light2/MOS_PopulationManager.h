// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_PopulationManager_h_
#define __MOS_PopulationManager_h_

#ifdef __GNUG__
#   pragma interface
#endif



// =============================================================================
/** @class  MOS_PopulationManager
    @brief  MOS_PopulationManager
    @par    Using example
    @code
    MOS_PopulationManager;
    @endcode
*/
// Created: HME 2005-09-29
// =============================================================================
class MOS_PopulationManager
{

public:
    //! @name Constructors/Destructor
    //@{
             MOS_PopulationManager();
    virtual ~MOS_PopulationManager();
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
    MOS_PopulationManager( const MOS_PopulationManager& );            //!< Copy constructor
    MOS_PopulationManager& operator=( const MOS_PopulationManager& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};



#include "MOS_PopulationManager.inl"

#endif // __MOS_PopulationManager_h_
