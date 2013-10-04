// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __PHY_HumanState_h_
#define __PHY_HumanState_h_

#include "simulation_kernel/Entities/Agents/Units/Humans/Human_ABC.h"

class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;
class PHY_HumanRank;
class PHY_HumanWound;

namespace xml
{
    class xostream;
}

namespace human
{
// =============================================================================
/** @class  PHY_HumanState
    @brief  PHY_HumanState
*/
// Created: LDC 2012-10-26
// =============================================================================
class PHY_HumanState
{
    public:
    //! @name Constructors/Destructor
    //@{
             PHY_HumanState();
             PHY_HumanState( unsigned int number, const PHY_HumanRank& rank, const PHY_HumanWound& state, E_HumanLocation location = eHumanLocation_Battlefield, bool contaminated = false, bool psyop = false );
    virtual ~PHY_HumanState();
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER();
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void Write( xml::xostream& xos ) const;
    //@}

    //! @name Member data
    //@{
    unsigned int          number_;
    const PHY_HumanRank*  rank_;
    const PHY_HumanWound* state_;
    E_HumanLocation       location_;
    bool                  contaminated_;
    bool                  psyop_;
    //@}
};

}

#endif // __PHY_HumanState_h_
