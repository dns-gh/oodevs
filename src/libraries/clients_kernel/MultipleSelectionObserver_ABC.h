// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// $$$$ JSR 2012-05-22: TODO à passer dans tools

#ifndef __MultipleSelectionObserver_ABC_h_
#define __MultipleSelectionObserver_ABC_h_

namespace kernel
{
// =============================================================================
/** @class  MultipleSelectionObserver_ABC
    @brief  MultipleSelectionObserver_ABC
*/
// Created: JSR 2012-05-22
// =============================================================================
class MultipleSelectionObserver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MultipleSelectionObserver_ABC() {}
    virtual ~MultipleSelectionObserver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void BeforeMultiSelection() = 0;
    virtual void AfterMultiSelection() = 0;
    //@}
};

// =============================================================================
/** @class  MultipleSelectionObserver_Base
    @brief  MultipleSelectionObserver_Base
*/
// Created: JSR 2012-05-22
// =============================================================================
template< typename T >
class MultipleSelectionObserver_Base
{
public:
    //! @name Constructors/Destructor
    //@{
             MultipleSelectionObserver_Base() {}
    virtual ~MultipleSelectionObserver_Base() {}
    //@}

    //! @name Operations
    //@{
    virtual void MultipleSelect( const std::vector< const T* >& elements ) = 0;
    //@}
};

// =============================================================================
/** @class  MultipleSelectionObserver
    @brief  MultipleSelectionObserver
*/
// Created: JSR 2012-05-22
// =============================================================================
template< typename T >
class MultipleSelectionObserver : public MultipleSelectionObserver_Base< T >, public MultipleSelectionObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MultipleSelectionObserver() {}
    virtual ~MultipleSelectionObserver() {}
    //@}

    //! @name Types
    //@{
    typedef std::vector< const T* >       T_Elements;
    typedef typename T_Elements::iterator         IT_Elements;
    typedef typename T_Elements::const_iterator  CIT_Elements;
    //@}

    //! @name Operations
    //@{
    virtual void BeforeMultiSelection() { elements_.clear(); }
    virtual void AfterMultiSelection() { NotifySelectionChanged( elements_ ); }
    virtual void MultipleSelect( const T_Elements& elements ) { elements_ = elements; }

    virtual void NotifySelectionChanged( const T_Elements& elements ) = 0;
    //@}

private:
    T_Elements elements_;
};

}

#endif // __MultipleSelectionObserver_ABC_h_
