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

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/OptionalValue.h"
#include "MedicalTreatmentCapacity.h"
#include "tools/Resolver_ABC.h"
#include <vector>

namespace sword
{
    class ObjectAttributeMedicalTreatment;
    class ObjectKnowledgeUpdate;
    class ObjectUpdate;
}

namespace kernel
{
    class Controller;
    class Displayer_ABC;
    class MedicalTreatmentType;
}

// =============================================================================
/** @class  MedicalTreatmentAttribute
    @brief  MedicalTreatmentAttribute
*/
// Created: RFT 2008-02-14
// =============================================================================
class MedicalTreatmentAttribute : public kernel::MedicalTreatmentAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MedicalTreatmentAttribute( kernel::Controller& controller, const tools::Resolver_ABC< kernel::MedicalTreatmentType >& resolver );
    virtual ~MedicalTreatmentAttribute();
    //@}

    //! @name Types
    //@{
    typedef std::map< int, MedicalTreatmentCapacity > T_TreatmentCapacities;
    //@}

    //! @name Accessors
    //@{
    int GetDoctors() const;
    int GetAvailableDoctors() const;
    const std::string& GetReferenceId() const;
    void FillCapacities( T_TreatmentCapacities& data ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInSummary( kernel::Displayer_ABC& displayer ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MedicalTreatmentAttribute( const MedicalTreatmentAttribute& );            //!< Copy constructor
    MedicalTreatmentAttribute& operator=( const MedicalTreatmentAttribute& ); //!< Assignment operator
    //@}

    //! @name
    //@{
    void UpdateData( const sword::ObjectAttributeMedicalTreatment& message );
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );
    virtual void DoUpdate( const sword::ObjectUpdate& message );
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::MedicalTreatmentType >& resolver_;
    int doctors_;
    int availableDoctors_;
    int                     status_;
    std::string             referenceID_;
    T_TreatmentCapacities   capacities_;
    //@}
};

#endif // __MedicalTreatmentAttribute_h_
