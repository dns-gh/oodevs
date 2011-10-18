// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FilterManager_h_
#define __FilterManager_h_

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include "FilterDescription.h"

namespace tools
{
    class ExerciseConfig;
}

namespace xml
{
    class xistream;
}

class Filter_ABC;
class QListBox;
class QWidgetStack;

// =============================================================================
/** @class  FilterManager
    @brief  FilterManager
*/
// Created: ABR 2011-06-20
// =============================================================================
class FilterManager : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             FilterManager( xml::xistream& xis, const tools::ExerciseConfig& config, Q3ListBox& list, Q3WidgetStack& stack, QWidget& parent );
    virtual ~FilterManager();
    //@}

    //! @name Operations
    //@{
    const std::string GetName() const;
    const std::string GetId() const;
    void AddFilter( Filter_ABC& filter, Q3ListBox& list, Q3WidgetStack& stack );
    Filter_ABC& GetFilter( unsigned int index );
    const Filter_ABC& GetFilter( unsigned int index ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadFilter( xml::xistream& xis, Q3ListBox& list, Q3WidgetStack& stack );
    //@}

private:
    //! @name Type
    //@{
    typedef boost::ptr_vector< Filter_ABC >      T_Filters;
    typedef T_Filters::iterator                 IT_Filters;
    typedef T_Filters::const_iterator          CIT_Filters;
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    const std::string            id_;
    QWidget&                     parent_;
    FilterDescription            description_;
    T_Filters                    filters_;
    //@}
};

#endif // __FilterManager_h_
