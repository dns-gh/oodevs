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

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

class MIL_Object_ABC;
class MIL_Object_ABC;
class MIL_Agent_ABC;
class PHY_Composante_ABC;
class MIL_Army_ABC;
class PHY_Volume;
class MT_Vector2D;

// =============================================================================
/** @class  ControlZoneCapacity
    @brief  ControlZoneCapacity is not available as object definition
            Can be only created and managed by an agent unit during PHY_ActionControlZone
*/
// Created: JCR 2008-06-02
// =============================================================================
class ControlZoneCapacity : public ObjectCapacity_ABC
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
             ControlZoneCapacity();
    explicit ControlZoneCapacity( const MIL_Agent_ABC& controler );
    virtual ~ControlZoneCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    void RetrieveTargets( const MIL_Object_ABC& object, T_TargetVector& targets ) const;
    //@}

private:
    //! @name Copy
    //@{
    ControlZoneCapacity( const ControlZoneCapacity& from );
    //@}

    //! @name Helpers
    //@{
    void ControlTarget( MIL_Agent_ABC* agent, const MIL_Army_ABC* army, double phCoef, T_TargetVector& targets ) const;
    double GetUnitDensityFirePercentage( const PHY_Volume& volume ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< double > T_FirePercentageVector;
    //@}

private:
    //! @name Member data
    //@{
    T_FirePercentageVector vFirePercentages_;
    const MIL_Agent_ABC* controller_;
    const MT_Vector2D* vFirerPosition_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ControlZoneCapacity )

#endif
