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
    virtual void DisplayTooltip( const gui::Viewport_ABC& viewport, const gui::GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual std::string SerializeType() const;
    //@}
};

}
}

#endif // __ActionParameterLimaList_h_
