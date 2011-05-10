// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef gui_DiffusionListFunctors_h_
#define gui_DiffusionListFunctors_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{

// =============================================================================
/** @class  DiffusionListFunctor_ABC
    @brief  DiffusionListFunctor_ABC
*/
// Created: ABR 2011-05-02
// =============================================================================
class DiffusionListFunctor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DiffusionListFunctor_ABC( const std::string name );
    virtual ~DiffusionListFunctor_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void operator()( const kernel::Entity_ABC& agent ) const = 0;
    //@}

protected:
    //! @name Member data
    //@{
    const std::string name_;
    //@}
};

// =============================================================================
/** @class  DiffusionListGenerator
    @brief  DiffusionListGenerator
*/
// Created: ABR 2011-05-02
// =============================================================================
class DiffusionListGenerator : public DiffusionListFunctor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DiffusionListGenerator( const std::string name );
    virtual ~DiffusionListGenerator();
    //@}

    //! @name Operations
    //@{
    virtual void operator()( const kernel::Entity_ABC& agent ) const;
    //@}
};

// =============================================================================
/** @class  DiffusionListClearer
    @brief  DiffusionListClearer
*/
// Created: ABR 2011-05-04
// =============================================================================
class DiffusionListClearer : public DiffusionListFunctor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DiffusionListClearer( const std::string name );
    virtual ~DiffusionListClearer();
    //@}

    //! @name Operations
    //@{
    virtual void operator()( const kernel::Entity_ABC& agent ) const;
    //@}
};

}

#endif // gui_DiffusionListFunctors_h_
