// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-11-02 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __MIL_PopulationPionAttritionData_h_
#define __MIL_PopulationPionAttritionData_h_

#include "MIL.h"

#include "Entities/Agents/Units/Weapons/PHY_AttritionData.h"

class MIL_PopulationAttitude;

// =============================================================================
// Created: NLD 2005-11-02
// =============================================================================
class MIL_PopulationPionAttritionData
{
public:
    //! @name Constructors/Destructor
    //@{
     MIL_PopulationPionAttritionData();
    ~MIL_PopulationPionAttritionData();
    //@}

    //! @name Initialization
    //@{
    void Initialize( MIL_InputArchive& archive );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MIL_PopulationPionAttritionData( const MIL_PopulationPionAttritionData& );            //!< Copy constructor
    MIL_PopulationPionAttritionData& operator=( const MIL_PopulationPionAttritionData& ); //!< Assignement operator
    //@}

    //! @name Initialization
    //@{
    void ReadAttitudeAttritionData( const MIL_PopulationAttitude& attitude, MIL_InputArchive& archive );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_AttritionData >   T_AttritionVector;
    typedef T_AttritionVector::const_iterator  CIT_AttritionVector;

    struct sAttritionData
    {
        sAttritionData();

        T_AttritionVector attritions_; // Per protection
        MT_Float          rPopulationDensity_;
    };

    typedef std::vector< sAttritionData > T_AttitudeAttritionDataVector; // Per attitude
    //@}

private:
    T_AttitudeAttritionDataVector attitudeAttritionData_;
};

#include "MIL_PopulationPionAttritionData.inl"

#endif // __MIL_PopulationPionAttritionData_h_
