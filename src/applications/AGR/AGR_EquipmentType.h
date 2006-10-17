// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AGR_EquipmentType_h_
#define __AGR_EquipmentType_h_

#include "AGR_Type_ABC.h"

// =============================================================================
/** @class  AGR_EquipmentType
    @brief  AGR_EquipmentType
*/
// Created: AGE 2006-10-17
// =============================================================================
class AGR_EquipmentType : public AGR_Type_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             AGR_EquipmentType();
    virtual ~AGR_EquipmentType();
    //@}

    //! @name Operations
    //@{
    virtual std::string MOS2RCCode( const std::string& strAsnParam ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AGR_EquipmentType( const AGR_EquipmentType& );            //!< Copy constructor
    AGR_EquipmentType& operator=( const AGR_EquipmentType& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __AGR_EquipmentType_h_
