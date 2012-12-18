// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_AmmunitionForIndirectFireData_h
#define fire_module_AmmunitionForIndirectFireData_h

#include <wrapper/View.h>
#include <boost/noncopyable.hpp>

class MT_Vector2D;

namespace sword
{
namespace fire
{
    class ModuleFacade;
    class DotationCategory;

// =============================================================================
/** @class  AmmunitionForIndirectFireData
    @brief  Ammunition for indirect fire data
*/
// Created: JVT 2004-08-03
// =============================================================================
class AmmunitionForIndirectFireData : boost::noncopyable
{
public:
             AmmunitionForIndirectFireData( const wrapper::View& firer, const std::string& type, const MT_Vector2D* target );
    virtual ~AmmunitionForIndirectFireData();

    //! @name Operations
    //@{
    void ApplyOnWeapon( ModuleFacade& module, const wrapper::View& model, const wrapper::View& component, const wrapper::View& weapon );

    int GetResult() const;
    //@}

private:
    //! @name Member data
    //@{
    const wrapper::View firer_;
    const std::string type_;
    const MT_Vector2D* target_;
    const DotationCategory* pCategory_;
    double dotationValue_;
    //@}
};

}
}

#endif // fire_module_AmmunitionForIndirectFireData_h
