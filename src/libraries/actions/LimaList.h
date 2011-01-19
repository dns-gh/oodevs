// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterLimaList_h_
#define __ActionParameterLimaList_h_

#include "Parameter.h"

namespace sword
{
    class PhaseLinesOrder;
}

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
             LimaList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const sword::PhaseLinesOrder& limas );
             LimaList( const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, xml::xistream& xis );
    virtual ~LimaList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( sword::MissionParameter& message ) const;
    virtual void CommitTo( sword::MissionParameter_Value& asn ) const;
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
