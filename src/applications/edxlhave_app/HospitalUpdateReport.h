// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __edxl_HospitalUpdateReport_h_
#define __edxl_HospitalUpdateReport_h_


namespace xml
{
    class xostringstream;
    class xostream;
}

namespace sword
{
    class ObjectAttributeMedicalTreatment;
    class MedicalTreatmentBedCapacity;
}

namespace edxl
{
    class Model;

// =============================================================================
/** @class  HospitalUpdateReport
    @brief  HospitalUpdateReport
*/
// Created: JCR 2010-05-31
// =============================================================================
class HospitalUpdateReport
{

public:
    //! @name Constructors/Destructor
    //@{
            HospitalUpdateReport( const std::string& name, const sword::ObjectAttributeMedicalTreatment& medical, const Model& model );
    virtual ~HospitalUpdateReport();
    //@}

    //! @name Operations
    //@{
    void Send( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    HospitalUpdateReport( const HospitalUpdateReport& );            //!< Copy constructor
    HospitalUpdateReport& operator=( const HospitalUpdateReport& ); //!< Assignment operator
    //@}

    //! @name
    //@{
    void BuildReport( const std::string& name, const sword::ObjectAttributeMedicalTreatment& medical );
    //@}

    //! @name Helpers
    //@{
    void UpdateFacilityStatus   ( xml::xostream& xos, const sword::ObjectAttributeMedicalTreatment& object );
    void UpdateDecon            ( xml::xostream& xos, const sword::ObjectAttributeMedicalTreatment& object );
    void UpdateEvacuation       ( xml::xostream& xos, const sword::ObjectAttributeMedicalTreatment& object );
    void UpdateCapacityStatus   ( xml::xostream& xos, const sword::ObjectAttributeMedicalTreatment& object );
    void UpdateBedStatus        ( xml::xostream& xos, const sword::MedicalTreatmentBedCapacity& object );
    void UpdateBedCapacity      ( xml::xostream& xos, const sword::MedicalTreatmentBedCapacity& object );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< xml::xostringstream > xos_;
    const Model& model_;
    //@}
};


}

#endif // __edxl_HospitalUpdateReport_h_
