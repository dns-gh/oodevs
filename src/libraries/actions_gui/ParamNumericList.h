// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ParamNumericList_h_
#define __ParamNumericList_h_

#include "ListParameter.h"

namespace kernel
{
    class ActionController;
    class ObjectTypes;
    class OrderParameter;
    class Controller;
}

namespace gui
{
    class ParametersLayer;
}

namespace actions
{
    namespace gui
    {
        // =============================================================================
        /** @class  ParamNumericList
        @brief  ParamNumericList
        */
        // Created: MMC 2011-03-22
        // =============================================================================
        class ParamNumericList : public ListParameter
        {
        public:
            //! @name Constructors/Destructor
            //@{
            ParamNumericList( QObject* parent, const kernel::OrderParameter& parameter, kernel::ActionController& actions, bool isReal );
            virtual ~ParamNumericList();
            //@}

            //! @name Operations
            //@{
            virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
            //@}

        private:
            //! @name Helpers
            //@{
            virtual Param_ABC* CreateElement();
            //@}

        private:
            //! @name Member data
            //@{
            kernel::OrderParameter parameter_;
            bool isReal_;
            unsigned int count_;
            //@}
        };

    }
}

#endif // __ParamNumericList_h_
