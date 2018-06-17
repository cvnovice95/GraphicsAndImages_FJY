using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Threading;

namespace Clock_1
{

    public partial class Form1 : Form
    {
        
        int X = 0;
        float Rs = 0;
        float Rm = 0;
        float Rh = 0;
        String txt = "";
        Graphics G;
        DateTime now;
        //Thread thtime;
        public Form1()
        {
            
            InitializeComponent();
            //thtime = new Thread(DrawForTime);
           // thtime.Start();
            timer1.Start();
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            G= e.Graphics;
            DrawClock();
        }
        private void DrawClock()
        {
            

            //画表盘
            Pen pen = new Pen(Color.Blue, 5);
            RectangleF rect = new RectangleF(0, 0, 400, 400);
            G.DrawEllipse(pen,rect);

            //画时间点
            TimePoint tp = new TimePoint();
            tp.DrawTimePoint(G);
          

            Pen pen_S = new Pen(Color.Red, 1);
            PointF p_S = new PointF(0,0);
            PointF P_S_2 = new PointF(0,-110);
            G.RotateTransform(Rs*6);
            G.DrawLine(pen_S, p_S, P_S_2);

            G.RotateTransform(-Rs * 6);//归位

            Pen pen_M = new Pen(Color.Green, 3);
            PointF p_M = new PointF(0, 0);
            PointF P_M_2 = new PointF(0, -80);
            G.RotateTransform((float)(0.1*Rs+6*Rm));
            G.DrawLine(pen_M, p_M, P_M_2);

            G.RotateTransform((-1)*(float)(0.1 * Rs + 6 * Rm)); //归位

            Pen pen_H = new Pen(Color.BlueViolet, 3);
            PointF p_H = new PointF(0, 0);
            PointF P_H_2 = new PointF(0, -60);
            G.RotateTransform((float)(Rm/2+Rh*30));
            G.DrawLine(pen_H, p_M, P_H_2);


            G.RotateTransform((-1)*(float)(Rm / 2 + Rh * 30)); //归位
            G.TranslateTransform(-200.0f, -200.0f);
            //时间的书写
            Font font = new Font("Verdana", 16); //定义字体
            String ApearTime = String.Format("{0}:{1}:{2}  {3}", Rh, Rm, Rs,txt);
            G.DrawString(ApearTime, font, new SolidBrush(Color.Red), 150, 430);


        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            now = DateTime.Now;
            int hour = now.Hour;
            int minute = now.Minute;
            int second = now.Second;
            Rs = second;
            Rm = minute;
            if (hour <= 12)
            {
                Rh = hour;
                txt = "AM";
            }
            else
            {
                Rh = hour - 12;
                txt = "PM";
            }
           
            this.Invalidate();//时间触发
        }
        private void DrawForTime()
        {
            while (true)
            {
                Invalidate();
                Thread.Sleep(1000);
            }
        }

    }
}
