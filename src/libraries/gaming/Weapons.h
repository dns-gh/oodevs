// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __Weapons_h_
#define __Weapons_h_

#include "clients_gui/Drawable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include <tools/Resolver.h>

namespace kernel
{
    class Controllers;
    class EquipmentType;
    class WeaponSystemType;
}

namespace sword
{
    class UnitAttributes;
}

class Equipment;

// =============================================================================
/** @class  Weapons
    @brief  Weapons
*/
// Created: SBO 2008-08-06
// =============================================================================
class Weapons : public kernel::Extension_ABC
              , public kernel::Updatable_ABC< sword::UnitAttributes >
              , public gui::Drawable_ABC
              , public tools::Resolver< Equipment >
              , public tools::Observer_ABC
              , public kernel::OptionsObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Weapons( kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::EquipmentType >& equipments,
                      const tools::Resolver_ABC< kernel::WeaponSystemType, std::string >& weapons );
    virtual ~Weapons();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const;
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Weapons( const Weapons& );            //!< Copy constructor
    Weapons& operator=( const Weapons& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UnitAttributes& message );
    void UpdateRange();
    void AddEquipmentRange( const kernel::EquipmentType& type, const kernel::WeaponSystemType* filter );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const tools::Resolver_ABC< kernel::EquipmentType >& equipments_;
    const tools::Resolver_ABC< kernel::WeaponSystemType, std::string >& weapons_;
    unsigned int minRange_;
    unsigned int maxRange_;
    unsigned int efficientRange_;
    bool useColor_;
    QColor color_;
    //@}
};

#endif // __Weapons_h_
