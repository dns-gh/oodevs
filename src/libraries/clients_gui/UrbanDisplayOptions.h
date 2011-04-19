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

#include "HumanDefs.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/Observer_ABC.h"
#include "Gradient.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class AccommodationTypes;
    class Controllers;
    class Usages_ABC;
    class UrbanColor_ABC;
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
                          , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanDisplayOptions( kernel::Controllers& controllers, const kernel::AccommodationTypes& accommodationTypes );
    virtual ~UrbanDisplayOptions();
    //@}

    //! @name Operations
    //@{
    void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    bool SetColor( kernel::UrbanColor_ABC& color, float livingSpace, const T_HumansStrMap& humans, const kernel::Usages_ABC& usages );
    void ChangePopulationDisplay( const std::string& name, bool visible );
    void NotifyUpdated( const gui::ChangePopulationDisplay& population );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::AccommodationTypes& accommodationTypes_;
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
    std::set< std::string > populationsDisplayed_;
    //@}
};

}

#endif // __UrbanDisplayOptions_h_
