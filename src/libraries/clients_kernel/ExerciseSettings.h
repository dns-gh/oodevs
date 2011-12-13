// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ExerciseSettings_h_
#define __ExerciseSettings_h_

#include <boost/noncopyable.hpp>
#pragma warning( push )
#pragma warning( disable: 4251 )
#include <boost/variant.hpp>
#pragma warning( pop )


namespace xml
{
    class xistream;
    class xofstream;
}

namespace tools
{
    class Loader_ABC;
    class SchemaWriter_ABC;
}

namespace kernel
{

// =============================================================================
/** @class  ExerciseSettings
    @brief  ExerciseSettings
*/
// Created: ABR 2011-12-09
// =============================================================================
class ExerciseSettings : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseSettings();
    virtual ~ExerciseSettings();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Load( xml::xistream& xis );
    void Serialize( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const;
    //@}

    //! @name Accessors
    //@{
    template< typename T >
    const T& GetValue( const std::string& name ) const;
    template< typename T >
    void SetValue( const std::string& name, T value );
    //@}

private:
    //! @name Types
    //@{
    typedef boost::variant< bool, std::string, int, unsigned int, long, unsigned long, float, double > SettingVariant;

    class Setting : private boost::noncopyable
    {
    public:
        explicit Setting( xml::xistream& xis );
        virtual ~Setting();

        void Serialize( xml::xofstream& xos ) const;

        const std::string            name_;
        const std::string            type_;
        SettingVariant               value_;
    };

    typedef std::map< std::string, Setting* > T_Settings;
    typedef T_Settings::iterator             IT_Settings;
    typedef T_Settings::const_iterator      CIT_Settings;
    //@}

    //! @name Helpers
    //@{
    void ReadSetting( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    T_Settings settings_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ExerciseSettings::GetValue
// Created: ABR 2011-12-09
// -----------------------------------------------------------------------------
template< typename T >
inline
const T& ExerciseSettings::GetValue( const std::string& name ) const
{
    CIT_Settings setting = settings_.find( name );
    if( setting == settings_.end() || setting->second == 0 )
        throw std::runtime_error( __FUNCTION__ "Unknown setting " + name + "." );
    return boost::get< T >( setting->second->value_ );
}

// -----------------------------------------------------------------------------
// Name: ExerciseSettings::SetValue
// Created: ABR 2011-12-09
// -----------------------------------------------------------------------------
template< typename T >
inline
void ExerciseSettings::SetValue( const std::string& name, T value )
{
    CIT_Settings setting = settings_.find( name );
    if( setting == settings_.end() || setting->second == 0 )
        throw std::runtime_error( __FUNCTION__ "Unknown setting " + name + "." );
    setting->second->value_ = value;
}

}

#endif // __ExerciseSettings_h_
