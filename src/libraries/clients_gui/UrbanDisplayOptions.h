// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __UrbanDisplayOptions_h_
#define __UrbanDisplayOptions_h_

#include "clients_kernel/OptionsObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/Observer_ABC.h"
#include "Gradient.h"
#include "urban/Motivations.h"

namespace urban
{
    class ColorAttribute;
}

namespace kernel
{
    class Controllers;
}

namespace gui
{
    struct ChangePopulationDisplay;
// =============================================================================
/** @class  UrbanDisplayOptions
    @brief  UrbanDisplayOptions
*/
// Created: LDC 2011-03-25
// =============================================================================
class UrbanDisplayOptions : public tools::Observer_ABC
                          , public kernel::OptionsObserver_ABC
                          , public tools::ElementObserver_ABC< gui::ChangePopulationDisplay >
{

public:
    //! @name Constructors/Destructor
    //@{
             UrbanDisplayOptions( kernel::Controllers& controllers );
    virtual ~UrbanDisplayOptions();
    //@}

    //! @name Operations
    //@{
    void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    bool SetColor( urban::ColorAttribute* attribute, float density, const std::map< std::string, float > & motivations );
    void ChangePopulationDisplay( const std::string& name, bool visible );
    void NotifyUpdated( const gui::ChangePopulationDisplay& population );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanDisplayOptions( const UrbanDisplayOptions& );            //!< Copy constructor
    UrbanDisplayOptions& operator=( const UrbanDisplayOptions& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    bool densityColor_;
    bool accommodationColor_;
    QString accommodationDisplayed_;
    QColor unoccupiedDensity_;
    float minDensity_;
    float maxDensity_;
    std::auto_ptr< Gradient > pGradient_;
    QColor unoccupiedAccommodation_;
    float minAccommodationDensity_;
    float maxAccommodationDensity_;
    std::auto_ptr< Gradient > pAccommodationGradient_;
    //@}
};
}

#endif // __UrbanDisplayOptions_h_
