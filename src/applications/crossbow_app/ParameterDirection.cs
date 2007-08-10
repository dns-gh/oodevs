using System;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace crossbow
{
    public sealed class ParameterDirection : IOrderParameter
    {
        private int m_direction = 0;

        public void Serialize(ITable table, int id)
        {            
            IRow row = table.CreateRow();

            Tools.SetValue(row, "order_id", id);
            Tools.SetValue(row, "name", Name);
            Tools.SetValue(row, "context", true);
            Tools.SetValue(row, "ParamValue", m_direction);
            row.Store();
        }

        public void OnContextMenu(MultiItemSelectionMenu cmd, int x, int y, IFeature selected)
        {
            throw new Exception("Direction.OnContextMenu should not be here.");
        }

        public void SetValue(string value)
        {
            m_direction = int.Parse(value);
        }

        public void NotifyUpdate(IMissionObserver observer)
        {
            // $$$$
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
