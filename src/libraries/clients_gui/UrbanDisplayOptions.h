// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_UrbanDisplayOptions_h_
#define __gui_UrbanDisplayOptions_h_

#include "Gradient.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/Observer_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class AccommodationTypes;
    struct ChangePopulationDisplay;
    class Controllers;
    class Options;
    class UrbanObject_ABC;
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
                          , public tools::ElementObserver_ABC< kernel::ChangePopulationDisplay >
                          , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanDisplayOptions( kernel::Controllers& controllers,
                                  const kernel::AccommodationTypes& accommodationTypes );
    virtual ~UrbanDisplayOptions();
    //@}

    //! @name Operations
    //@{
    void Load( const kernel::Options& options );
    QColor ComputeUrbanColor( const kernel::UrbanObject_ABC& object ) const;

    const std::shared_ptr< Gradient >& GetDensityGradient() const;
    const std::shared_ptr< Gradient >& GetAccommodationGradient() const;
    //@}

    //! @name ElementObserver_ABC implementation
    //@{
    void NotifyUpdated( const kernel::ChangePopulationDisplay& population );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::AccommodationTypes& accommodationTypes_;

    std::set< std::string > populationsDisplayed_;
    QString accommodationDisplayed_;

    bool accommodationEnabled_;
    std::shared_ptr< Gradient > accommodationGradient_;
    QColor unoccupiedAccommodation_;
    float minAccommodation_;
    float maxAccommodation_;

    bool densityEnabled_;
    std::shared_ptr< Gradient > densityGradient_;
    QColor unoccupiedDensity_;
    float minDensity_;
    float maxDensity_;
    //@}
};

} //! namespace gui

#endif // __gui_UrbanDisplayOptions_h_
