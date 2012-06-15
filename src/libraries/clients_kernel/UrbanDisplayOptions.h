// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __kernel_UrbanDisplayOptions_h_
#define __kernel_UrbanDisplayOptions_h_

#include "Gradient.h"
#include "HumanDefs.h"
#include "OptionsObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/Observer_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class AccommodationTypes;
    struct ChangePopulationDisplay;
    class Controllers;
    class UrbanColor_ABC;
    class Usages_ABC;

// =============================================================================
/** @class  UrbanDisplayOptions
    @brief  UrbanDisplayOptions
*/
// Created: LDC 2011-03-25
// =============================================================================
class UrbanDisplayOptions : public tools::Observer_ABC
                          , public OptionsObserver_ABC
                          , public tools::ElementObserver_ABC< kernel::ChangePopulationDisplay >
                          , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanDisplayOptions( Controllers& controllers, const AccommodationTypes& accommodationTypes );
    virtual ~UrbanDisplayOptions();
    //@}

    //! @name Operations
    //@{
    void OptionChanged( const std::string& name, const OptionVariant& value );
    bool SetColor( UrbanColor_ABC& color, float livingSpace, const T_HumansStrMap& humans, const Usages_ABC& usages, unsigned int structuralState );
    void ChangePopulationDisplay( const std::string& name, bool visible );
    void NotifyUpdated( const kernel::ChangePopulationDisplay& population );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    const AccommodationTypes& accommodationTypes_;
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

} //! namespace kernel

#endif // __kernel_UrbanDisplayOptions_h_
