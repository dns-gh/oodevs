 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalTreatmentAttribute_h_
#define __MedicalTreatmentAttribute_h_

#include "ObjectAttribute_ABC.h"
#include "protocol/Protocol.h"
#include <vector>

namespace dispatcher
{

// =============================================================================
/** @class  MedicalTreatmentAttribute
    @brief  MedicalTreatmentAttribute
*/
// Created: RFT 2006-09-19
// =============================================================================
class MedicalTreatmentAttribute : public ObjectAttribute_ABC
{
private:
    struct MedicalCapacity
    {
        MedicalCapacity() : typeId_ ( -1 ), baseline_ ( 0 ), available_ ( 0 ), emergency_ ( 0 ) {}

        void Update( const sword::MedicalTreatmentBedCapacity& capacity );
        void Send( sword::MedicalTreatmentBedCapacity& capacity ) const;

        unsigned baseline_;
        unsigned available_;
        unsigned emergency_;
        int typeId_;
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit MedicalTreatmentAttribute( const sword::ObjectAttributes& asnMsg );
    virtual ~MedicalTreatmentAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const sword::ObjectAttributes& asnMsg );
    virtual void Send  ( sword::ObjectAttributes& asnMsg ) const;
    //@}

private:
    //! @name 
    //@{
    void Update( const sword::ObjectAttributeMedicalTreatment& message );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MedicalCapacity >  T_TreatmentCapacityVector;
    typedef sword::ObjectAttributeMedicalTreatment_EnumMedicalTreatmentStatus EnumMedicalTreatmentStatus;
    //@}

private:
    //! @name Data members
    //@{
    std::string                 referenceID_;
    EnumMedicalTreatmentStatus  status_;
    T_TreatmentCapacityVector   capacities_;
    int                         doctors_;
    int                         availableDoctors_;
    //@}
};

}

#endif // __MedicalTreatmentAttribute_h_
