// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __OccupantAttribute_h_
#define __OccupantAttribute_h_

#include "ObjectAttribute_ABC.h"

class MIL_Agent_ABC;

// =============================================================================
/** @class  OccupantAttribute
    @brief  OccupantAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class OccupantAttribute : public ObjectAttribute_ABC
{ 

public:
    //! @name Constructors/Destructor
    //@{
             OccupantAttribute();
    virtual ~OccupantAttribute();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    void AddOccupant( const MIL_Agent_ABC& agent );
    void ReleaseOccupant( const MIL_Agent_ABC& agent );
    //@}

    //! @name 
    //@{
    const MIL_Agent_ABC* GetOccupant() const;
    //@}

    //! @name Operations
    //@{
    OccupantAttribute& operator=( const OccupantAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MIL_Agent_ABC* >  T_AgentSet;
    typedef T_AgentSet::const_iterator  CIT_AgentSet;
    //@}

private:
    //! @name Member data
    //@{
    const MIL_Agent_ABC*  pOccupant_;
    //@}
};

#endif // __AnimatorAttibute_h_
