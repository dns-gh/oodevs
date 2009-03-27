// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MedicalTreatmentType_h_
#define __MedicalTreatmentType_h_

namespace xml { class xistream; };

namespace kernel
{

// =============================================================================
/** @class  MedicalTreatmentType
    @brief  MedicalTreatmentType
*/
// Created: RFT 2006-04-04
// =============================================================================
class MedicalTreatmentType
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MedicalTreatmentType( xml::xistream& input );
    virtual ~MedicalTreatmentType();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    std::string GetName() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    std::string name_;
    //@}
};

}

#endif // __MedicalTreatmentType_h_
