// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PHY_HumanFactor_h_
#define __PHY_HumanFactor_h_

#include "MT_Tools/MT_String.h"

namespace sword
{
    enum UnitAttributes_EnumUnitTiredness;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  PHY_HumanFactor
    @brief  PHY_HumanFactor
*/
// Created: LDC 2011-06-16
// =============================================================================
class PHY_HumanFactor
{
public:
    //! @name Accessors
    //@{
    const std::string&        GetName                         () const;
          unsigned int        GetID                           () const;
          double              GetCoefMaxSpeedModificator      () const;
          double              GetCoefReloadingTimeModificator () const;
          double              GetCoefPhModificator            () const;
          double              GetCoefPostureTimeModificator   () const;
          double              GetCoefSensorDistanceModificator() const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const PHY_HumanFactor& rhs ) const;
    bool operator!=( const PHY_HumanFactor& rhs ) const;
    //@}

protected:
    PHY_HumanFactor( const std::string& strName, unsigned int nType );

protected:
    //! @name Init
    //@{
    void Read( xml::xistream& xis );
    //@}

private:
    std::string               strName_;
    unsigned int              nType_;

    double rCoefMaxSpeedModificator_;
    double rCoefReloadingTimeModificator_;
    double rCoefPhModificator_;
    double rCoefPostureTimeModificator_;
    double rCoefSensorDistanceModificator_;
};

#endif // __PHY_HumanFactor_h_
