// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef CLIENTS_GUI_URBANDISPLAYOPTIONS_H__
#define CLIENTS_GUI_URBANDISPLAYOPTIONS_H__

#include "Gradient.h"
#include "clients_kernel/HumanDefs.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/Observer_ABC.h>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class AccommodationTypes;
    struct ChangePopulationDisplay;
    class Controllers;
    class UrbanColor_ABC;
    class Usages_ABC;
}

namespace gui
{
// =============================================================================
/** @class  UrbanDisplayOptions
    @brief  UrbanDisplayOptions
*/
// Created: LDC 2011-03-25
// =============================================================================
class UrbanDisplayOptions : public tools::Observer_ABC
                          , public kernel::OptionsObserver_ABC
                          , public tools::ElementObserver_ABC< kernel::ChangePopulationDisplay >
                          , public boost::noncopyable
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
    bool SetColor( kernel::UrbanColor_ABC& color, float livingSpace, const kernel::T_HumansStrMap& humans, const kernel::Usages_ABC& usages, unsigned int structuralState );
    void ChangePopulationDisplay( const std::string& name, bool visible );
    void NotifyUpdated( const kernel::ChangePopulationDisplay& population );
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
    std::unique_ptr< Gradient > pGradient_;
    QColor unoccupiedAccommodation_;
    float minAccommodationDensity_;
    float maxAccommodationDensity_;
    std::unique_ptr< Gradient > pAccommodationGradient_;
    std::set< std::string > populationsDisplayed_;
    //@}
};

} //! namespace kernel

#endif // CLIENTS_GUI_URBANDISPLAYOPTIONS_H__
