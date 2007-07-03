// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GradientPreferences_h_
#define __GradientPreferences_h_

#include "clients_kernel/Resolver.h"

namespace xml
{
    class xistream;
    class xostream;
}

namespace gui
{
    class Gradient;

// =============================================================================
/** @class  GradientPreferences
    @brief  GradientPreferences
*/
// Created: SBO 2007-07-03
// =============================================================================
class GradientPreferences : public kernel::Resolver< Gradient, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             GradientPreferences();
    virtual ~GradientPreferences();
    //@}

    //! @name Operations
    //@{
    void Commit( const std::vector< Gradient* >& presets );
    void Reset();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GradientPreferences( const GradientPreferences& );            //!< Copy constructor
    GradientPreferences& operator=( const GradientPreferences& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadGradient( xml::xistream& xis );
    void Load( xml::xistream& xis );
    void Save( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

}

#endif // __GradientPreferences_h_
