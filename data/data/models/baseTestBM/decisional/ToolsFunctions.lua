
--TODO Ugly, try to avoid the string comparison
function CreateKnowledge( strKnowledgeType, knowledgeSource )
  if( strKnowledgeType == "net.masagroup.world.concrete.Point" ) then
    return knowledges.create( net.masagroup.world.concrete.Point, knowledgeSource )
  else
    error( "Knowledge unknown : " .. strKnowledgeType )
  end
end

function InitTaskParameter( params, strKnowledgeType, name, knowledgeSource, list )
    if list then
      params[name] = {};
      for i = 1, #knowledgeSource do
        params[name][i] = CreateKnowledge( strKnowledgeType, knowledgeSource[i] )
      end
    else
      params[name] = { CreateKnowledge( strKnowledgeType, knowledgeSource ) }
    end
end 



actionCallbacks = {}
actionKnowledgeCallbacks = {}

CallbackAction = function( action, value )
    if actionCallbacks[ action ] then
        actionCallbacks[ action ]( value )
    end
end

KnowledgeCallbackAction = function( action, value )
    if actionKnowledgeCallbacks[ action ] then
        actionKnowledgeCallbacks[ action ]( value )
    end
end

RemoveAction = function( action )
    actionCallbacks[ action ] = nil
    actionKnowledgeCallbacks[ action ] = nil
end

LinearInterpolation = function( min, max, start, stop, upslope, value )
  local res = 0;
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