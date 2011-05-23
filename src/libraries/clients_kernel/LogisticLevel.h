// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************


#ifndef __KERNEL_LOGISTIC_LEVEL_H__
#define __KERNEL_LOGISTIC_LEVEL_H__

namespace kernel
{

// =============================================================================
/** @class  LogisticLevel
    @brief  LogisticLevel
*/
// Created: AHC 2010-10-07
// =============================================================================
class LogisticLevel
{
public:
    //! @name Values
    //@{
    static LogisticLevel none_;
    static LogisticLevel logistic_base_;

    //@}
    //! @name Destructor
    //@{
    virtual ~LogisticLevel();
    //@}

    //! @name accessors
    //@{
    const std::string&  GetName() const;
    int GetId() const;
    bool operator==( const LogisticLevel& rhs ) const;
    bool operator!=( const LogisticLevel& rhs ) const;

    static const LogisticLevel& Resolve( const std::string& name );
    static const LogisticLevel& Resolve( int id );
    //@}

private:
    //! @name Constructor
    //@{
    LogisticLevel(int id_, const std::string& name);
    //@}
    //! @name Copy/Assignment
    //@{
    LogisticLevel( const LogisticLevel& );            //!< Copy constructor
    LogisticLevel& operator=( const LogisticLevel& ); //!< Assignment operator
    //@}

private:
    //! @name Attributes
    //@{
    int id_;
    std::string name_;
    //@}
};

}

#endif // __KERNEL_LOGISTIC_LEVEL_H__
