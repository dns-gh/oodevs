using System;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.MOLE;

namespace crossbow
{
    abstract class FeatureDrawer_ABC
    {
        public abstract void InitializeDisplay(IDynamicElement symbol, float factor, IDynamicSymbolProperties properties);
        public abstract void DrawEnvelope(IDynamicDisplay dynamicDisplay, double fromPoint, IDynamicSymbolProperties properties);
        public abstract void Draw(IDynamicDisplay dynamicDisplay, FieldsProperty m_Fields);
    }
}