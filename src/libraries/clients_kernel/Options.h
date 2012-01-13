// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Options_h_
#define __Options_h_

#include <string>
#include <vector>
#include <map>
#include "OptionVariant.h"
#include <boost/noncopyable.hpp>

namespace tools
{
    class  Observer_ABC;
}
namespace kernel
{
    class OptionsObserver_ABC;
    class Settings_ABC;

// =============================================================================
/** @class  Options
    @brief  Options
*/
// Created: AGE 2006-02-13
// =============================================================================
class Options : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Options();
    virtual ~Options();
    //@}

    //! @name Operations
    //@{
    void Register( tools::Observer_ABC& observer );
    void Unregister( tools::Observer_ABC& observer );

    void Change( const std::string& name, const OptionVariant& value, bool savable = true );
    const OptionVariant& GetOption( const std::string& name, const OptionVariant& defaultValue, bool savable = true );

    void Remove( const std::string& name );

    void Load( Settings_ABC& settings, const std::string& path = "" );
    void Save( Settings_ABC& settings );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< OptionsObserver_ABC* > T_Observers;
    typedef T_Observers::const_iterator       CIT_Observers;

    typedef std::pair< OptionVariant, bool >   T_Savable;
    typedef std::map< std::string, T_Savable > T_Options;
    typedef T_Options::const_iterator        CIT_Options;

    typedef std::vector< std::string > T_Removed;
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void Load( Settings_ABC& settings, const std::string& name, T defaultValue );
    void Clear( Settings_ABC& settings );
    void CreateOption( Settings_ABC& settings, const std::string& name, char type );
    //@}

private:
    //! @name Member data
    //@{
    T_Observers observers_;
    T_Options options_;
    T_Removed removed_;
    //@}
};

}

#endif // __Options_h_
