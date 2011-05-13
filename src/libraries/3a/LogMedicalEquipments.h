// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogMedicalEquipments_h_
#define __LogMedicalEquipments_h_

#include "Extractors.h"
#include "FilterHelper.h"

namespace extractors
{

// =============================================================================
/** @class  LogMedicalEquipments
    @brief  LogMedicalEquipments
*/
// Created: FPO 2011-05-03
// =============================================================================
class LogMedicalEquipments : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             LogMedicalEquipments();
    explicit LogMedicalEquipments( xml::xistream& xis );
    virtual ~LogMedicalEquipments();
    //@}

    //! @name Operations
    //@{
    bool HasValue( const sword::SimToClient& wrapper ) const
    {
        return ( wrapper.message().has_log_medical_state() && (
                 wrapper.message().log_medical_state().has_evacuation_ambulances() ||
                 wrapper.message().log_medical_state().has_collection_ambulances() ) );
    }
    int Extract( const sword::SimToClient& wrapper );
    //@}

private:
    //! @name Types
    //@{
    struct Equip
    {
        Equip( bool isAsked = false ) : isAsked_( isAsked ), quantity_( 0 ) { }
        bool isAsked_;
        int quantity_;
    };
    //@}

private:
    //! @name Member data
    //@{
    Equip ambulances_releve_;
    Equip ambulances_ramassage_;
    //@}
};

}

#endif // __LogMedicalEquipments_h_
