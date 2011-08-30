// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalCapacity_h_
#define __MedicalCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "Entities/Agents/Units/Humans/PHY_InjuredHuman.h"

#include <queue>

class MedicalTreatmentAttribute;

// =============================================================================
/** @class  MedicalCapacity
    @brief  MedicalCapacity
*/
// Created: RFT 2008-05-30
// =============================================================================
class MedicalCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MedicalCapacity();
    explicit MedicalCapacity( xml::xistream& xis );
             MedicalCapacity( float emergencyBedsRate, float emergencyDoctorsRate, float nightDoctorsRate );

    virtual ~MedicalCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void Update( MIL_Object_ABC& object, unsigned int time );
    void Update( xml::xistream& xis, const MIL_Object_ABC& /*object*/ );

    //! @name Decisional functions
    //@{
    void TransferPatient();
    void ReceivePatient( PHY_InjuredHuman& injuredHuman );
    bool CanTreat( const MIL_Object_ABC& object, const PHY_InjuredHuman& injuredHuman );
    //@}

private:
    //! @name Copy
    //@{
    MedicalCapacity( const MedicalCapacity& );
    //@}

    //! @name
    //@{
    void RequestTransfert( PHY_InjuredHuman& injuredHuman );
    void MakeDiagnosis( MedicalTreatmentAttribute& attr );
    void Accept( MedicalTreatmentAttribute& attr, PHY_InjuredHuman& injuredHuman );
    //@}

    //! @name Helpers
    //@{
    void InitializeData( xml::xistream& xis );
    //@}

private:
    typedef std::queue< PHY_InjuredHuman* >    T_WaitingList;

private:
    //! @name Member data
    //@{
    float emergencyBedsRate_;
    float emergencyDoctorsRate_;
    float nightDoctorsRate_;

    T_WaitingList entranceWaitingList_;
    T_WaitingList transferWaitingList_;
    T_WaitingList deadsList_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( MedicalCapacity )

#endif // __MedicalCapacity_h_
