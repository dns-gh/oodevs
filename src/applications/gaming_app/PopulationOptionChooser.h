// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PopulationOptionChooser_h_
#define __PopulationOptionChooser_h_

#include "clients_kernel/OptionsObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/Resolver.h"

namespace kernel
{
    class AccommodationType;
    class Controllers;
    class Inhabitant_ABC;
    class ModelLoaded;
    class ModelUnLoaded;
}

class StaticModel;

// =============================================================================
/** @class  PopulationOptionChooser
    @brief  PopulationOptionChooser
*/
// Created: LDC 2011-03-23
// =============================================================================
class PopulationOptionChooser : public QDockWindow
                              , public tools::Observer_ABC
                              , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                              , public tools::ElementObserver_ABC< kernel::ModelUnLoaded >
                              , public tools::ElementObserver_ABC< kernel::Inhabitant_ABC >
                              , public kernel::OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             PopulationOptionChooser( QMainWindow* parent, kernel::Controllers& controllers, StaticModel& staticModel );
    virtual ~PopulationOptionChooser();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::ModelUnLoaded& );
    virtual void NotifyCreated( const kernel::Inhabitant_ABC& inhabitant );
    virtual void NotifyDeleted( const kernel::Inhabitant_ABC& inhabitant );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnDensityToggled( bool toggled );
    void OnOccupationToggled( bool toggled );
    void OnAccomodationToggled( bool toggled );
    void OnPopulationToggled( bool ) ;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationOptionChooser( const PopulationOptionChooser& );            //!< Copy constructor
    PopulationOptionChooser& operator=( const PopulationOptionChooser& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QButtonGroup* activities_;
    QButtonGroup* populations_;
    QRadioButton* density_;
    QRadioButton* occupation_;
    tools::StringResolver< kernel::AccommodationType >& accomodations_;
    //@}
};

#endif // __PopulationOptionChooser_h_
