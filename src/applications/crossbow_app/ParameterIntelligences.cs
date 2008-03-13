using System;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Framework;

namespace Sword
{
    namespace Crossbow
    {
        public sealed class ParameterIntelligences : IOrderParameter
        {
            public void Serialize(ITable table, int id)
            {
                IRow row = table.CreateRow();
                Tools.SetValue(row, "order_id", id);
                Tools.SetValue(row, "name", "Intelligences");
                Tools.SetValue(row, "ParamValue", "");
                row.Store();
            }

            public void OnContextMenu(MultiItemContextMenu menu, int x, int y, IFeature selected)
            {
                throw new Exception("Intelligences.OnContextMenu should not be here.");
            }

            public void SetValue(string value)
            {
                // NOTHING
            }

            public void NotifyUpdate(IMissionObserver observer)
            {
                // NOTHING
            }

            public string Name
            {
                get
                {
                    return "Intelligences";
                }
            }
        }
    }
}
