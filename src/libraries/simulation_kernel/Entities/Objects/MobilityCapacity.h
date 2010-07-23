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
    MT_Float GetDefaultSpeed() const;
    MT_Float ApplySpeedPolicy( MT_Float rAgentSpeedWithinObject, MT_Float rAgentSpeedWithinEnvironment, MT_Float rAgentMaxSpeed, MT_Float structural ) const;
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
    MT_Float ComputeStructuralFactor( const MT_Float structural ) const;
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
    MT_Float      rDefaultSpeed_;
    float         rSpeedPolicyMaxSpeedAgentFactor_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MobilityCapacity )

#endif
