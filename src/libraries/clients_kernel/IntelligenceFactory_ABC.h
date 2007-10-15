// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IntelligenceFactory_ABC_h_
#define __IntelligenceFactory_ABC_h_

namespace kernel
{
    class Intelligence_ABC;
    class Karma;
    class Team_ABC;

// =============================================================================
/** @class  IntelligenceFactory_ABC
    @brief  IntelligenceFactory_ABC
*/
// Created: SBO 2007-10-12
// =============================================================================
class IntelligenceFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             IntelligenceFactory_ABC() {}
    virtual ~IntelligenceFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual Intelligence_ABC& Create( Team_ABC& team, const std::string& symbol, const std::string& level, const Karma& karma, const geometry::Point2f& position ) = 0;
    //@}
};

}

#endif // __IntelligenceFactory_ABC_h_
