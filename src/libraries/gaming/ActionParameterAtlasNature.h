// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterAtlasNature_h_
#define __ActionParameterAtlasNature_h_

#include "ActionParameter.h"
#include "AtlasNature.h"

class AtlasNatures;

// =============================================================================
/** @class  ActionParameterAtlasNature
    @brief  ActionParameterAtlasNature
*/
// Created: SBO 2007-05-24
// =============================================================================
class ActionParameterAtlasNature : public ActionParameter< AtlasNature >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionParameterAtlasNature( const kernel::OrderParameter& parameter, const AtlasNature& nature );
             ActionParameterAtlasNature( const kernel::OrderParameter& parameter, const ASN1T_AtlasNature& asn, const AtlasNatures& natures );
             ActionParameterAtlasNature( const kernel::OrderParameter& parameter, xml::xistream& xis, const AtlasNatures& natures );
    virtual ~ActionParameterAtlasNature();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterAtlasNature( const ActionParameterAtlasNature& );            //!< Copy constructor
    ActionParameterAtlasNature& operator=( const ActionParameterAtlasNature& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    //@}
};

#endif // __ActionParameterAtlasNature_h_
