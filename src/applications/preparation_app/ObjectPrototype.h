// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPrototype_h_
#define __ObjectPrototype_h_

#include "clients_gui/ObjectPrototype_ABC.h"

namespace kernel
{
    class Object_ABC;
    class Team_ABC;
}

namespace tools
{
    class GeneralConfig;
}

class StaticModel;
class ObjectsModel;
class UrbanModel;
class WeatherModel;

// =============================================================================
/** @class  ObjectPrototype
    @brief  Object prototype
*/
// Created: SBO 2006-04-18
// =============================================================================
class ObjectPrototype : public gui::ObjectPrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectPrototype( const QString& objectName,
                              QWidget* parent,
                              kernel::Controllers& controllers,
                              const StaticModel& model,
                              ObjectsModel& objectsModel,
                              const UrbanModel& urbanModel,
                              const WeatherModel& weather,
                              const kernel::Team_ABC& noSideTeam,
                              const std::shared_ptr< gui::ParametersLayer >& layer,
                              const tools::GeneralConfig& config );
    virtual ~ObjectPrototype();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void DoCommit( const kernel::Team_ABC& team );
    //@}

private:
    //! @name Member data
    //@{
    ObjectsModel& model_;
    kernel::Object_ABC* creation_;
    //@}
};

#endif // __ObjectPrototype_h_
