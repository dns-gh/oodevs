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
    @brief  MobilityCapacity
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
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( Object& object );
    virtual void Instanciate( Object& object ) const;
    MT_Float GetDefaultSpeed() const;
    MT_Float ApplySpeedPolicy( MT_Float rAgentSpeedWithinObject, MT_Float rAgentSpeedWithinEnvironment, MT_Float rAgentMaxSpeed ) const;
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
    //@}

    //! @name Types
    //@{
    enum E_SpeedPolicy
    {
        eSpeedPolicy_Slowest,
        eSpeedPolicy_AgentMaxSpeed,
        eSpeedPolicy_ObjectMaxSpeed
    };
    //@}

private:
    //! @name Member data
    //@{
    E_SpeedPolicy nSpeedPolicy_;
    float         rDefaultSpeed_;
    float         rSpeedPolicyMaxSpeedAgentFactor_;
    //@}
};

#endif