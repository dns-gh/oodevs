// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorCondition_h_
#define __SuccessFactorCondition_h_

namespace xml
{
    class xistream;
    class xostream;
}

class Score_ABC;
class ScoresModel;

// =============================================================================
/** @class  SuccessFactorCondition
    @brief  SuccessFactorCondition
*/
// Created: SBO 2009-06-16
// =============================================================================
class SuccessFactorCondition
{

public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorCondition( xml::xistream& xis, const ScoresModel& model );
             SuccessFactorCondition( const Score_ABC& score, const std::string& op, float value );
    virtual ~SuccessFactorCondition();
    //@}

    //! @name Accessors
    //@{
    const Score_ABC& GetScore() const;
    std::string GetOperator() const;
    float GetValue() const;
    //@}

    //! @name Operations
    //@{
    void Serialize( xml::xostream& xos );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorCondition( const SuccessFactorCondition& );            //!< Copy constructor
    SuccessFactorCondition& operator=( const SuccessFactorCondition& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Score_ABC& score_;
    const std::string operator_;
    const float value_;
    //@}
};

#endif // __SuccessFactorCondition_h_
