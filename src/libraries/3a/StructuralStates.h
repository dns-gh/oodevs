// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __StructuralStates_h_
#define __StructuralStates_h_

#include "Extractors.h"
#include "FilterHelper.h"

namespace extractors
{

// =============================================================================
/** @class  StructuralStates
    @brief  StructuralStates
FIX ME : Renommer
*/
// Created: FPO 2011-05-13
// =============================================================================
class StructuralStates : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             StructuralStates();
    explicit StructuralStates( xml::xistream& xis );
    virtual ~StructuralStates();
    //@}

    //! @name Operations
    //@{
    bool HasValue( const sword::SimToClient& wrapper ) const
    {
        if( wrapper.message().has_urban_update() && wrapper.message().urban_update().has_attributes() )
        {
            const sword::UrbanAttributes& attributes = wrapper.message().urban_update().attributes();
            if( !hasInfrastructure_ && attributes.has_infrastructures() && attributes.infrastructures().has_infrastructure() )
                hasInfrastructure_ = true;

            return ( hasInfrastructure_ && wrapper.message().has_urban_update()
                && wrapper.message().urban_update().has_attributes()
                && wrapper.message().urban_update().attributes().has_structure() );
        }
        return false;
    }
    int Extract( const sword::SimToClient& wrapper );
    //@}

private:
    //! @name Member data
    //@{
    mutable bool hasInfrastructure_;
    //@}
};

}

#endif // __StructuralStates_h_
