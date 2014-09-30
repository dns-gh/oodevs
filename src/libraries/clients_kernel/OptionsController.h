// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __OptionsController_h_
#define __OptionsController_h_

#include <boost/noncopyable.hpp>

namespace tools
{
    class  Observer_ABC;
}
namespace kernel
{
    class Options;
    class OptionVariant;
    class OptionsObserver_ABC;
    class Settings_ABC;

// =============================================================================
/** @class  OptionsController
    @brief  OptionsController
*/
// Created: ABR 2014-06-24
// =============================================================================
class OptionsController : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             OptionsController();
    virtual ~OptionsController();
    //@}

    //! @name Operations
    //@{
    void Register( tools::Observer_ABC& observer );
    void Unregister( tools::Observer_ABC& observer );

    const std::shared_ptr< Options >& GetGeneralOptions() const;
    const std::shared_ptr< Options >& GetViewOptions() const;
    void SetGeneralOptions( const std::shared_ptr< Options >& options );
    void SetViewOptions( const std::shared_ptr< Options >& options );
    void UpdateGeneralOptions();
    void UpdateViewOptions();

    const OptionVariant& GetOption( const std::string& name ) const;
    void Change( const std::string& name, const OptionVariant& value, bool savable = true );
    void Remove( const std::string& name );

    void LoadGeneralOptions( Settings_ABC& settings );
    void SaveGeneralOptions( Settings_ABC& settings );
    void LoadViewOptions( Settings_ABC& settings );
    void SaveViewOptions( Settings_ABC& settings );

    void Load( Settings_ABC& settings );
    void Save( Settings_ABC& settings );
    //@}

private:
    //! @name Member data
    //@{
    std::vector< OptionsObserver_ABC* > observers_;
    std::shared_ptr< Options > generalOptions_;
    std::shared_ptr< Options > viewOptions_;
    //@}
};

}

#endif // __OptionsController_h_
