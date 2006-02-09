// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TypePopulation_h_
#define __TypePopulation_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"

class AgentModel;

// =============================================================================
// Created: HME 2005-09-28
// =============================================================================


class TypePopulation
{
	MT_COPYNOTALLOWED( TypePopulation );
public:
    //! @name Constructors/Destructor
    //@{
    TypePopulation( const std::string& strName, InputArchive& archive );
    virtual ~TypePopulation();
    //@}

    //! @name Accessors
    //@{
    const std::string&    GetName                () const;
          uint            GetID                  () const;
    const AgentModel& GetModel               () const;
          MT_Float        GetConcentrationDensity() const;
          MT_Float        GetDefaultFlowDensity  () const;
          MT_Float        GetMaxSpeed            () const;
    //@}

private:
	const std::string     strName_;
    uint                  nID_;
    const AgentModel* pModel_;
    MT_Float              rConcentrationDensity_;
    MT_Float              rDefaultFlowDensity_;
    MT_Float              rMaxSpeed_;
};


#include "TypePopulation.inl"

#endif // __TypePopulation_h_
