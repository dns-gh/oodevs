--- Return true if the provided element is a value of the given table.
-- @param t Table
-- @param k Any type, the seeked element
-- @return Boolean
exists = function( t, k )
    for _, content in pairs( t ) do
        if content == k then
            return true
        end
    end
    return false
end

--- Returns true if the given table of table contains a table with
-- the provided element as a value.
-- @param t Table containing only tables as values
-- @param k Any type, the seeked element
-- @return Boolean
existsInside = function( t, k )
    for _, content in pairs( t ) do
        for _, element in pairs( content ) do
            if element == k then
                return true
            end
        end
    end
    return false
end

--- Returns true if the provided index is a key of the given table.
-- @param t Table
-- @param i Any type, the seeked index
-- @return Boolean
existsIndex = function( t, i )
    return t[i] ~= nil
end

--- Returns a list containing all the values of the two given tables.
--- Values that appear in both tables only appear once in the returned list.
-- @param table1 First table
-- @param table2 Second table
-- @return List
fusionList = function( table1, table2 )
    local res = {}
    for _, elem in pairs( table1 or emptyTable ) do
        res[ #res+1 ] = elem
    end
    for _, elem in pairs( table2 or emptyTable ) do
        if not exists( res, elem ) then
            res[ #res+1 ] = elem
        end
    end
    return res
end

--- Removes the given value from the provided list, and returns it
-- @param value Any type, the seeked value
-- @param list List upon which the removal will take place
-- @return List, the provided list without the given value
removeFromList = function( value, list )
    for i, k in pairs( list ) do
        if value == k then
            table.remove( list, i )
            break
        end
    end
    return list
end

-- Removes all the given values from the provided list, and returns it
-- @param values Table, list of values to remove
-- @param list List upon which the removal will take place
-- @return List, the provided list without the given values
removeListFromList = function( values, list )
    for _, value in pairs( values ) do
        for i, k in pairs( list ) do
            if value == k then
                table.remove( list, i )
                break
            end
        end
    end
    return list
end

--- Returns a deep copy of the given table
-- @param tableFrom Table of elements to copy
-- @return Table
copyTable =  function ( tableFrom ) 
    local tableTo = {}
    for k, v in pairs( tableFrom or emptyTable ) do
        tableTo[k] = v
    end
    return tableTo
end

--- Returns the size of the given table
-- @param table Table
-- @return Integer, the number of distinct keys in the given table
function tableSize ( table )
    local cpt = 0
    for k in pairs( table or emptyTable ) do
        cpt = cpt + 1
    end
    return cpt
end


--- Clears the given table
-- @param table Table
function clearTable ( table )
    for k in pairs ( table or emptyTable ) do
        table[ k ] = nil
    end
end

--- Returns the inverse of the given table
-- For instance : if tableFrom = { 1 = "a", 2 = "b", 3 = "a" },
-- then the function returns { "a" = { 1, 3 }, "b" = { 2 } }
-- @param tableFrom Table
-- @return Table
integration.inverseTable = function ( tableFrom )
    local tableTo = {}
    for k, v in pairs( tableFrom or emptyTable ) do
        tableTo[ v ] = tableTo[ v ] or {}
        tableTo[ v ][ #tableTo[ v ] + 1 ] = k
    end
    return tableTo
end

--- Shuffles the provided list using a Fisher-Yates shuffle
-- @param list List
-- @return List, a random permutation of the given list
integration.shuffleList = function( list )
    for i = #list, 2, -1 do
        local j = getRandomNumber( 1, i )
        local tmp = list[i]
        list[i] = list[j]
        list[j] = tmp
    end
    return list
end
