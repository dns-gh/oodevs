// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KeyMarshaller_h_
#define __KeyMarshaller_h_

#include "Reductor_ABC.h"
#include <map>

#pragma warning (push)
#pragma warning (disable : 4355)

// =============================================================================
/** @class  KeyMarshaller
    @brief  KeyMarshaller
*/
// Created: AGE 2007-08-30
// =============================================================================
template< typename K, typename A1, typename A2 >
class KeyMarshaller : public Slot_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit KeyMarshaller( Function2_ABC< K, A1, A2 >& function )
        : function_( function )
        , first_   ( this )
        , second_  ( this )
    {
        // NOTHING
    }
    virtual ~KeyMarshaller()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    Function1_ABC< K, A1 >& FirstParameter()  { return first_; }
    Function1_ABC< K, A2 >& SecondParameter() { return second_; }
    //@}

private:
    //! @name Operations
    //@{
    virtual std::string GetName() const { return function_.GetName(); }
    virtual Slot_ABC* GetSubSlot( unsigned int slot )
    {
        return slot == 0 ? &first_ :
               slot == 1 ? &second_ :
               static_cast< Slot_ABC* >( 0 );
    }
    //@}

    //! @name Helpers
    //@{
    template< typename T >
    struct Function : public Reductor_ABC< K, T >
    {
        Function( KeyMarshaller* that )
            : that_   ( that )
            , done_   ( false )
            , hasKey_ ( false )
            , current_()
        {
            // NOTHING
        }
        virtual void OnBeginTick() {}
        virtual void SetKey( const K& key )
        {
            hasKey_ = true;
            current_ = key;
        }
        virtual void Apply( const T& arg )
        {
            values_[ current_ ] = arg;
        }
        virtual void OnEndTick()
        {
            done_ = true;
            that_->Commit();
        }
        void Reset()
        {
            done_ = hasKey_ = false;
            current_ = K();
            values_.clear();
        }
        KeyMarshaller* that_;
        bool done_;
        bool hasKey_;
        K current_;
        std::map< K, T > values_;
    };
    void Commit()
    {
        if( first_.done_ && second_.done_ )
        {
            function_.BeginTick();
            if( first_.hasKey_ && second_.hasKey_ )
                MergeKeys();
            else if( first_.hasKey_ )
                ForwardFirst();
            else if( second_.hasKey_ )
                ForwardSecond();
            else
                function_.Apply( first_.values_[ K() ], second_.values_[ K() ] );
            first_.Reset();
            second_.Reset();
            function_.EndTick();
        }
    }
    void MergeKeys()
    {
        std::map< K, A1 >::const_iterator firstIt  = first_.values_.begin();
        std::map< K, A2 >::const_iterator secondIt = second_.values_.begin();
        while( firstIt != first_.values_.end() && secondIt != second_.values_.end() )
            if( firstIt->first < secondIt->first )
                ++firstIt;
            else if( secondIt->first < firstIt->first )
                ++secondIt;
            else
            {
                function_.SetKey( firstIt->first );
                function_.Apply ( firstIt->second, secondIt->second );
                ++firstIt;
                ++secondIt;
            }
    }
    void ForwardFirst()
    {
        const A2& second = second_.values_[ K() ];
        for( std::map< K, A1 >::const_iterator it = first_.values_.begin(); it != first_.values_.end(); ++it )
        {
            function_.SetKey( it->first );
            function_.Apply( it->second, second );
        }
    }
    void ForwardSecond()
    {
        const A1& first = first_.values_[ K() ];
        for( std::map< K, A2 >::const_iterator it = second_.values_.begin(); it != second_.values_.end(); ++it )
        {
            function_.SetKey( it->first );
            function_.Apply( first, it->second );
        }
    }
    //@}

private:
    //! @name Member data
    //@{
    Function2_ABC< K, A1, A2 >& function_;
    Function< A1 > first_;
    Function< A2 > second_;
    //@}
};

#pragma warning (pop)

#endif // __KeyMarshaller_h_
