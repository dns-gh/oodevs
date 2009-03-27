// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HealableCapacity_h_
#define __HealableCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"

class MedicalTreatmentAttribute;

class HealableCapacity : public ObjectCapacity_ABC
{
private:
    typedef std::list< PHY_Human >           T_WaitingList;
    typedef T_WaitingList::const_iterator    CIT_WaitingList;
    typedef T_WaitingList::iterator          IT_WaitingList;

public:
	//! @name Constructors/Destructor
    //@{
	explicit HealableCapacity( xml::xistream& xis );
	         HealableCapacity();
    virtual ~HealableCapacity();
	//@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( Object& object );
    //@}

	//! @name virtual
    //@{
    virtual void             Instanciate( Object& object ) const;
    virtual void             Update( Object& object, float time );
	//@}

    //! @name Operators
    //@{
    const std::string GetFirstWaitingPatientInjuryName() const;
    int   GetFirstWaitingPatientInjuryCategory() const;
    bool  UpdateInitialState( MedicalTreatmentAttribute& attr, float time );
    bool  UpdateState( MedicalTreatmentAttribute& attr );
    void  DeactivateEmergencyPlan();
    void  ActivateEmergencyPlan();
    void  MakeDiagnosis( MedicalTreatmentAttribute& attr );
    //@}

    //! @name DecFunctions
    //@{
    void TransferPatient();
    void ReceivePatient( const PHY_Human& injuredHuman );
    //@}

private:
    //! @name Copy
    //@{
    HealableCapacity( const HealableCapacity& );
    //@}

    //! @name Initialize
    //@{
    void InitializeData( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    bool  bEmergencyPlan_;
    bool  bBusinessHours_;

    float emergencyBedsRate_;
    float emergencyDoctorsRate_;
    float nightDoctorsRate_;

    T_WaitingList careWaitingList_;
    T_WaitingList transferWaitingList_;
    T_WaitingList entranceWaitingList_;

    //@}
};

#endif // __HealableCapacity_h_