using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace crossbow
{
    public sealed class ParameterDirection : IOrderParameter
    {
        private int m_direction;

        public void Serialize(ITable table, int id)
        {            
            IRow row = table.CreateRow();

            Tools.SetValue(row, "order_id", id);
            Tools.SetValue(row, "name", Name);
            Tools.SetValue(row, "context", true);
            Tools.SetValue(row, "ParamValue", m_direction);
            row.Store();
        }

        public void OnContextMenu(ICommandBar cmd, int x, int y, IFeature selected)
        {
            // NOTHING
        }

        public string Name
        {
            get
            {
                return "direction";
            }
        }
    }
}
