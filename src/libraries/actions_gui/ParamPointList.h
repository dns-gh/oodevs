// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPointList_h_
#define __ParamPointList_h_

#include "ParamLocationList.h"

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamPointList
    @brief  ParamPointList
*/
// Created: SBO 2007-03-15
// =============================================================================
class ParamPointList : public ParamLocationList
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamPointList( QObject* parent, const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& controller );
    virtual ~ParamPointList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamPointList( const ParamPointList& );            //!< Copy constructor
    ParamPointList& operator=( const ParamPointList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual Param_ABC* CreateElement();
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter& parameter_;
    //@}
};

    }
}

#endif // __ParamPointList_h_
