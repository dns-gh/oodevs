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

#include "game_asn/Simulation.h"
#include "Parameter.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace actions {
    namespace parameters {

// =============================================================================
/** @class  LimaList
    @brief  LimaList
*/
// Created: SBO 2007-04-16
// =============================================================================
class LimaList : public Parameter< QString >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit LimaList( const kernel::OrderParameter& parameter );
             LimaList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const ASN1T_LimasOrder& limas );
             LimaList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~LimaList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    virtual bool IsSet() const;
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DisplayTooltip( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LimaList( const LimaList& );            //!< Copy constructor
    LimaList& operator=( const LimaList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadLima( xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
    //@}
};

    }
}

#endif // __ActionParameterLimaList_h_
