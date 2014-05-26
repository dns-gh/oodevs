// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Affinities_h_
#define __Affinities_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Displayable_ABC.h"

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

namespace gui
{
    class PropertiesDictionary;
}

namespace sword
{
    class PopulationUpdate;
    class CrowdUpdate;
    class UnitAttributes;
}

class TeamsModel;
class AffinitiesVisitor_ABC;

// =============================================================================
/** @class  Affinities
    @brief  Affinities
*/
// Created: ABR 2011-01-28
// =============================================================================
class Affinities : public kernel::Extension_ABC
                 , public kernel::Updatable_ABC< sword::PopulationUpdate >
                 , public kernel::Updatable_ABC< sword::CrowdUpdate >
                 , public kernel::Updatable_ABC< sword::UnitAttributes >
{
public:
    //! @name Constructors/Destructor
    //@{
             Affinities( kernel::Entity_ABC& entity, kernel::Controller& controller, TeamsModel& teams, gui::PropertiesDictionary& dico );
    virtual ~Affinities();
    //@}

    //! @name Operations
    //@{
    bool Accept( AffinitiesVisitor_ABC& visitor );
    void FillParameterList( actions::parameters::ParameterList* parameterList ) const;
    void Clear();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Affinities( const Affinities& );            //!< Copy constructor
    Affinities& operator=( const Affinities& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, float > T_Affinities;
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::PopulationUpdate& message );
    virtual void DoUpdate( const sword::CrowdUpdate& message );
    virtual void DoUpdate( const sword::UnitAttributes& message );
    template< typename T >
    void Update( const T& message );
    T_Affinities InitializeAffinities();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC& entity_;
    kernel::Controller& controller_;
    TeamsModel& teams_;
    gui::PropertiesDictionary& dico_;
    const QString baseName_;
    T_Affinities affinities_;
    T_Affinities changingAffinities_;
    //@}
};

#endif // __Affinities_h_
