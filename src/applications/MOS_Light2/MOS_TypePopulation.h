// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_TypePopulation_h_
#define __MOS_TypePopulation_h_

#ifdef __GNUG__
#   pragma interface
#endif


#include "MOS_Types.h"

class MOS_AgentModel;

// =============================================================================
/** @class  MOS_TypePopulation
    @brief  MOS_TypePopulation
    @par    Using example
    @code
    MOS_TypePopulation;
    @endcode
*/
// Created: HME 2005-09-28
// =============================================================================


class MOS_TypePopulation
{
	MT_COPYNOTALLOWED( MOS_TypePopulation );
public:
    //! @name Constructors/Destructor
    //@{
    MOS_TypePopulation( const std::string& strName, MOS_InputArchive& archive );
    virtual ~MOS_TypePopulation();
    //@}

    //! @name Operations
    //@{
    //@}

    //! @name Accessors
    //@{
    const std::string&    GetName () const;
          uint            GetID   () const;
    const MOS_AgentModel& GetModel() const;
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
    //@}

    //! @name Helpers
    //@{
    //@}

private:
	const std::string     strName_;
    uint                  nID_;
    const MOS_AgentModel* pModel_;
};


#include "MOS_TypePopulation.inl"

#endif // __MOS_TypePopulation_h_
