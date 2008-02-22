// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PluginConfig_h_
#define __PluginConfig_h_

namespace xml
{
    class xistream;
}

namespace dispatcher
{

// =============================================================================
/** @class  PluginConfig
    @brief  PluginConfig
*/
// Created: SBO 2007-07-24
// =============================================================================
class PluginConfig
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PluginConfig( const std::string& name );
    explicit PluginConfig( xml::xistream& xis );
    virtual ~PluginConfig();
    //@}

    //! @name Operations
    //@{
    std::string GetName() const;
    bool IsEnabled() const;
    std::string GetParameter( const std::string& name ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PluginConfig( const PluginConfig& );            //!< Copy constructor
    PluginConfig& operator=( const PluginConfig& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadParameter( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::string, std::string > T_Parameters;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    bool enabled_;
    T_Parameters parameters_;
    //@}
};

}

#endif // __PluginConfig_h_
