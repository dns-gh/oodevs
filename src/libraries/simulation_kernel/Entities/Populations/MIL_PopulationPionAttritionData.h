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

namespace xml
{
    class xistream;
}

class MIL_PopulationAttitude;
class PHY_Protection;

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
    void Initialize( xml::xistream& xis );
    //@}

    //! @name Accessors
    //@{
    const PHY_AttritionData& GetAttritionData( const MIL_PopulationAttitude& attitude, const PHY_Protection& protection ) const;
          MT_Float           GetPH           ( const MIL_PopulationAttitude& attitude, MT_Float rDensity ) const;
    //@}
    
private:
    //! @name Copy/Assignement
    //@{
    MIL_PopulationPionAttritionData( const MIL_PopulationPionAttritionData& );            //!< Copy constructor
    MIL_PopulationPionAttritionData& operator=( const MIL_PopulationPionAttritionData& ); //!< Assignement operator
    //@}

    //! @name Initialization
    //@{
    void ReadAttitudeData( const MIL_PopulationAttitude& attitude, xml::xistream& xis );
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
        MT_Float          rIntensity_;
    };

    typedef std::vector< sAttritionData > T_AttitudeAttritionDataVector; // Per attitude
    //@}

    //! @name Helpers
    //@{
    void ReadAttritionEffect    ( xml::xistream& xis );
    void ReadAttritionUnitEffect( xml::xistream& xis, sAttritionData& attitudeData );
    //@}

private:
    T_AttitudeAttritionDataVector attitudeAttritionData_;
};

#include "MIL_PopulationPionAttritionData.inl"

#endif // __MIL_PopulationPionAttritionData_h_
