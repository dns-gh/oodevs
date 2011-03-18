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

namespace gui
{
    class DecimalSpinBoxAndSlider;
}

namespace actions
{
    namespace parameters
    {
        class ParameterList;
    }
}

namespace kernel
{
    class PropertiesDictionary;
}

namespace sword
{
    class PopulationUpdate;
    class CrowdUpdate;
    class UnitAttributes;
}

class QGrid;
class TeamsModel;

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
             Affinities( kernel::Controller& controller, TeamsModel& teams, kernel::PropertiesDictionary& dico );
    virtual ~Affinities();
    //@}

    //! @name Operations
    //@{
    void CreateAffinitiesSpinBoxs( QGrid* grid, std::map< unsigned long, gui::DecimalSpinBoxAndSlider* >& spinboxs );
    void CancelChanges();
    void ValidateChanges();
    void FillParameterList( actions::parameters::ParameterList* parameterList ) const;
    bool HasAffinities() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Affinities( const Affinities& );            //!< Copy constructor
    Affinities& operator=( const Affinities& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::PopulationUpdate& message );
    virtual void DoUpdate( const sword::CrowdUpdate& message );
    virtual void DoUpdate( const sword::UnitAttributes& message );
    void CreateDictionary() const;
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, float > T_Affinities;
    typedef T_Affinities::iterator          IT_Affinities;
    typedef T_Affinities::const_iterator   CIT_Affinities;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    TeamsModel& teams_;
    T_Affinities affinities_;
    T_Affinities changingAffinities_;
    kernel::PropertiesDictionary& dico_;
    //@}
};

#endif // __Affinities_h_
