// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_DotationCategory_h
#define fire_module_DotationCategory_h

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <set>

namespace xml
{
    class xistream;
    class xisubstream;
}

namespace sword
{
namespace wrapper
{
    class View;
}
namespace fire
{
// =============================================================================
/** @class  DotationCategory
    @brief  Dotation category
*/
// Created: JVT 2004-08-03
// =============================================================================
class DotationCategory
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DotationCategory( xml::xistream& xis );
    virtual ~DotationCategory();
    //@}

    //! @name Operations
    //@{
    static boost::shared_ptr< DotationCategory > FindDotationCategory( const std::string& name );
    static void Initialize( xml::xisubstream xis );

    double ModifyPh( const wrapper::View& firer, const wrapper::View& target, double ph ) const;
    double ModifyDangerosity( const wrapper::View& compTarget ) const;

    unsigned int ReserveAmmunition( const wrapper::View& firer, unsigned int nNbrAmmoToFire );
    //@}

    //! @name Accessors
    //@{
    bool CanBeUsedForDirectFire() const;
    bool CanBeUsedForIndirectFire() const;
    bool HasDotation( const wrapper::View& firer ) const;
    const std::string& GetName() const;
    bool CanFire( const wrapper::View& entity, const wrapper::View& component, int nComposanteFiringType, int ammoDotationClass ) const;
    bool CanFire( const wrapper::View& entity, const wrapper::View& component, const std::string& type ) const;

    double GetValue( const wrapper::View& entity ) const;
    //@}

private:
    //! @name Helpers
    //@{
    static void ReadResource( xml::xistream& xis );
    void ReadAttritions( xml::xistream& xis );
    void ReadIndirectFires( xml::xistream& xis );
    void ReadIndirectFire( xml::xistream& xis );

    bool HasIndirectWeaponCategory( const std::string& type ) const;
    //@}

private:
    //! @name Types
    //@{
    struct CaseInsensitiveCompare
    {
        template< typename T >
        bool operator()( const T& t1, const T& t2 ) const
        {
            return boost::ilexicographical_compare( t1, t2 );
        }
    };
    //@}

private:
    //! @name Member data
    //@{
    std::string strName_;
    std::set< std::string, CaseInsensitiveCompare > indirectTypes_;
    bool directFire_;
    //@}
};

}
}

#endif // fire_module_DotationCategory_h
