using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Drawing2D;
namespace Clock_1
{
    class TimePoint
    {
      
        public TimePoint()
        {
        }
        public void DrawTimePoint(Graphics g)
        {
            Rectangle rect = new Rectangle(0, -180, 10, 20);
            LinearGradientBrush lBrush = new LinearGradientBrush(rect, Color.Green, Color.Yellow, LinearGradientMode.BackwardDiagonal);
            g.FillRectangle(lBrush, rect); 
            SolidBrush sdBrush = new SolidBrush(Color.Black);
            g.TranslateTransform(200.0f, 200.0f);
            for (int i = 0; i < 12; i++)
            {
                g.FillEllipse(lBrush, rect); 
                //g.FillRectangle(sdBrush, 0, -180, 10, 20);
                g.RotateTransform(30.0f);
            }
        }
     
    }
}
