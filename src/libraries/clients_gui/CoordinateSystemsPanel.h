// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __CoordinateSystemsPanel_h_
#define __CoordinateSystemsPanel_h_

#include "PreferencePanel_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class CoordinateSystems;
    class Controllers;
    class Options;
}

namespace gui
{
// =============================================================================
/** @class  CoordinateSystemsPanel
    @brief  CoordinateSystemsPanel
*/
// Created: AME 2010-03-15
// =============================================================================
class CoordinateSystemsPanel : public PreferencePanel_ABC
                      , public tools::Observer_ABC
                      , public kernel::OptionsObserver_ABC
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             CoordinateSystemsPanel( QWidget* parent, kernel::Controllers& controllers, kernel::CoordinateSystems& coordSystems );
    virtual ~CoordinateSystemsPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    virtual void Reset();
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    CoordinateSystemsPanel( const CoordinateSystemsPanel& );            //!< Copy constructor
    CoordinateSystemsPanel& operator=( const CoordinateSystemsPanel& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
     kernel::Options& options_;
     kernel::Controllers& controllers_;
     kernel::CoordinateSystems& coordinateSystems_;
     QComboBox* listCoordSys_;
     int previousCoordinateSystem_;
    //@}
};
}
#endif // __CoordinateSystemsPanel_h_
