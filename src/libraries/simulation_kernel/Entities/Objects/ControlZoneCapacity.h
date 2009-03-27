// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ControlZoneCapacity_h_
#define __ControlZoneCapacity_h_

#include "MIL.h"
#include "ObjectCapacity_ABC.h"
#include "MT_Tools/MT_Random.h"

namespace xml
{
    class xistream;
}

class MIL_Object_ABC;
class Object;
class MIL_Agent_ABC;
class PHY_Composante_ABC;
class MIL_Army_ABC;
class PHY_Volume;

// *****************************************************************************
//
// ControlZoneCapacity is not available as object definition
// Can be only created and managed by an agent unit during PHY_ActionControlZone
//
// *****************************************************************************
class ControlZoneCapacity
    : public ObjectCapacity_ABC
{
public:    
    //! @name Types
    //@{
    typedef std::pair< MIL_Agent_ABC*, PHY_Composante_ABC* > T_TargetPair;
    typedef std::vector< T_TargetPair >                      T_TargetVector;
    typedef T_TargetVector::const_iterator                   CIT_TargetVector;
    //@}

public:
	//! @name Constructors/Destructor
    //@{
    explicit ControlZoneCapacity( const MIL_Agent_ABC& controler );
             ControlZoneCapacity();
    virtual ~ControlZoneCapacity();
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
    //@{
    void RetrieveTargets( const MIL_Object_ABC& object, T_TargetVector& targets ) const;
    //@}

private:
    //! @name Copy
    //@{
    ControlZoneCapacity( const ControlZoneCapacity& from );
    //@}

    //! @name Apply functor
    //@{
    void ControlTarget( MIL_Agent_ABC* agent, const MIL_Army_ABC& army, MT_Float phCoef, T_TargetVector& targets ) const;
    MT_Float GetUnitDensityFirePercentage( const PHY_Volume& volume ) const;
    //@}

private:
    //! @name types
    //@{
    typedef std::vector< MT_Float > T_FirePercentageVector;
    //@}

private:
    //! @name 
    //@{
    T_FirePercentageVector vFirePercentages_;
    //@}

    //! @name 
    //@{    
    const MIL_Agent_ABC* controller_;
    const MT_Vector2D* vFirerPosition_;
    //@}

private:
    //! @name Static
    //@{
    static MT_Random randomGenerator_;
    //@}
};

#endif