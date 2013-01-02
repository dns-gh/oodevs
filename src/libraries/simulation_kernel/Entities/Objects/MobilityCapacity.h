// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MobilityCapacity_h_
#define __MobilityCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  MobilityCapacity
    @brief  Mobility capacity
*/
// Created: JCR 2008-06-02
// =============================================================================
class MobilityCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MobilityCapacity();
    explicit MobilityCapacity( xml::xistream& xis );
    virtual ~MobilityCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Register( MIL_Object_ABC& object );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    double GetDefaultSpeed() const;
    double ApplySpeedPolicy( double rAgentSpeedWithinObject, double rAgentSpeedWithinEnvironment, double rAgentMaxSpeed, double structural ) const;
    bool IsMaxSpeed() const;
    //@}

private:
    //! @name Copy
    //@{
    MobilityCapacity( const MobilityCapacity& from );
    //@}

    //! @name Helpers
    //@{
    void InitializeSpeed( xml::xistream& xis );
    void InitializeSpeedPolicy( xml::xistream& xis );
    double ComputeStructuralFactor( const double structural ) const;
    //@}

    //! @name Types
    //@{
    enum E_SpeedPolicy
    {
        eSpeedPolicy_Slowest,
        eSpeedPolicy_AgentMaxSpeed,
        eSpeedPolicy_ObjectMaxSpeed,
        eSpeedPolicy_UrbanObjectMaxSpeed
    };
    //@}

private:
    //! @name Member data
    //@{
    E_SpeedPolicy nSpeedPolicy_;
    double        rDefaultSpeed_;
    float         rSpeedPolicyMaxSpeedAgentFactor_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MobilityCapacity )

#endif
