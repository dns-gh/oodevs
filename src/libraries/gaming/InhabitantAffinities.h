// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __InhabitantAffinities_h_
#define __InhabitantAffinities_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

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
    class Displayer_ABC;
}

namespace sword
{
    class PopulationUpdate;
}

class QGrid;
class TeamsModel;

// =============================================================================
/** @class  InhabitantAffinities
    @brief  InhabitantAffinities
*/
// Created: ABR 2011-01-28
// =============================================================================
class InhabitantAffinities : public kernel::Extension_ABC
                           , public kernel::Updatable_ABC< sword::PopulationUpdate > 
{
public:
    //! @name Constructors/Destructor
    //@{
             InhabitantAffinities( TeamsModel& teams );
    virtual ~InhabitantAffinities();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC* displayer ) const;
    void CreateAffinitiesSpinBoxs( QGrid* grid, std::map< unsigned long, gui::DecimalSpinBoxAndSlider* >& spinboxs );
    void FillParameterList( actions::parameters::ParameterList* parameterList ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InhabitantAffinities( const InhabitantAffinities& );            //!< Copy constructor
    InhabitantAffinities& operator=( const InhabitantAffinities& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::PopulationUpdate& message );
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned long, float >             T_Affinities;
    typedef T_Affinities::iterator                      IT_Affinities;
    typedef T_Affinities::const_iterator               CIT_Affinities;
    //@}

private:
    //! @name Member data
    //@{
    T_Affinities    affinities_;
    TeamsModel&     teams_;
    //@}
};

#endif // __InhabitantAffinities_h_
