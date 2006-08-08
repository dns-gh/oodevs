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

class Observer_ABC;
class OptionsObserver_ABC;
class Settings_ABC;

// =============================================================================
/** @class  Options
    @brief  Options
*/
// Created: AGE 2006-02-13
// =============================================================================
class Options
{

public:
    //! @name Constructors/Destructor
    //@{
             Options();
    virtual ~Options();
    //@}

    //! @name Operations
    //@{
    void Register( Observer_ABC& observer );
    void Remove  ( Observer_ABC& observer );

    void Change( const std::string& name, const OptionVariant& value );
    const OptionVariant& GetOption( const std::string& name, const OptionVariant& defaultValue );

    void Load( Settings_ABC& settings );
    void Save( Settings_ABC& settings );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Options( const Options& );            //!< Copy constructor
    Options& operator=( const Options& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< OptionsObserver_ABC* > T_Observers;
    typedef T_Observers::const_iterator       CIT_Observers;

    typedef std::map< std::string, OptionVariant > T_Options;
    typedef T_Options::const_iterator            CIT_Options;
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    void Load( Settings_ABC& settings, const std::string& name, T defaultValue );
    //@}

private:
    //! @name Member data
    //@{
    T_Observers observers_;
    T_Options   options_;
    //@}
};

#endif // __Options_h_
