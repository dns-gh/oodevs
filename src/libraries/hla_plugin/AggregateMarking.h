// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_AggregateMarking_h
#define plugins_hla_AggregateMarking_h

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  AggregateMarking
    @brief  Aggregate marking
*/
// Created: AGE 2008-02-21
// =============================================================================
class AggregateMarking
{
public:
    //! @name Constructors/Destructor
    //@{
             AggregateMarking();
    explicit AggregateMarking( const std::string& name );
    virtual ~AggregateMarking();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << markingType_
                << markingData_;
    }
    template< typename Archive >
    void Deserialize( Archive& archive )
    {
        archive >> markingType_
                >> markingData_;
    }
    std::string str() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned char markingType_;
    unsigned char markingData_[31];
    //@}
};

}
}

#endif // plugins_hla_AggregateMarking_h
