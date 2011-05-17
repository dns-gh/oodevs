// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __DictionaryExtensions_h_
#define __DictionaryExtensions_h_

#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/Updatable_ABC.h"

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

namespace sword
{
    class AutomatAttributes;
    class CrowdUpdate;
    class FormationUpdate;
    class PartyCreation;
    class PopulationUpdate;
    class UnitAttributes;
}

namespace kernel
{
    class Controllers;
    class ExtensionTypes;
}

// =============================================================================
/** @class  DictionaryExtensions
    @brief  DictionaryExtensions
*/
// Created: ABR 2011-05-12
// =============================================================================
class DictionaryExtensions : public kernel::DictionaryExtensions
                           , public kernel::Updatable_ABC< sword::AutomatAttributes >
                           , public kernel::Updatable_ABC< sword::CrowdUpdate >
                           , public kernel::Updatable_ABC< sword::FormationUpdate >
                           , public kernel::Updatable_ABC< sword::PartyCreation >
                           , public kernel::Updatable_ABC< sword::PopulationUpdate >
                           , public kernel::Updatable_ABC< sword::UnitAttributes >
{

public:
    //! @name Constructors/Destructor
    //@{
             DictionaryExtensions( kernel::Controllers& controllers, const std::string& extensionType, const kernel::ExtensionTypes& resolver );
    virtual ~DictionaryExtensions();
    //@}

    //! @name Helpers
    //@{
    void FillParameterList( actions::parameters::ParameterList* parameterList ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::AutomatAttributes& message );
    virtual void DoUpdate( const sword::CrowdUpdate& message );
    virtual void DoUpdate( const sword::FormationUpdate& message );
    virtual void DoUpdate( const sword::PartyCreation& message );
    virtual void DoUpdate( const sword::PopulationUpdate& message );
    virtual void DoUpdate( const sword::UnitAttributes& message );
    template< typename T >
    void Update( const T& message );
    //@}
};

#endif // __DictionaryExtensions_h_
