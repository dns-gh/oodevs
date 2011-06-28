// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FilterDialogs_h_
#define __FilterDialogs_h_

#include <boost/noncopyable.hpp>
#include "tools/Resolver.h"

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

class FilterDialog;
class Menu;
class Model;

// =============================================================================
/** @class  FilterDialogs
    @brief  FilterDialogs
*/
// Created: ABR 2011-06-21
// =============================================================================
class FilterDialogs : public tools::StringResolver< FilterDialog >
                    , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             FilterDialogs( QWidget* parent, const tools::ExerciseConfig& config, Model& model, Menu& menu );
    virtual ~FilterDialogs();
    //@}

    //! @name Operations
    //@{
    void Load();
    void Purge();
    //@}

private:
    //! @name Helpers
    //@{
    void Load( xml::xistream& xis );
    void ReadSection( xml::xistream& xis );
    void CreateImportDialog();
    //@}

private:
    //! @name Data member
    //@{
    const tools::ExerciseConfig& config_;
    Model&                       model_;
    Menu&                        menu_;
    QWidget*                     parent_;
    int                          menuSeparatorId_;
    //@}
};

#endif // __FilterDialogs_h_
