-- ****************************************************************************
--
-- This file is part of a MASA library or program.
-- Refer to the included end-user license agreement for restrictions.
--
-- Copyright (c) 2014 MASA Group
--
-- ****************************************************************************

function MakeCombinedResult(result, expected)
    return result .. "-- EXPECTED --\n" .. expected
end

function CreatePolygonFromXY(coords)
    if not coords then
        return nil
    end
    local points = {}
    for i = 1, #coords/2 do
        points[i] = DEC_Geometrie_CreerPointXY(coords[2*i-1], coords[2*i])
    end
    return DEC_Geometrie_CreerLocalisationPolygone(points)
end

function CreatePolygonFromLatLong(coords)
    if not coords then
        return nil
    end
    local points = {}
    for i = 1, #coords/2 do
        points[i] = DEC_Geometrie_CreerPointLatLong(coords[2*i-1], coords[2*i])
    end
    return DEC_Geometrie_CreerLocalisationPolygone(points)
end

function PointToString(point)
    return string.format("(%.2f, %.2f)", point:DEC_Geometrie_X(), point:DEC_Geometrie_Y())
end

function PolygonToString(poly)
    if not poly then
        return "nil"
    end
    output = "["
    if poly then
        local points = DEC_Geometrie_ListePointsLocalisation(poly)
        for i = 1, #points do
        output = output .. PointToString(points[i]) .. ", "
        end
    end
    output = output .. "]"
    return output
end
