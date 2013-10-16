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

#include "clients_kernel/OptionsObserver_ABC.h"
#include <tools/Resolver.h>

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Options;
}

namespace gui
{
    class Gradient;
}

namespace gui
{
// =============================================================================
/** @class  GradientPreferences
    @brief  GradientPreferences
*/
// Created: SBO 2007-07-03
// =============================================================================
class GradientPreferences : public tools::Resolver< Gradient, QString >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit GradientPreferences( kernel::Options& options );
    virtual ~GradientPreferences();
    //@}

    //! @name Operations
    //@{
    void Commit( const std::vector< Gradient* >& presets );
    void Reset();
    void SetGradient( const QString& name, const QString& values );
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
    void Save() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Options& options_;
    //@}
};

}

#endif // __GradientPreferences_h_
