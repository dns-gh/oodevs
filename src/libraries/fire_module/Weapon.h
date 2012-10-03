// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_Weapon_h
#define fire_module_Weapon_h

#include <boost/shared_ptr.hpp>
#include <wrapper/View.h>

class MT_Vector2D;

namespace sword
{
namespace fire
{
    class ModuleFacade;
    class WeaponType;
    class DotationCategory;

// =============================================================================
/** @class  Weapon
    @brief  Weapon
*/
// Created: JVT 2004-08-03
// =============================================================================
class Weapon
{
public:
    //! @name Constructors/Destructor
    //@{
    Weapon( ModuleFacade& module, const wrapper::View& model, const wrapper::View& weapon );
    //@}

    //! @name Accessors
    //@{
    bool IsReady() const;

    bool CanDirectFire( const wrapper::View& component, const wrapper::View& parameters ) const;
    bool CanIndirectFire( const wrapper::View& entity, const wrapper::View& component, const std::string& type, const MT_Vector2D* target ) const;

    bool HasDotation( const wrapper::View& firer ) const;

    const DotationCategory& GetDotation() const;
    //@}

    //! @name Operations
    //@{
    //bool     IndirectFire             ( MIL_Agent_ABC& firer, MIL_Effect_IndirectFire& effect );
    void     DirectFire               ( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& compTarget, bool bUsePH ) const;
    void     DirectFire               ( const wrapper::View& firer, const wrapper::View& element ) const;
    //void     ThrowSmoke               ( MIL_Agent_ABC& firer, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmo ) const;
    double GetDangerosity           ( const wrapper::View& firer, const wrapper::View& target, const wrapper::View& targetComposante, bool bUsePH, bool checkAmmo ) const;
    //@}

    //! @name Operators
    //@{
    bool operator==( const Weapon& rhs ) const;
    //@}

private:
    //! @name Helpers
    //@{
    double ModifyReloadingDuration( const wrapper::View& firer, double rDuration ) const;
    //@}

private:
    //! @name Member data
    //@{
    ModuleFacade* module_;
    wrapper::View model_;
    wrapper::View weapon_;
    boost::shared_ptr< WeaponType > type_;
    //@}
};

}
}

#endif // fire_module_Weapon_h
