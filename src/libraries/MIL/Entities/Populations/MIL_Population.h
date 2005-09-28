// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-28 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __MIL_Population_h_
#define __MIL_Population_h_

#include "MIL.h"

class MIL_PopulationType;
class MIL_PopulationFlow;
class MIL_PopulationConcentration;
class MIL_Army;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_Population
{
public:
    //! @name Constructors/Destructor
    //@{
     MIL_Population( const MIL_PopulationType& type, uint nID, MIL_InputArchive& archive );
    ~MIL_Population();
    //@}

    //! @name Accessors
    //@{
    uint GetID() const;
    //@}

    //! @name Network 
    //@{
    void SendCreation    () const;
    void SendFullState   () const;
    void SendChangedState() const;    
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MIL_Population( const MIL_Population& );            //!< Copy constructor
    MIL_Population& operator=( const MIL_Population& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< MIL_PopulationConcentration* > T_ConcentrationSet;
    typedef T_ConcentrationSet::const_iterator       CIT_ConcentrationSet;

    typedef std::set< MIL_PopulationFlow* > T_FlowSet;
    typedef T_FlowSet::const_iterator       CIT_FlowSet;
    //@}

private:
    //! @name Network
    //@{    
    void SendDestruction() const;    
    //@}

private:
    const MIL_PopulationType& type_;
    const uint                nID_;
    const MIL_Army*           pArmy_;
          std::string         strName_;

          T_ConcentrationSet  concentrations_;
          T_FlowSet           flows_;
};

#include "MIL_Population.inl"

#endif // __MIL_Population_h_
