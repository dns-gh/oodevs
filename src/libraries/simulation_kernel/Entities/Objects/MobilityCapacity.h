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

class MobilityCapacity : public ObjectCapacity_ABC
{
public:
	//! @name Constructors/Destructor
    //@{
	explicit MobilityCapacity( xml::xistream& xis );
             MobilityCapacity();
	virtual ~MobilityCapacity();
	//@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( Object& /*object*/ );
    //@}

	//! @name virtual
    //@{
    virtual void Instanciate( Object& object ) const;
	//@}

    //! @name 
    MT_Float GetDefaultSpeed() const;
    MT_Float ApplySpeedPolicy( MT_Float rAgentSpeedWithinObject, MT_Float rAgentSpeedWithinEnvironment, MT_Float rAgentMaxSpeed ) const;
    //@}

private:
    //! @name Copy
    //@{
    MobilityCapacity( const MobilityCapacity& from );
    //@}

    //! @name Initialize
    //@{
    void InitializeSpeed( xml::xistream& xis );
    void InitializeSpeedPolicy( xml::xistream& xis );
    //@}

private:
    //! @name Type
    //@{
    enum E_SpeedPolicy
    {
        eSpeedPolicy_Slowest,
        eSpeedPolicy_AgentMaxSpeed,
        eSpeedPolicy_ObjectMaxSpeed
    };
    //@}

private:
    //! @name 
    //@{
    E_SpeedPolicy   nSpeedPolicy_;
    float           rDefaultSpeed_;
    float           rSpeedPolicyMaxSpeedAgentFactor_;
    //@}
};

#endif