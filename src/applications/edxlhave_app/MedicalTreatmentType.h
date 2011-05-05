// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __MedicalTreatmentType_h_
#define __MedicalTreatmentType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace edxl
{

// =============================================================================
/** @class  MedicalTreatmentType
    @brief  MedicalTreatmentType
*/
// Created: BCI 2011-05-02
// =============================================================================
class MedicalTreatmentType : boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
    MedicalTreatmentType( xml::xistream& xis );
    virtual ~MedicalTreatmentType();
    //@}

    //! @name Accessors
    //@{
    unsigned int GetId() const;
    const std::string& GetName() const;
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    std::string name_;
    //@}
};

}

#endif // __MedicalTreatmentType_h_
