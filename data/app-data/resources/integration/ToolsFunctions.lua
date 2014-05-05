require 'debug'

utilities = utilities or {}

--- Linearly interpolates the given value from the [minFrom ; maxFrom] interval
--- to the [minTo ; maxTo] interval, and returns the value.
--- Throws an error if minFrom is equal to maxFrom.
--- If the given value is not in the [minFrom ; maxFrom] interval, then
--- this method will return either minTo or maxTo, depending on how the
--- given value compares to the initial boundaries.
-- Examples : LinearInterpolation(0, 10, 0, 100, true, 50) returns 5,
-- LinearInterpolation(0, 10, 0, 100, false, 60) returns 4.
-- @param minTo Float, the left bound for the target interval.
-- @param maxTo Float, the right bound for the target interval.
-- @param minFrom Float, the left bound for the starting interval.
-- @param maxFrom Float, the right bound for the starting interval.
-- @param upslope Boolean. If set to false, the [minTo ; maxTo] interval is reversed
-- (i.e. the interpolation occurs from [minFrom ; maxFrom] to [maxTo ; minTo]).
-- @param value Float, the value to interpolate
-- @return Float, the interpolated value (in [minTo ; maxTo])
utilities.LinearInterpolation = function( minTo, maxTo, minFrom, maxFrom, upslope, value )
    if( minFrom == maxFrom ) then
        error( "Can't interpolate if minFrom == maxFrom" )
    end
    
    if not upslope then
        local tmp = maxTo
        maxTo = minTo
        minTo = tmp
    end
    
    if( value < minFrom ) then
        return minTo
    elseif( value > maxFrom ) then
        return maxTo
    else
        return minTo + ( value - minFrom ) * ( maxTo - minTo ) / ( maxFrom - minFrom )
    end
end

--- Returns the smallest number in the given table of numeric values.
-- @param t Table of numeric values
-- @return Float, the min value
utilities.minValue = function( t )
    local minValue = math.huge
    for _, value in pairs( t ) do
        if value < minValue then
            minValue = value
        end
    end
    return minValue
end

--- Splits the string given in parameter with respect to the separator,
--- and returns the result in an array.
-- If the separator is not found in the given string, the returned
-- list contains only the given string.
-- @param separator String, the separator.
-- @param str String, the string to split.
-- @return List of strings
utilities.explode =  function ( separator, str ) 
    if separator == "" then
        error("Can't split a string with an empty separator")
    end
    local pos, arr = 0, {}
    for st, sp in function() return string.find( str, separator, pos, true ) end do
        arr[ #arr + 1 ] = string.sub( str, pos, st-1 )
        pos = sp + 1
    end
    arr[ #arr + 1 ] = string.sub( str, pos )
    return arr
end

--- Displays a set of localized elements (debug function).
-- @param elements Table of knowledges defining a "getPosition" method returning a simulation position.
utilities.affichePositions = function( elements )
    local simPoints = {}
    for _, element in pairs( elements ) do
        local point = element:getPosition()
        simPoints[ #simPoints + 1 ] = point
    end
    DEC_DebugAffichePoints( simPoints )
end

--- Returns true once the delay given in parameter has passed, false otherwise
-- @param knowledge Any knowledge
-- @param delay Float, the delay in minutes
-- @return Boolean, whether the given delay has elapsed or not.
utilities.waitInMin = function( knowledge, delay )
    knowledge[myself] = knowledge[myself] or {}
    knowledge[myself].tempsDebut = knowledge[myself].tempsDebut or getSimulationTime()
    return delay * 60 <= getSimulationTime() - knowledge[myself].tempsDebut
end

--- Returns the current simulation time (in seconds).
-- @return Float
utilities.getSimulationTime = function()
    return DEC_TempsSim()
end

--- Return a random integer between minValue and maxValue.
-- @param minValue Integer, the minimum value of the range (included in the range)
-- @param maxValue Integer, the maximum value of the range (included in the range)
-- @return Integer
utilities.getRandomNumber = function( minValue, maxValue )
    return DEC_RandomValue( minValue, maxValue )
end

--- Returns the current state of the given action
-- @param idAction Integer, the id of the action
-- @return Integer, the id of the current state of the action
utilities.getEtatAction = function( idAction )
    return DEC_EtatAction( idAction )
end

------------------------------------------------------------------
--- DECLARATIONS ENSURING BACKWARDS COMPATIBILITY
------------------------------------------------------------------

LinearInterpolation = utilities.LinearInterpolation

minValue = utilities.minValue

explode = utilities.explode

affichePositions = utilities.affichePositions

waitInMin = utilities.waitInMin

getSimulationTime = utilities.getSimulationTime

getRandomNumber = utilities.getRandomNumber

getEtatAction = utilities.getEtatAction
