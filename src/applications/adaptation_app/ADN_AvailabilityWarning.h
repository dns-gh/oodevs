// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_AvailabilityWarning_h_
#define __ADN_AvailabilityWarning_h_

#include "ADN_Types.h"
#include "ADN_Type_Vector_ABC.h"

class ADN_AvailabilityWarning;

typedef ADN_Type_Vector_ABC< ADN_AvailabilityWarning >  T_AvailabilityWarning_Vector;
typedef T_AvailabilityWarning_Vector::iterator         IT_AvailabilityWarning_Vector;

class xml::xistream;
class xml::xostream;

// =============================================================================
/** @class  ADN_AvailabilityWarning
    @brief  ADN_AvailabilityWarning
*/
// Created: SBO 2006-08-04
// =============================================================================
class ADN_AvailabilityWarning
        : public ADN_Ref_ABC
        , public ADN_DataTreeNode_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_AvailabilityWarning();
    virtual ~ADN_AvailabilityWarning();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetNodeName();
    std::string GetItemName();

    void ReadArchive( xml::xistream& input );
    void WriteArchive( xml::xostream& output );
    void WriteArchive( xml::xostream& output, const std::string& attribute, const std::string& value );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ADN_AvailabilityWarning( const ADN_AvailabilityWarning& );            //!< Copy constructor
    ADN_AvailabilityWarning& operator=( const ADN_AvailabilityWarning& ); //!< Assignement operator
    //@}

public:
    //! @name Member data
    //@{
    ADN_Type_Int percentage_;
    //@}
};

#endif // __ADN_AvailabilityWarning_h_
