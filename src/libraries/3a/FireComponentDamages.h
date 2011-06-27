// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireComponentDamages_h_
#define __FireComponentDamages_h_

#include "Extractors.h"
#include "FilterHelper.h"

#include "IdentifierValue_ABC.h"

namespace extractors
{

// =============================================================================
/** @class  FireComponentDamages
    @brief  FireComponentDamages
            components='id1,id2,...'
*/
// Created: AGE 2007-10-24
// =============================================================================
class FireComponentDamages : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    struct IdentifierValueFirer : public IdentifierValue_ABC
    {
        enum { has_parameter = false };
        //! @name Operations
        //@{
        virtual void Receive( const sword::SimToClient& wrapper )
        {
            if( wrapper.message().has_unit_damaged_by_unit_fire() )
                Set( wrapper.message().unit_damaged_by_unit_fire().firer().id() );
        }
        //@}
    };
    struct IdentifierValueTarget : public IdentifierValue_ABC
    {
        enum { has_parameter = false };
        //! @name Operations
        //@{
        virtual void Receive( const sword::SimToClient& wrapper )
        {
            if( wrapper.message().has_unit_damaged_by_unit_fire() )
                Set( wrapper.message().unit_damaged_by_unit_fire().unit().id() );
        }
        //@}
    };
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             FireComponentDamages();
    explicit FireComponentDamages( xml::xistream& xis );
    virtual ~FireComponentDamages();
    //@}

    //! @name Operations
    //@{
    bool HasValue( const sword::SimToClient& wrapper ) const
    {
        return wrapper.message().has_unit_damaged_by_unit_fire();
    }
    NumericValue Extract( const sword::SimToClient& wrapper ) const;
    //@}

private:
    //! @name Member data
    //@{
    FilterHelper< int > componentFilter_;
    FilterHelper< int > partyFilter_;
    int stateMask_;
    bool fratricideMask_;
    bool directFireMask_;
    bool indirectFireMask_;
    //@}
};

}

#endif // __FireComponentDamages_h_
