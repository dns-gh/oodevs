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

#include "ENT/ENT_Enums.h"

namespace kernel
{
    class OptionVariant;
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

    //! @name Copy/Assignment
    //@{
    explicit Options( const Options& other );   //!< Copy constructor
    Options& operator=( const Options& );       //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    void Purge();
    void InitializeGeneral();
    void InitializeView();
    void InitializeLayers( const std::vector< E_LayerTypes >& types );
    void Set( const std::string& name, const OptionVariant& value );
    const OptionVariant& Get( const std::string& name ) const;
    bool Has( const std::string& name ) const;

    void Apply( const std::function< void ( const std::string&, const OptionVariant&, bool ) >& functor ) const;
    void Remove( const std::string& name );

    void Load( Settings_ABC& settings, const std::string& path = "" );
    void Save( Settings_ABC& settings, bool all );
    //@}

private:
    //! @name Helpers
    //@{
    void Create( Settings_ABC& settings, const std::string& name, char type );
    void ReadGradient( xml::xistream& xis );
    void Set( const std::string& name, const OptionVariant& value, bool isInPreferencePanel );
    //@}

    //! @name Types
    //@{
    // map< OptionName, pair< OptionValue, IsInPreferencePanel >
    typedef std::map< std::string, std::pair< OptionVariant, bool > >  T_Options;
    //@}

private:
    //! @name Member data
    //@{
    T_Options options_;
    //@}
};

} //! namespace kernel

#endif // __Options_h_
