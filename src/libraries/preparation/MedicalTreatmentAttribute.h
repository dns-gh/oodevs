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
#include "clients_kernel/Resolver_ABC.h"
#include <list>

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
private:
    typedef std::list< const kernel::MedicalTreatmentType* > T_MedicalTreatmentTypeList;
    typedef T_MedicalTreatmentTypeList::const_iterator       CIT_MedicalTreatmentTypeList;
    typedef T_MedicalTreatmentTypeList::iterator             IT_MedicalTreatmentTypeList;

public:
    //! @name Constructors/Destructor
    //@{
    explicit MedicalTreatmentAttribute( kernel::PropertiesDictionary& dico );
             MedicalTreatmentAttribute( xml::xistream& xis, const kernel::Resolver_ABC< kernel::MedicalTreatmentType, std::string >& MedicalTreatmentTypes, kernel::PropertiesDictionary& dico );
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
    void SetData( const kernel::MedicalTreatmentType& type );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MedicalTreatmentAttribute( const MedicalTreatmentAttribute& );            //!< Copy constructor
    MedicalTreatmentAttribute& operator=( const MedicalTreatmentAttribute& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

public:
    //! @name Member data
    //@{
    T_MedicalTreatmentTypeList    typeList_;
    int                           nMedicalTreatmentType_;
    int                           beds_;
    int                           availableBeds_;
    int                           doctors_;
    int                           availableDoctors_;
    //@}
};

#endif // __MedicalTreatmentAttribute_h_
