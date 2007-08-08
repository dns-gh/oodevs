using System;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.MOLE;

namespace crossbow
{
    public interface IDynamicElement
    {
        ICachedGraphic CachedGraphic { get; }
        IDynamicGlyph Glyph { get; }
    }    
}