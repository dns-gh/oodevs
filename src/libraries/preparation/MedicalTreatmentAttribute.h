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
#include "clients_kernel/Serializable_ABC.h"
#include "tools/Resolver_ABC.h"
#include <map>

namespace kernel
{
    class Displayer_ABC;
    class MedicalTreatmentType;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  MedicalTreatmentAttribute
    @brief  MedicalTreatmentAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class MedicalTreatmentAttribute : public kernel::MedicalTreatmentAttribute_ABC
                    , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MedicalTreatmentAttribute( const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& treatmentTypes, kernel::PropertiesDictionary& dico );
             MedicalTreatmentAttribute( xml::xistream& xis, const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& treatmentTypes, kernel::PropertiesDictionary& dico );
    virtual ~MedicalTreatmentAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetDoctors( unsigned n );
    void SetReferenceID( const std::string& id );
    void UpdateTreatmentCapacity( const std::string& type, unsigned beds );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MedicalTreatmentAttribute( const MedicalTreatmentAttribute& );            //!< Copy constructor
    MedicalTreatmentAttribute& operator=( const MedicalTreatmentAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadBedCapacity( xml::xistream& xis );
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, unsigned > T_TreatmentCapacities;
    //@}

public:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& resolver_;
    int                     doctors_;
    std::string             referenceID_;
    T_TreatmentCapacities   capacities_;
    //@}
};

#endif // __MedicalTreatmentAttribute_h_
