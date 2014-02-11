// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ResourceNetworkDialog_h_
#define __ResourceNetworkDialog_h_

#include "clients_gui/ResourceLinksDialog_ABC.h"
#include <tools/Resolver.h>

namespace kernel
{
    class EntityResolver_ABC;
}

class StaticModel;

// =============================================================================
/** @class  ResourceNetworkDialog
    @brief  ResourceNetworkDialog
*/
// Created: JSR 2010-09-08
// =============================================================================
class ResourceNetworkDialog : public gui::ResourceLinksDialog_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ResourceNetworkDialog( QMainWindow* parent, const kernel::Model_ABC& model, kernel::Controllers& controllers, const StaticModel& staticModel, const kernel::EntityResolver_ABC& resolver );
    virtual ~ResourceNetworkDialog();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoValidate( kernel::Entity_ABC* element = 0 );
    virtual bool DoGenerateProduction();
    virtual void NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode );
    bool IsNetworkValid( const gui::ResourceNode& node, unsigned int id, const std::string& resource, std::set< unsigned int >& array );
    unsigned int ComputeConsumption( unsigned int id, const std::string& resource, double inhabitantConsumption ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::EntityResolver_ABC& resolver_;
    const StaticModel& staticModel_;
    //@}
};

#endif // __ResourceNetworkDialog_h_
