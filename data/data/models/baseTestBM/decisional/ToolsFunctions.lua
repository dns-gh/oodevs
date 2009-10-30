
--TODO Ugly, try to avoid the string comparison
function CreateKnowledge( strKnowledgeType, knowledgeSource, knowledgedPosition )
  if( strKnowledgeType == "net.masagroup.sword.military.world.Point" ) then
    --default_engine.createPoint( {}, knowledgedPosition[1], knowledgedPosition[2], knowledgedPosition[3] )
    return knowledges.create( net.masagroup.sword.military.world.Point, knowledgeSource )
  else
    error( "Knowledge unknown : " .. strKnowledgeType )
  end
end

function InitTaskParameter( params, strKnowledgeType, name, knowledgeSource, knowledgedPosition, list )
    if( name == "none" ) then
      if list then
        for i = 1, #knowledgeSource do
          params[i] = CreateKnowledge( strKnowledgeType, knowledgeSource[i], knowledgedPosition[i] )
        end
      else
        params[1] = CreateKnowledge( strKnowledgeType, knowledgeSource, knowledgedPosition )
      end
    elseif list then
      params[name] = {};
      for i = 1, #knowledgeSource do
        params[name][i] = CreateKnowledge( strKnowledgeType, knowledgeSource[i], knowledgedPosition[i] )
      end
    else
      params[name] = { CreateKnowledge( strKnowledgeType, knowledgeSource, knowledgedPosition ) }
    end
end

function InitializeMe( strKnowledgeType, knowledgeSource, x, y ,z )
    --default_engine.createMe( {}, x, y, z )
    knowledges.initializeMe( strKnowledgeType, knowledgeSource )
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