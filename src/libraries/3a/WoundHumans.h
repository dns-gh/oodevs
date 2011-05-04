// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __WoundHumans_h_
#define __WoundHumans_h_

#include "Extractors.h"
#include "FilterHelper.h"

#include "IdentifierValue_ABC.h"

namespace extractors
{

// =============================================================================
/** @class  WoundHumans
    @brief  WoundHumans
*/
// Created: FPO 2011-05-02
// Ne fonctionne pas FIXME
// =============================================================================
class WoundHumans : public Extractor< NumericValue >
{

public:
    struct IdentifierValue : public IdentifierValue_ABC
    {
    enum { has_parameter = false };
        //! @name Operations
        //@{
        virtual void Receive( const sword::SimToClient& wrapper )
        {
            // Problème pour gérer valeurs multiples pour différents identifier, FIXME
            if( wrapper.message().has_stop_unit_fire() )
                for( int i = 0; i < wrapper.message().stop_unit_fire().units_damages().elem_size(); ++i )
                    Set( wrapper.message().stop_unit_fire().units_damages().elem( i ).target().id() );
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
             WoundHumans();
    /*implicit*/ WoundHumans( xml::xistream& xis );
    virtual ~WoundHumans();
    //@}

    //! @name Operations
    //@{
    bool HasValue( const sword::SimToClient& wrapper ) const
    {
        return wrapper.message().has_stop_unit_fire();
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

#endif // __WoundHumans_h_
