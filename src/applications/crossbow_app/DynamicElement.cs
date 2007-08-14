using System;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Geometry;
using ESRI.ArcGIS.MOLE;

namespace Crossbow
{
    public interface IDynamicElement
    {
        ICachedGraphic CachedGraphic { get; }
        IDynamicGlyph Glyph { get; }
        float Width { get; }
        float Height { get; }
    }

    public class DynamicForceElement : IDynamicElement
    {
        private ICachedGraphic m_graphic;
        private IDynamicGlyph m_glyph;
        private float m_width;
        private float m_height;

        public ICachedGraphic CachedGraphic
        {
            get
            {
                return m_graphic;
            }
            set 
            {
                m_graphic = value;
            }
        }
        public IDynamicGlyph Glyph
        {
            get
            {
                return m_glyph;
            }
            set
            {
                m_glyph = value;
                if (m_glyph != null)
                    m_glyph.QueryDimensions(ref m_width, ref m_height);
            }
        }
        public float Width
        {
            get
            {
                return m_width;
            }
        }
        public float Height
        {
            get
            {
                return m_height;
            }
        }
    }
}