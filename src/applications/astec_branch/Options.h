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

class Observer_ABC;
class OptionsObserver_ABC;
class OptionVariant;
class Settings;

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

    void LoadBoolean( Settings& settings, const std::string& name, bool defaultValue );
    void LoadInteger( Settings& settings, const std::string& name, int defatulValue );

    void SaveBoolean( Settings& settings, const std::string& name );
    void SaveInteger( Settings& settings, const std::string& name );
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

private:
    //! @name Member data
    //@{
    T_Observers observers_;
    T_Options   options_;
    //@}
};

#endif // __Options_h_
