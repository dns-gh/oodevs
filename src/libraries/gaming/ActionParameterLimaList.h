// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLimaList_h_
#define __ActionParameterLimaList_h_

#include "game_asn/Asn.h"
#include "ActionParameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

class Simulation;

// =============================================================================
/** @class  ActionParameterLimaList
    @brief  ActionParameterLimaList
*/
// Created: SBO 2007-04-16
// =============================================================================
class ActionParameterLimaList : public ActionParameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameterLimaList( const kernel::OrderParameter& parameter );
             ActionParameterLimaList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const Simulation& simulation, const ASN1T_LimasOrder& limas );
             ActionParameterLimaList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const Simulation& simulation, xml::xistream& xis );
    virtual ~ActionParameterLimaList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_OrderContext& asn ) const;
    virtual void Clean( ASN1T_OrderContext& asn ) const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameterLimaList( const ActionParameterLimaList& );            //!< Copy constructor
    ActionParameterLimaList& operator=( const ActionParameterLimaList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadLima( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter, const Simulation& simulation );
    //@}
};

#endif // __ActionParameterLimaList_h_
