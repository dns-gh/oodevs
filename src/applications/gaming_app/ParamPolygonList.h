// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPolygonList_h_
#define __ParamPolygonList_h_

#include "ParamLocationList.h"

// =============================================================================
/** @class  ParamPolygonList
    @brief  ParamPolygonList
*/
// Created: SBO 2007-03-15
// =============================================================================
class ParamPolygonList : public ParamLocationList
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamPolygonList( QObject* parent, const kernel::OrderParameter& parameter, gui::ParametersLayer& layer, const kernel::CoordinateConverter_ABC& converter, kernel::ActionController& controller );
    virtual ~ParamPolygonList();
    //@}

    //! @name Operations
    //@{
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamPolygonList( const ParamPolygonList& );            //!< Copy constructor
    ParamPolygonList& operator=( const ParamPolygonList& ); //!< Assignment operator
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

#endif // __ParamPolygonList_h_
