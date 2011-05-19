// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FireHumanDamages_h_
#define __FireHumanDamages_h_

#include "Extractors.h"
#include "FilterHelper.h"

#include "IdentifierValue_ABC.h"

namespace extractors
{

// =============================================================================
/** @class  FireHumanDamages
    @brief  FireHumanDamages
*/
// Created: FPO 2011-04-29
// =============================================================================
class FireHumanDamages : public Extractor< NumericValue >
{
public:
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

public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             FireHumanDamages();
    /*explicit*/ FireHumanDamages( xml::xistream& xis );
    virtual ~FireHumanDamages();
    //@}

    //! @name Operations
    //@{
    bool IsCreation( const sword::SimToClient& wrapper ) const
    {
        return wrapper.message().has_control_begin_tick();
    }
    bool IsDestruction( const sword::SimToClient& wrapper ) const
    {
        return wrapper.message().has_control_end_tick();
    }
    bool HasValue( const sword::SimToClient& wrapper ) const
    {
        return wrapper.message().has_unit_damaged_by_unit_fire();
    }
    float Extract( const sword::SimToClient& wrapper ) const;
    //@}

private:
    //! @name Member data
    //@{
    int rankMask_;
    int stateMask_;
    FilterHelper< int > filter_;
    //@}
};

}

#endif // __FireHumanDamages_h_
