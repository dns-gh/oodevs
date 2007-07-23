using ESRI.ArcGIS.Display;

namespace crossbow
{
    public interface IFeatureDrawer
    {
        void InitializeDisplay(IDynamicElement symbol, float factor, IDynamicSymbolProperties properties);
        void DrawEnvelope(IDynamicDisplay dynamicDisplay, double fromPoint, IDynamicSymbolProperties properties);
        void Draw(IDynamicDisplay dynamicDisplay, FieldsProperty m_Fields);
    }
}