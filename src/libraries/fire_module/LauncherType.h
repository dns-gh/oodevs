// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_LauncherType_h
#define fire_module_LauncherType_h

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <map>

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
/** @class  LauncherType
    @brief  Launcher type
*/
// Created: JVT 2004-08-03
// =============================================================================
class LauncherType : private boost::noncopyable
{
public:
    //! @name Manager
    //@{
    static void Initialize( xml::xisubstream xis );

    static boost::shared_ptr< LauncherType > FindLauncherType( const std::string& strName );
    //@}

    //! @name Operations
    //@{
    double GetPHModificator( const wrapper::View& firer, const wrapper::View& target ) const;
    double GetPHModificator() const;
    //@}

    //! @name Accessors
    //@{
    bool CanDirectFire() const;
    bool CanIndirectFire() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, boost::shared_ptr< LauncherType > > T_LauncherTypeMap;
    typedef T_LauncherTypeMap::const_iterator CIT_LauncherTypeMap;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     LauncherType( const std::string& strName, xml::xistream& xis );
    //@}

    //! @name Helpers
    //@{
    void InitializeForIndirectFire( xml::xistream& xis );
    void InitializeForDirectFire  ( xml::xistream& xis );
    
    static void ReadLauncher( xml::xistream& xis );
    void ReadDirect         ( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const std::string       strName_;
    bool                    bDirectFire_;
    bool                    bIndirectFire_;
    static T_LauncherTypeMap  launcherTypes_;
    //@}
};

}
}

#endif // fire_module_LauncherType_h
