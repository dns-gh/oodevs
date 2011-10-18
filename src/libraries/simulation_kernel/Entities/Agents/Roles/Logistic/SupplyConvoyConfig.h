// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SupplyConvoyConfig_h_
#define __SupplyConvoyConfig_h_

#include "MT_Tools/MT_InterpolatedFunction.h"

class MIL_AgentTypePion;
class MIL_MissionType_ABC;

namespace logistic
{

class SupplyConvoyConfig
{
public:
    //! @name Ugly static initialization
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();
    //@}

private:
    //! @name Ugly static initialization
    //@{
    struct LoadingWrapper;
    static void InitializeConvoyUnitType( xml::xistream& xis );
    static void InitializeConvoyMission ( xml::xistream& xis );
    static void InitializeInterpolatedTime ( xml::xistream& xis, const std::string& strTagName, MT_InterpolatedFunction< double >& data );
    static void InitializeSpeedModificators( xml::xistream& xis );
    static void ReadInterpolatedTime( xml::xistream& xis, MT_InterpolatedFunction< double >& data, std::pair< unsigned int, double >& upperBound );
    static void ReadSpeedModifier( xml::xistream& xis, std::pair< unsigned int, double >& upperBound );   
    //@}

public:
    static MT_InterpolatedFunction< double > setupTime_;
    static MT_InterpolatedFunction< double > loadingTime_;
    static MT_InterpolatedFunction< double > unloadingTime_;
    static MT_InterpolatedFunction< double > coefSpeedModificator_;
    static const MIL_AgentTypePion* convoyAgentType_;
    static const MIL_MissionType_ABC* convoyMissionType_;
};

} // end namespace logistic

#endif // __SupplyConvoyConfig_h_
