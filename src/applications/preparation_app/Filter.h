// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Filter_h_
#define __Filter_h_

#include "Filter_ABC.h"

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

class QWidget;

// =============================================================================
/** @class  Filter
    @brief  Filter
*/
// Created: ABR 2011-06-17
// =============================================================================
class Filter : public Filter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Filter();
    virtual ~Filter();
    //@}

    //! @name Operations
    //@{
    virtual const std::string GetName() const;
    virtual const std::string GetDescription() const;
    virtual bool NeedToReloadExercise() const;
    //@}

protected:
    //! @name Helpers
    //@{
    void ReadDescriptions( xml::xistream& xis );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadDescription( xml::xistream& xis );
    //@}

protected:
    //! @name Types
    //@{
    typedef std::map< std::string, std::pair< std::string, std::string > > T_Descriptions;
    //@}

protected:
    //! @name Member data
    //@{
    const std::string currentLanguage_;
    T_Descriptions    descriptions_;
    //@}
};

#endif // __Filter_h_
