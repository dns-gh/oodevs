// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IntelligencePrototype_h_
#define __IntelligencePrototype_h_

namespace kernel
{
    class Intelligence_ABC;
    class IntelligenceFactory_ABC;
    class Karma;
    class Team_ABC;

// =============================================================================
/** @class  IntelligencePrototype
    @brief  IntelligencePrototype
*/
// Created: SBO 2007-10-12
// =============================================================================
class IntelligencePrototype
{

public:
    //! @name Constructors/Destructor
    //@{
             IntelligencePrototype( kernel::Team_ABC& team, const std::string& symbol, const std::string& level, const Karma& karma );
    virtual ~IntelligencePrototype();
    //@}

    //! @name Operations
    //@{
    Intelligence_ABC& CreateIntelligence( IntelligenceFactory_ABC& factory, const geometry::Point2f& point ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IntelligencePrototype( const IntelligencePrototype& );            //!< Copy constructor
    IntelligencePrototype& operator=( const IntelligencePrototype& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
          kernel::Team_ABC& team_;
    const std::string symbol_;
    const std::string level_;
    const Karma& karma_;
    //@}
};

}

#endif // __IntelligencePrototype_h_
