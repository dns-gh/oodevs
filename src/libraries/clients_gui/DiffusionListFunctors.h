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

struct LongNameComparator
{
    bool operator() ( const QString& lhs, const QString& rhs ) const;
};

// =============================================================================
/** @class  DiffusionListData
    @brief  DiffusionListData
*/
    // Created: ABR 2012-02-28
// =============================================================================
class DiffusionListData
{
public:
    //! @name Static member data
    //@{
    static const QString separator_;
    static const QRegExp regexp_;
    //@}

public:
    //! @name Types
    //@{
    typedef std::map< QString, const kernel::Entity_ABC*, LongNameComparator >   T_Entities;
    typedef T_Entities::iterator                                                IT_Entities;
    typedef T_Entities::const_iterator                                         CIT_Entities;
    //@}
};

// =============================================================================
/** @class  DiffusionListFunctor_ABC
    @brief  DiffusionListFunctor_ABC
*/
    // Created: ABR 2012-02-28
// =============================================================================
class DiffusionListFunctor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DiffusionListFunctor_ABC( const std::string name, const kernel::Entity_ABC& currentTeam );
    virtual ~DiffusionListFunctor_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void operator()( const kernel::Entity_ABC& agent ) const = 0;
    //@}

protected:
    //! @name Helpers
    //@{
    bool IsFromCurrentTeam( const kernel::Entity_ABC& agent ) const;
    //@}

protected:
    //! @name Member data
    //@{
    const std::string         name_;
    const kernel::Entity_ABC& currentTeam_;
    //@}
};

// =============================================================================
/** @class  DiffusionListReceiversExtractor
    @brief  DiffusionListReceiversExtractor
*/
// Created: ABR 2012-02-28
// =============================================================================
class DiffusionListReceiversExtractor : public DiffusionListFunctor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DiffusionListReceiversExtractor( const std::string name, const kernel::Entity_ABC& currentTeam, DiffusionListData::T_Entities& targets );
    virtual ~DiffusionListReceiversExtractor();
    //@}

    //! @name Operations
    //@{
    virtual void operator()( const kernel::Entity_ABC& agent ) const;
    //@}

private:
    //! @name Member data
    //@{
    DiffusionListData::T_Entities& targets_;
    //@}
};

// =============================================================================
/** @class  DiffusionListEmittersExtractor
    @brief  DiffusionListEmittersExtractor
*/
// Created: ABR 2012-02-28
// =============================================================================
class DiffusionListEmittersExtractor : public DiffusionListFunctor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DiffusionListEmittersExtractor( const std::string name, const kernel::Entity_ABC& currentTeam, DiffusionListData::T_Entities& targets );
    virtual ~DiffusionListEmittersExtractor();
    //@}

    //! @name Operations
    //@{
    virtual void operator()( const kernel::Entity_ABC& agent ) const;
    //@}

private:
    //! @name Member data
    //@{
    DiffusionListData::T_Entities& targets_;
    //@}
};

// =============================================================================
/** @class  DiffusionListGenerator
    @brief  DiffusionListGenerator
*/
// Created: ABR 2012-02-28
// =============================================================================
class DiffusionListGenerator : public DiffusionListFunctor_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DiffusionListGenerator( const std::string name, const kernel::Entity_ABC& currentTeam, QStringList& generatedDiffusionList );
    virtual ~DiffusionListGenerator();
    //@}

    //! @name Operations
    //@{
    virtual void operator()( const kernel::Entity_ABC& agent ) const;
    //@}

private:
    //! @name Member data
    //@{
    QStringList& generatedDiffusionList_;
    //@}
};

}

#endif // gui_DiffusionListFunctors_h_
