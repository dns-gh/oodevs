require 'debug'

--- Return a linear interpolation between to point
-- @param min The min value on Y axis
-- @param min The max value on Y axis
-- @param start The value on x where interpolation begin
-- @param stop The value on x where interpolation stop
-- @param upslop Indicate if the function inscrease or decrease
-- @param value The x value where we want f(x)
-- @author MGD
-- @release 2010-01-22
LinearInterpolation = function( min, max, start, stop, upslope, value )
    local res = 0;
    if( start == stop ) then
        error( "Can't interpolate if start == stop" )
    end
    if( upslope ) then
        if( value < start ) then
            res = min
        elseif( value > stop ) then
            res = max
        else
            res = min + ( value - start ) * ( max - min ) / ( stop - start )
        end
    else
        if( value < start ) then
            res = max
        elseif( value > stop ) then
            res = min
        else
            res = max + ( value - start ) * ( min - max ) / ( stop - start )
        end
    end
    return res
end

--- Return min value in table
-- @param t: table of numeric values
-- @author DDA
-- @release 2010-11-26
minValue = function( t )
    local minValue = math.huge
    for i, _ in pairs( t ) do
        if t[ i ] < minValue then
            minValue =  t[ i ]
        end
    end
    return minValue
end

--- Splits the string given in parameter with respect to the separator,
--- and returns the result in an array
-- @param separator: the separator
-- @param str: the string to split
-- @author LMT
-- @release 2010-11-18
explode =  function ( seperator, str ) 
    local pos, arr = 0, {}
    for st, sp in function() return string.find( str, seperator, pos, true ) end do
        arr[#arr+1] = string.sub( str, pos, st-1 )
        pos = sp + 1
    end
    arr[#arr+1] = string.sub( str, pos )
    return arr
end

--- Debug function used to display a set of localized elements
-- @param elements : a list of localized elements
affichePositions = function( elements )
    local simPoints = {}
    for _, element in pairs( elements ) do
        local point = element:getPosition()
        simPoints[ #simPoints + 1 ] = point
    end
    DEC_DebugAffichePoints( simPoints )
end

--- Returns true once the delay given in parameter has passed, false otherwise
-- @param self
-- @param delay : the delay in seconds
waitInMin = function( self, delay )
    self[myself] = self[myself] or {}
    self[myself].tempsDebut = self[myself].tempsDebut or getSimulationTime()
    return  delay * 60 <= getSimulationTime() - self[myself].tempsDebut
end

--- Returns the simulation time
getSimulationTime = function()
    return DEC_TempsSim()
end

--- Return a random number between minValue and maxValue, maxValue excluded
-- @param minValue : the minimum value of the range
-- @param maxValue : the maximum value of the range (excluded from the range)
getRandomNumber = function( minValue, maxValue )
    return DEC_RandomValue( minValue, maxValue )
end

--- Returns the action current state
-- @param idAction : the id of the current action
getEtatAction = function( idAction )
    return DEC_EtatAction( idAction )
end