// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef kernel_DisasterType_h
#define kernel_DisasterType_h

#include <boost/noncopyable.hpp>
#include <map>

namespace xml
{
    class xistream;
}

namespace kernel
{
// =============================================================================
/** @class  DisasterType
    @brief  Disaster type
*/
// Created: LGY 2012-11-19
// =============================================================================
class DisasterType : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DisasterType( xml::xistream& input );
    virtual ~DisasterType();
    //@}

    //! @name Operations
    //@{
    const std::string& GetName() const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadContaminationThresholds( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< double, std::string > T_Contamination;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    T_Contamination contamination_;
    //@}
};

}

#endif // kernel_DisasterType_h
