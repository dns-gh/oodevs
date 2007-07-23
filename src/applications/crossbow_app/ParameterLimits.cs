using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace crossbow
{
    public sealed class ParameterLimits : IOrderParameter
    {
        public void Serialize(ITable table, int id)
        {
            // $$$$ SBO 2007-07-23: TODO!
        }

        public void OnContextMenu(ICommandBar cmdBar, int x, int y, IFeature selected)
        {
            // $$$$ SBO 2007-07-23: TODO!
        }

        public string Name
        {
            get
            {
                return "limits";
            }
        }
    }
}
